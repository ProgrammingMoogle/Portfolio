/*************************************************************************/
/*!
\file   EnemyDoorBehavior.hpp
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
#include "GameObject\Component.h"
#include "GameObject/GameObjectManager.h"

/**
 * \brief 
 */
class EnemyDoorBehavior :
  public Component
{

  GameObjectManager& manager_;
public:
  EnemyDoorBehavior(GameObject &parent);
  ~EnemyDoorBehavior();
  static void DeleteDoor(ObjectId obj, const Messaging::Message& message);
  void ondelete();
  virtual Component* Clone_() const { return new EnemyDoorBehavior(*this); }
  virtual void Update(float dt);
};

