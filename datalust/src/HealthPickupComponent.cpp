/*************************************************************************/
/*!
\file   HealthPickupComponent.cpp
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
#include "HealthPickupComponent.h"

/**
 * \brief 
 * \param parent 
 * \param _amt 
 */
HealthPickupComponent::HealthPickupComponent(GameObject& parent, int _amt): Component(parent), amt(_amt)
{
}


/**
 * \brief 
 */
HealthPickupComponent::~HealthPickupComponent()
{
}

/**
 * \brief 
 * \return 
 */
Component* HealthPickupComponent::Clone_() const
{
  return new HealthPickupComponent(*this);
}
