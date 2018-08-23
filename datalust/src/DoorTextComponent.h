/*************************************************************************/
/*!
\file   DoorTextComponent.h
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

//#include "Graphics/TextSprite.hpp"
//#include "Graphics/TextSprite.hpp"

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
class DoorTextComponent :
  public Component
{

public:
  DoorTextComponent(GameObject& parent);
  ~DoorTextComponent();
  virtual Component* Clone_() const { return new DoorTextComponent(*this);  }
  virtual void Start();

  static void HandleDamage(ObjectId, Messaging::Message const&);

private:
  void UpdateHealth();
};

