/*************************************************************************/
/*!
\file   RswitchComponent.cpp
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
#include <glm/detail/type_vec2.hpp>

/**
 * \brief 
 */
class RSwitchComponent :
  public Component
{
  int room;
  glm::vec2 pos;
  float timer = 0.0f;
    float delay = 3.0f;
public:
  RSwitchComponent(GameObject& parent, int room, glm::vec2 pos_);
  ~RSwitchComponent();
  //! clone function MUST BE IMPLEMENTED BY ALL CLASSES (recomended: implement using a copy ctor)
  virtual Component* Clone_() const;
  int GetRoom() const { return room; }
  glm::vec2  GetPlayerPos() const { return pos; };
  virtual void Update(float dt);
};

