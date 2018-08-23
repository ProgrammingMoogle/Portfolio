/*************************************************************************/
/*!
\file   DelayedDestructionBehavior.h
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
#include "GameObject/GameObject.h"
#include "DelayedDestructionBehavior.hpp"

/**
 * \brief 
 * \param parent 
 * \param lifetime 
 */
DelayedDestruction::DelayedDestruction(GameObject & parent, float lifetime) :
  Component(parent),
  lifetime_(lifetime)
{
}

/**
 * \brief 
 * \param lifetime 
 */
void DelayedDestruction::setLifetime(float lifetime)
{
  lifetime_ = lifetime;
}

/**
 * \brief 
 * \param dt 
 */
void DelayedDestruction::Update(float dt)
{
  if (GetParent().IsActive() == false) {
    return;
  }

    if ((lifetime_ -= dt) <= 0)
    {
      GetParent().Destroy();
    }
}

/**
 * \brief 
 * \return 
 */
Component * DelayedDestruction::Clone_() const
{
  return new DelayedDestruction(*this);
}
