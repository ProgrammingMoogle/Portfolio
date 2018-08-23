/*************************************************************************/
/*!
\file   GoalBehavior.cpp
\author Josh Ibrahim
\par
\par    DigiPen login: j.ibrahim
\par    Course: GAME 200
\par   Copyright � 2017 DigiPen (USA) Corp. and its owners. All Rights Reserved.
\date   9/28/2017
\brief
Functions include:
*/
/*************************************************************************/
#include "GoalBehavior.h"
#include <Messaging\Messaging.hpp>
#include "GameStateManager/GameStateManager.h"
#include "Engine/Engine.h"
#include "Graphics/ParticleAnimator.hpp"
#include "Physics/ParticleEmitter.hpp"


/**
 * \brief 
 * \param parent 
 * \param num 
 */
GoalBehavior::GoalBehavior(GameObject &parent, int num):Component(parent), roomnumber(num)
{
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();
  m_handler->Register(Messaging::ID::Collision, key, GoalBehavior::CollisionHandle);
}


/**
 * \brief 
 */
GoalBehavior::~GoalBehavior()
{
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void GoalBehavior::CollisionHandle(ObjectId obj, const Messaging::Message& message)
{
  /*GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);
  Player1Behavior* pb = object.Find<Player1Behavior>();
  pb->onCollisionHandle(message);*/

  KPE::RigidBody* KeyBody = nullptr;

  if (message.message.collision.body_->GetParent().GetKey() == obj) {
    KeyBody = message.message.collision.body_;
  }
  else if (message.message.collision.otherBody_->GetParent().GetKey() == obj) {
    KeyBody = message.message.collision.otherBody_;
  }



  if (message.message.collision.otherBody_->GetParent().GetType() == message.message.collision.body_->GetParent().GetType()) {
    //const_cast<Messaging::Message&>(message.message.collision.pass_ ) = false;
    *(message.message.collision.pass_) = false;
    return;
  }
  if (KeyBody == nullptr)
  {
    return;
  }


  //GameState* state = engine.Find<GameStateManager>()->CurrentState();
  //GameObjectManager& manager = state->Manager();
  GameObject& object = KeyBody->GetParent(); //manager.Find(obj);

                                             //if (object.GetKey() ==  obj)
                                             //Messaging::Message& temp = const_cast<Messaging::Message&>(message);
  object.Find<GoalBehavior>()->onCollisionHandle(message);
}

/**
 * \brief 
 * \param message 
 */
void GoalBehavior::onCollisionHandle(const Messaging::Message& message)
{
  KPE::RigidBody* collidedBody = nullptr;
  KPE::RigidBody* otherBody = nullptr;

  if (this == nullptr || message.message.collision.body_ == nullptr || message.message.collision.otherBody_ == nullptr) return;

  if (message.message.collision.body_ == GetParent().Find<KPE::RigidBody>()) {
    collidedBody = message.message.collision.otherBody_;
    otherBody = message.message.collision.body_;
  }
  else {
    collidedBody = message.message.collision.body_;
    otherBody = message.message.collision.otherBody_;
  }

  if (!startcountdown && collidedBody->GetParent().GetName() == "Player1")
  {
    (*message.message.collision.pass_) = false;
    
    ObjectFactory::Create("data/json/Player1Wins.json", engine.Find<GameStateManager>()->CurrentState()->Manager() );

    startcountdown = true;
    timer = delay;

    collidedBody->GetParent().DeActivate();//win can't move

    Graphics::Sprite * spr = GetParent().Find<Graphics::Sprite>();
    if (spr == nullptr)
      return;


    KPE::ParticleEmitter * emitter = GetParent().Find<KPE::ParticleEmitter>();
    if (emitter == nullptr)
      return;

    KPE::ParticleAnimator * animator = GetParent().Find<KPE::ParticleAnimator>();
    if (animator == nullptr)
      return;

    spr->setBlendColor(Graphics::Color{ 0,0,0,0 });
    spr->setAlpha(0);

    emitter->setRate(emitter->getRate() * 1.25f);
    emitter->setMaxCount(5);
    emitter->setPLifetime(emitter->getPLifetime());
    emitter->setWarmuptime(0);
    emitter->setParticleSize(500);
    animator->setGrowth(-1.75f * animator->getGrowth());
    emitter->Reset();

  }
}

/**
 * \brief 
 */
void GoalBehavior::change_level()
{
  engine.Find<GameStateManager>()->DelayedPopState();
  engine.Find<GameStateManager>()->DelayedPushState(roomnumber);
}

/**
 * \brief 
 * \param dt 
 */
void GoalBehavior::Update(float dt)
{
  if (GetParent().IsActive())
  {
    if (startcountdown) {
      timer -= dt;

      if (timer <= 0.0f)
      {

        change_level();
        
      }
    }
  }
}
