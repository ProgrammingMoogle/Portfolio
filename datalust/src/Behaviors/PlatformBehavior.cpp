/******************************************************************************/
/*!
  \file   PlatformBehavior.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief  Behavior component for semi-solid platforms
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#define _USE_MATH_DEFINES
#include <cmath>

#include "PlatformBehavior.hpp"
#include <GameObject/GameObject.h>
#include <Graphics/Graphics.hpp>
#include <Engine/Engine.h>
#include <Transform/Transform.hpp>
#include <GameObject/GameObjectManager.h>
#include <Behaviors/BulletBehavior.h>

/**
 * \brief 
 * \param parent 
 */
PlatformBehavior::PlatformBehavior( GameObject& parent ) : Component( parent ) {
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();
  m_handler->Register(Messaging::ID::Collision, key, PlatformBehavior::CollisionHandler);
}

/**
 * \brief 
 */
PlatformBehavior::~PlatformBehavior() {
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();
  m_handler->Unregister( Messaging::ID::Collision, key );
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void PlatformBehavior::CollisionHandler( ObjectId obj, const Messaging::Message & message ) {
  KPE::RigidBody* KeyBody = nullptr;

  if (message.message.collision.body_->GetParent().GetKey() == obj) {
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
  object.Find<PlatformBehavior>()->onCollisionHandler(message);
}

/**
 * \brief 
 * \param dt 
 */
void PlatformBehavior::Update( float dt ) {
  Graphics::Sprite *spr = GetParent().Find<Graphics::Sprite>();
  if (spr != nullptr)
  {
    Graphics::Color color{};
    spr->getBlendColor(&color);
    spr->setBlendColor((1.0f - (0.25f * dt)) * color + (0.25f * dt) * Graphics::Color{ 0.5f, 0.5f, 0.5f, 1.0f });
  }
#if 0
  GameObject* player_ = nullptr;
  try {
    player_ = &engine.Find<GameStateManager>()->CurrentState()->Manager().Find( "Player1" );
  } catch ( ... ) { return; }
  if ( !player_ ) { return; }

  GameObject& player = *player_;
  GameObject& platform = GetParent();
  float playerBottom = player.Find<Transform>()->getPosition().y - player.Find<KPE::Colliders>()->getBottom();
  float platformTop = platform.Find<Transform>()->getPosition().y + platform.Find<KPE::Colliders>()->getTop();
  
  if(playerBottom < platformTop)
    GetParent().Find<Graphics::Sprite>()->setBlendColor( { 1, 1, 0, 0.25 } );
  else
    GetParent().Find<Graphics::Sprite>()->setBlendColor( { 1, 1, 0, 0.5 } );
#endif
}

/**
 * \brief 
 * \param message 
 */
void PlatformBehavior::onCollisionHandler( const Messaging::Message & message)
{
#if 0
  if ( message.message.collision.otherBody_ == GetParent().Find<KPE::RigidBody>() ) {
    if ( message.message.collision.body_->GetParent().GetType() == "Player" ) {
      GameObject& player = message.message.collision.body_->GetParent();
      GameObject& platform = GetParent();
      float playerBottom = player.Find<Transform>()->getPosition().y - player.Find<KPE::Colliders>()->getBottom();
      float platformTop = platform.Find<Transform>()->getPosition().y + platform.Find<KPE::Colliders>()->getTop();

      *(message.message.collision.pass_) = true;

      /*if(playerBottom < platformTop)
        *( message.message.collision.pass_ ) = false;*/
    }
  }
#endif

  if (message.message.collision.otherBody_ == GetParent().Find<KPE::RigidBody>()
      && GetParent().GetType() != "CoinDoor")
  {
    BulletBehavior *b = message.message.collision.body_->GetParent().Find<BulletBehavior>();
    if (b == nullptr)
      return;

    Graphics::Sprite *spr = message.message.collision.otherBody_->GetParent().Find<Graphics::Sprite>();
    if (spr == nullptr)
      return;

    Graphics::Color color;
    spr->getBlendColor(&color);
    spr->setBlendColor(0.7f * color + 0.3f * BulletColors[b->GetBullet()]);
	engine.Find<Audio::AudioManager>()->PlaySound("Hit_BodyImpactWall");
  }
}

