/*************************************************************************/
/*!
\file   BulletBehavior.cpp
\author studio 200
\par
\par    DigiPen login:
\par    Course: GAME 200
\par   Copyright � 2017 DigiPen (USA) Corp. and its owners. All Rights Reserved.
\date   9/28/2017
\brief
Functions include:
*/
/*************************************************************************/
#pragma once
#include <Engine/Engine.h>
#include <Transform/Transform.hpp>
#include <Audio/Audio.hpp>
#include "Graphics/Window.hpp"
#include "Physics/ParticleEmitter.hpp"
#include "Physics/ParticleSystem.hpp"
#include "Behaviors/DelayedDestructionBehavior.hpp"
#include "BulletBehavior.h"


/**
 * \brief
 * \param parent
 * \param dmg
 * \param scalar
 */
BulletBehavior::BulletBehavior(GameObject &parent, int dmg, float scalar, unsigned int c) : Component(parent), dmg_(dmg), scalar_(scalar), _color(c)
{
  b_lifetime = 1.5f;
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  //m_handler->Register(Messaging::ID::AnimationUpdate, SetKey(parent.GetKey()), BulletBehavior::Punch);
  m_handler->Register(Messaging::ID::Collision, parent.GetKey(), BulletBehavior::Collision);
}


/**
 * \brief
 */
BulletBehavior::~BulletBehavior()
{
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetKey();
  //m_handler->Unregister(Messaging::ID::AnimationUpdate, key);
  m_handler->Unregister(Messaging::ID::Collision, key);
}

/**
 * \brief
 * \param dt
 */
void BulletBehavior::Update(float dt)
{
  if (!GetParent().IsDestroyed())
  {
    b_lifetime -= dt;

    if (b_lifetime <= 0.0f)
    {
      GetParent().Destroy();
      switchcursortodefualt();
    }
  }
}

/**
 * \brief
 */
void BulletBehavior::Punch(ObjectId obj, const Messaging::Message& message)
{
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);
  object.Find<BulletBehavior>()->onPunch(message);
}

/**
 * \brief
 */
void BulletBehavior::Collision(ObjectId obj, const Messaging::Message& message)
{
  KPE::RigidBody* KeyBody = nullptr;

  if (message.message.collision.body_->GetParent().GetKey() ==  obj) {
    KeyBody = message.message.collision.body_;
  }
  else if (message.message.collision.otherBody_->GetParent().GetKey() == obj) {
    KeyBody = message.message.collision.otherBody_;
  }

  if (message.message.collision.otherBody_->GetParent().GetType() == message.message.collision.body_->GetParent().GetType())
    return;

  if (KeyBody == nullptr)
  {
    return;
  }

  //GameState* state = engine.Find<GameStateManager>()->CurrentState();
  //GameObjectManager& manager = state->Manager();
  GameObject& object = KeyBody->GetParent(); //manager.Find(obj);

  //if (object.GetKey() ==  obj)
  object.Find<BulletBehavior>()->onCollision(message);
}

/**
 * \brief 
 * \param t 
 */
void BulletBehavior::SetLifeTime(float t)
{
  b_lifetime = std::abs(t);
}

/**
 * \brief
 */
void BulletBehavior::onPunch(const Messaging::Message& message)
{
  std::string name = message.message.animation_update.animation;

  if (name == "punch" &&
    engine.Find<GameStateManager>()->CurrentState()->Manager().Find(player_).Find<Graphics::Sprite>()
    == message.message.animation_update.sprite)
  {
    frame = message.message.animation_update.frame;
  }
}

/**
 * \brief 
 */
void BulletBehavior::switchcursortodefualt()
{
  //switch to defualt recticle after hit
  Graphics::Window* w = engine.Find<Graphics::Manager>()->window();
  w->set_cursor("data\\img\\spr\\Reticle.cur");
}

/**
 * \brief
 */
void BulletBehavior::onCollision(const Messaging::Message& message)
{    
  if (this == nullptr)
    return;

      KPE::RigidBody* otherBody = nullptr;
      KPE::RigidBody* thisBody = nullptr;

      if (message.message.collision.body_ == GetParent().Find<KPE::RigidBody>()) {
        otherBody = message.message.collision.otherBody_;
        thisBody = message.message.collision.body_;
      }
      else {
        otherBody = message.message.collision.body_;
        thisBody = message.message.collision.otherBody_;
      }



      if (!collided)
      {

        if (otherBody->GetParent().GetType() == "Hole" ||
          otherBody->GetParent().GetType() == "Player" ||
          otherBody->GetParent().GetType() == "EnemyBullet")
        {
          *(message.message.collision.pass_) = false;
          return;
        }

        HealthComponent * h = otherBody->GetParent().Find<HealthComponent>();

        if (h != nullptr)
        {
          h->DoDamage(dmg_, _curr);
        }
        this->GetParent().Destroy();

        GameStateManager * gsm{ engine.Find<GameStateManager>() };
        GameObjectManager& objMan = gsm->CurrentState()->Manager();

        GameObject& deathsplosion = objMan.Add(
          ObjectFactory::ArchetypeGameObject(
            "data/json/ExplosionArchetype.json",
            thisBody->GetParent().Find<Transform>()->getPosition()
          ));

        KPE::ParticleSystem * particles = deathsplosion.Find<KPE::ParticleSystem>();
        if (particles != nullptr)
        {
          particles->setBlendColor(BulletColors[GetBullet()]);
        }

        KPE::ParticleEmitter * emitter = deathsplosion.Find<KPE::ParticleEmitter>();
        if (emitter != nullptr)
        {
          // 2D rotation matrix
          const glm::mat2x2 m  = 
          [&message]
          {
            const glm::vec2 & n = message.message.collision.normal_;
            const double rot = std::atan2(n.y, n.x);
            const double sin = std::sin(rot);
            const double cos = std::cos(rot);

            return glm::mat2x2{ {cos, sin}, {-sin, cos} };
          }();

          emitter->setPower        (m * emitter->getPower()        );
          emitter->setPowerVariance(m * emitter->getPowerVariance());
        }
      }
      



        switchcursortodefualt();

#if 0
      Audio::AudioManager *audio_manager = engine.Find<Audio::AudioManager>();

      int sound = audio_manager->PlayRandomSound({
        std::make_tuple("Hit_DeepMetalKick", 0, 0.3f),
        std::make_tuple("Hit_BodyImpactWall", 0, 0.03f),
        std::make_tuple("Crash_BodyHitMetalGate", 0, 0.3f) });

      switch (sound)
      {
      case 0:
        audio_manager->SetSoundPitch(2.5f, "Hit_DeepMetalKick");
        break;
      case 1:
        audio_manager->SetSoundPitch(2.5f, "Hit_BodyImpactWall");
        break;
      case 2:
        audio_manager->SetSoundPitch(1.5f, "Crash_BodyHitMetalGate");
        break;
      }
#endif
      collided = true;
}

/**
     * \brief 
     * \param player 
     */
    void BulletBehavior::AttachPlayer(ObjectId player)
    {
      player_ = player;
      ptransform = engine.Find<GameStateManager>()->CurrentState()->Manager().Find(player).Find<Transform>();
    }
