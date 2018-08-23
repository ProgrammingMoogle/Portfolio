/*************************************************************************/
/*!
\file   DelayedDestructionBehavior.cpp
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
#include "GameObject/Component.h"

/**
 * \brief 
 */
class DelayedDestruction : public Component
{
public:
  DelayedDestruction(GameObject& parent, float lifetime = 0);

  void setLifetime(float lifetime);

  virtual void Update(float dt);

  virtual Component* Clone_() const;

private:
  float lifetime_;
};