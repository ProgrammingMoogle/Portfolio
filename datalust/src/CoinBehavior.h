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
#pragma once
#include "Behaviors/Behaviors.hpp"
#include "BulletType.h"

const float COINLIFETIME  = 1.5f;
const float COINSPEED =  3.2f;

/**
 * \brief 
 */
class CoinBehavior :
  public Component
{
  float b_lifetime;
  BulletType _curr;
  bool towardsplayer = false;
  bool _scatter = false;
  float delay = 0.03f;
  float timer = delay;
  bool collided = false;
  void scattereffect();
  void switchcursortodefualt();
  void trackplayer();
public:
  CoinBehavior(GameObject &parent);
  ~CoinBehavior();
  virtual Component* Clone_() const { return new CoinBehavior(*this); }
  virtual void Update(float dt);
  static void CollisionHandle(ObjectId, const Messaging::Message&);
  void onCollision( const Messaging::Message& message);
  void SetBullet(BulletType color) { _curr = color; }
  BulletType GetBullet() { return _curr; }
  void scatter() {towardsplayer = true; }
};
