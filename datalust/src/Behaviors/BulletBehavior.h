/*************************************************************************/
/*!
\file   BulletBehavior.h
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
#include <GameObject\Component.h>
#include <Messaging\Messaging.hpp>
#include "HealthComponent.h"

#define BULLETLIFETIME 100.5f
#define BULLETSPEED 3.0f
/**
 * \brief 
 */
class BulletBehavior :
  public Component
{
public:
  BulletBehavior(GameObject &parent, int dmg, float scalar, unsigned int c);
  ~BulletBehavior();
  void Update(float dt);
  virtual Component* Clone_() const { return new BulletBehavior(*this); }
  void AttachPlayer(ObjectId player);
  static void Punch(ObjectId, const Messaging::Message&);
  static void Collision(ObjectId, const Messaging::Message&);
  void SetBullet(BulletType color) { _curr = color; }
  void SetLifeTime(float);
  BulletType GetBullet() { return _curr; }
  void SetDMG(int dmg) { dmg_ = dmg; }
private:
  ObjectId player_ = nullobj;
  Transform* ptransform;
  int dmg_;
  float scalar_;
  int frame = 0;
  float b_lifetime;
  unsigned _color;
  void onPunch(const Messaging::Message&);
  void switchcursortodefualt();
  void onCollision(const Messaging::Message&);
  bool collided = false;
  BulletType _curr;
};

