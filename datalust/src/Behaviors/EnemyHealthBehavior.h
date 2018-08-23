/*************************************************************************/
/*!
\file   EnemyHealthBehavior.h
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
#include <GameObject\Component.h>
#include <Messaging\Messaging.hpp>
#include <BulletType.h>

/**
 * \brief 
 */
class EnemyHealthBehavior : public Component{
private:
  ObjectId enemyID_;
  int totalHP_;
  BulletType color_;
  void onCollision(const Messaging::Message&);
public:
  EnemyHealthBehavior(GameObject &parent, ObjectId id, int totalHP = 2);
  ~EnemyHealthBehavior();
  void Update(float dt);
  virtual Component* Clone_() const { return new EnemyHealthBehavior(*this); }
  static void CollisionHandler(ObjectId, const Messaging::Message&);
};