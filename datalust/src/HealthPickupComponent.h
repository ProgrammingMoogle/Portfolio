/*************************************************************************/
/*!
\file   HealthPickupComponent.h
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

/**
 * \brief 
 */
class HealthPickupComponent :
  public Component
{
  int amt;
public:
  HealthPickupComponent(GameObject& parent, int _amt);
  ~HealthPickupComponent();

  int GetAmt() const { return amt; }

  virtual Component* Clone_() const;
};

