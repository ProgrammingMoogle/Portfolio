/*************************************************************************/
/*!
\file   EnemyBehavior.h
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
#include <glm/vec2.hpp>
#include "GameObject/Component.h"

/**
 * \brief 
 */
namespace Messaging
{
  struct Message;
};

/**
 * \brief 
 */
class EnemyBehavior : public Component
{
public:
  EnemyBehavior(
    GameObject& parent,
    float _movespeed = 10.0f,
    bool _canShoot = true,
    bool _shootplayer = false,
    glm::vec2 _shootDir = glm::vec2{-1, 0},
    float bulletSpeed = 1.5f,
    float shootDelay = 1.0f
  );

  virtual void Update(float dt);

  void setCanShoot(bool _shoots);
  
  void setShootsPlayer(bool _targetPlayer);

  void setShootDir(glm::vec2 const& _dir);
  void setShootDir(glm::vec2 && _dir);
  void setBulletSpeed(float _bulletSpeed);

  void setMoveSpeed(float _movespeed);

  template<typename T>
  static void OnCollision(ObjectId obj, Messaging::Message const& m);

protected:
  virtual void OnCollisionEnter(KPE::RigidBody& us, KPE::RigidBody & them, Messaging::Collision const& collision);
  glm::vec2 const& getPlayerDir() const;

protected:
  float movespeed;
  
  bool canShoot;
  bool shootPlayer;
  
  glm::vec2 shootDir;
  float bulletSpeed;

  float delay;
  float timer;
};

#include "EnemyBehavior.inl"