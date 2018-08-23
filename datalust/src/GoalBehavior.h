/*************************************************************************/
/*!
\file   GoalBehavior.h
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
#pragma once
#include "GameObject/Component.h"
#include <Messaging\Messaging.hpp>
/**
 * \brief 
 */
class GoalBehavior :
  public Component
{
  int roomnumber;
  float timer = 0.0f;
  float delay = 5.0f;
  bool startcountdown = false;
public:
  GoalBehavior(GameObject &parent, int num);
  ~GoalBehavior();
  static void CollisionHandle(ObjectId, const Messaging::Message&);
  void onCollisionHandle(const Messaging::Message&);
  virtual Component* Clone_() const { return new GoalBehavior(*this); }
  void change_level();
  void Update(float dt);
};

