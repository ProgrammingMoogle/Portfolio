/******************************************************************************/
/*!
  \file   HoleBehavior.hpp
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

#include "HoleBehavior.hpp"
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
HoleBehavior::HoleBehavior( GameObject& parent ) : Component( parent ) {
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();
  m_handler->Register(Messaging::ID::Collision, key, HoleBehavior::CollisionHandler);
}

/**
 * \brief 
 */
HoleBehavior::~HoleBehavior() {
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();
  m_handler->Unregister( Messaging::ID::Collision, key );
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void HoleBehavior::CollisionHandler( ObjectId obj, const Messaging::Message & message ) {
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
  object.Find<HoleBehavior>()->onCollisionHandler(message);
}

/**
 * \brief 
 * \param dt 
 */
void HoleBehavior::Update( float dt ) {

}

/**
 * \brief 
 * \param message 
 */
void HoleBehavior::onCollisionHandler( const Messaging::Message & message) {
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
   
   


  if (collidedBody->GetParent().GetName() == "Player1")
  {
    GetParent().Find<KPE::RigidBody>()->setInverseMass(0);
  }
  else if (collidedBody->GetParent().GetName() == "Enemy")
  {
    GetParent().Find<KPE::RigidBody>()->setInverseMass(0);
  }
}

