/*************************************************************************/
/*!
\file   CoinBehavior.cpp
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
#include "CoinBehavior.h"
#include "Engine/Engine.h"
#include "HealthComponent.h"
#include <Transform/Transform.hpp>
#include "Behaviors/Player1Behavior.hpp"
#include "Graphics/Window.hpp"

/**
 * \brief 
 */
void CoinBehavior::switchcursortodefualt()
{
    //switch to defualt recticle after hit
    Graphics::Window* w = engine.Find<Graphics::Manager>()->window();
    w->set_cursor("data\\img\\spr\\Reticle.cur");
}

/**
 * \brief 
 * \param parent 
 */
CoinBehavior::CoinBehavior(GameObject &parent) : Component(parent)
{
  b_lifetime = 1.5f;

  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();
  m_handler->Register(Messaging::ID::Collision, key, CoinBehavior::CollisionHandle);
}


/**
 * \brief 
 */
CoinBehavior::~CoinBehavior()
{
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetKey();
  //m_handler->Unregister(Messaging::ID::AnimationUpdate, key);
  m_handler->Unregister(Messaging::ID::Collision, key);
}

/**
 * \brief 
 */
void CoinBehavior::scattereffect()
{
}

/**
 * \brief 
 * \param dt 
 */
void CoinBehavior::Update(float dt)
{
  if (!GetParent().IsDestroyed())
  {
    /*
    b_lifetime -= dt;

    if (b_lifetime <= 0.0f)
    {
      GetParent().Destroy();
    }*/
    if (collided) {
      timer -= dt;
      if (timer <= 0.0f)
      {
        towardsplayer = true;
      }
      else {
        collided = false;
      }
    }
    if (towardsplayer)
    {
      trackplayer();
    }
    else if (_scatter)
    {
      scattereffect();
    }

  }
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void CoinBehavior::CollisionHandle(ObjectId obj,  const Messaging::Message& message)
{
  KPE::RigidBody* KeyBody = nullptr;

  if (message.message.collision.body_->GetParent().GetKey() == obj) {
    KeyBody = message.message.collision.body_;
  }
  else if (message.message.collision.otherBody_->GetParent().GetKey() == obj) {
    KeyBody = message.message.collision.otherBody_;
  }



  if (message.message.collision.otherBody_->GetParent().GetType() == message.message.collision.body_->GetParent().GetType()){
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
  object.Find<CoinBehavior>()->onCollision(message);
}

/**
 * \brief 
 */
void CoinBehavior::trackplayer()
{

  GameObjectManager& man = engine.Find<GameStateManager>()->CurrentState()->Manager();
  glm::vec2   dir = man.Find("Player1").Find<Transform>()->getPosition() - GetParent().Find<Transform>()->getPosition();
  dir = glm::normalize(dir);
  dir *= COINSPEED;

  //head towards player
  this->GetParent().Find<KPE::RigidBody>()->setVelocity(dir);
}

/**
 * \brief 
 * \param message 
 */
void CoinBehavior::onCollision(const Messaging::Message& message)
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
  
  switchcursortodefualt();
  
  *(message.message.collision.pass_) = false;

  if (collidedBody->GetParent().GetType() == "Hole")
  {
    *(message.message.collision.pass_) = false;
    return;
  }

  if (collidedBody->GetParent().GetType() == "Enemy") //|| collidedBody->GetParent().GetType() == "CoinDoor")
  {
    *(message.message.collision.pass_) = false;
    //collidedBody->GetParent().Find<HealthComponent>()->DoDamage(1, COINS);//enemy damage
    //GetParent().Destroy();
    //return;
    return;
  }
  

  if (collidedBody->GetParent().GetType() == "CoinDoor")
  {
    collidedBody->GetParent().Find<HealthComponent>()->DoDamage(1, COINS);//enemy damage
    
    GetParent().Destroy();
	//Have to search for sound that fits this
	engine.Find<Audio::AudioManager>()->PlaySound("Crash_BodyHitMetalGate");
    return;
  }

  if (collidedBody->GetParent().GetType() == "Platform")
  { //find player
    *(message.message.collision.pass_) = false;
    trackplayer();
    towardsplayer = true;
    return;
  }

  if (collidedBody->GetParent().GetType() == "Player")
  {
    
    *(message.message.collision.pass_) = false; //TODO: MAKE FEEDBACK LESS

    /*ADD  to PLAYER COIN COUNT*/
    collidedBody->GetParent().Find<Player1Behavior>()->AddCoin();
    GetParent().Destroy();
	engine.Find<Audio::AudioManager>()->PlaySound("menu_accept");
    return;
  }
    
    collided = true;
  }
