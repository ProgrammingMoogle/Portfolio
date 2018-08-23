/*************************************************************************/
/*!
\file   EnemyBullet.cpp
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
#include "Engine/Engine.h"
#include "Messaging/Messaging.hpp"
#include "HealthComponent.h"
#include "EnemyBullet.hpp"
#include "Cheats\CheatOptions.hpp"	
#include "Player1Behavior.hpp"

/**
 * \brief 
 * \param parent 
 * \param _lifetime 
 */
EnemyBullet::EnemyBullet(GameObject & parent, float _lifetime) : Component{ parent },
  lifetime(_lifetime), collided{false}, firstUpdate{true}
{

}

/**
 * \brief 
 */
EnemyBullet::~EnemyBullet()
{
  auto && m = engine.Find<Messaging::MessageHandler>();
  auto key = GetParent().GetKey();
  m->Unregister(Messaging::ID::Collision, key);
}

/**
 * \brief 
 */
void EnemyBullet::Start()
{
  auto && m = engine.Find<Messaging::MessageHandler>();
  auto key = GetParent().GetKey();
  m->Register(Messaging::ID::Collision, key, HandleCollisions);
}

/**
 * \brief 
 * \param time 
 */
void EnemyBullet::setLifetime(float time)
{
  lifetime = time;
}

/**
 * \brief 
 * \param id 
 * \param m 
 */
void EnemyBullet::HandleCollisions(ObjectId id, Messaging::Message const & m)
{
  KPE::RigidBody *body = m.message.collision.body_;
  KPE::RigidBody *otherBody = m.message.collision.otherBody_;
  if (body == nullptr || otherBody == nullptr)
    return;

  if (otherBody->GetParent().GetKey() == id)
    otherBody->GetParent().Find<EnemyBullet>()->onCollisionEnter(*otherBody, *body, m);
  else if (body->GetParent().GetKey() == id)
    body->GetParent().Find<EnemyBullet>()->onCollisionEnter(*body, *otherBody, m);
}

/**
 * \brief 
 * \return 
 */
Component * EnemyBullet::Clone_() const
{
  return new  EnemyBullet(*this);
}

/**
 * \brief 
 * \param dt 
 */
void EnemyBullet::Update(float dt)
{
  if (!GetParent().IsActive())
    return;

  lifetime -= dt;
  if (lifetime <= 0)
    GetParent().Destroy();

  collided = false;
}

/**
 * \brief 
 * \param us 
 * \param them 
 * \param m 
 */
void EnemyBullet::onCollisionEnter(KPE::RigidBody& us, KPE::RigidBody& them, Messaging::Message const&m)
{
  *m.message.collision.pass_ = false;

  if (collided) return;
  collided = true;

  if (firstUpdate)
  {
    firstUpdate = false;
    return;
  }

  if (
    them.GetParent().GetType() == "Hole"        ||
    them.GetParent().GetType() == "Enemy"       ||
    them.GetParent().GetType() == "EnemyBullet" ||
    them.GetParent().GetType() == "Punch"
    )
  {
    return;
  }

  us.GetParent().Destroy();

  if (them.GetParent().GetType() != "Player")
    return;

  GameObject & player = them.GetParent();
  HealthComponent * health = player.Find<HealthComponent>();
  if (health == nullptr)
    return;

  if (!player.Find<Player1Behavior>()->godMode_)
  {
	  health->DoDamage(1, NONE);
  }
	
}
