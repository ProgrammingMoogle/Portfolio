/*************************************************************************/
/*!
\file   RSwitchComponent.cpp
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
#include "RSwitchComponent.h"
#include "GameObject/GameObject.h"
#include <iostream>


/**
 * \brief 
 * \param parent 
 * \param room 
 * \param pos_ 
 */
RSwitchComponent::RSwitchComponent(GameObject& parent, int room, glm::vec2 pos_): Component(parent), room(room), pos(pos_)
{
  timer = delay;
}


/**
 * \brief 
 */
RSwitchComponent::~RSwitchComponent()
{
}

/**
 * \brief 
 * \return 
 */
Component* RSwitchComponent::Clone_() const
{
  return new RSwitchComponent(*this);
}

/**
 * \brief 
 * \param dt 
 */
void RSwitchComponent::Update(float dt)
{
  if (GetParent().IsActive() )
  {
    timer -= dt;

    if (timer <= 0.0)
    {
      timer = delay;
      std::cout << "room number :" << room << " " << "PlayePos" << pos.x << " " << pos.y << "\n";
    }
  }
}
