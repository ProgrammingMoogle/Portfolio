/*************************************************************************/
/*!
\file   EnemyBullet.h
\author Studio200
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
#include "GameObject/Component.h"

// forward decl
/**
 * \brief 
 */
namespace Messaging
{
  struct Message;
}

/**
 * \brief 
 */
namespace KPE
{
  class RigidBody;
}

/**
 * \brief 
 */
class EnemyBullet : public Component
{
public:
  EnemyBullet(GameObject & parent, float lifetime = 1.5f);
  virtual ~EnemyBullet();

  void setLifetime(float);
  
  void Update(float dt);

  static void HandleCollisions(ObjectId id, Messaging::Message const& m);

  virtual Component * Clone_() const;

private:
  void onCollisionEnter(KPE::RigidBody& us, KPE::RigidBody& them, Messaging::Message const&m);
  void Start();
  bool firstUpdate = true;
  bool collided;
  float lifetime;
};