/*************************************************************************/
/*!
\file   BulletType.h
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
#include "Graphics/GraphicsTypes.hpp"

/**
 * \brief 
 */
enum BulletType {
  BLUE = 0,
  RED = 1,
  COINS,
  NONE,
  COUNT
};

/**
 * \brief 
 */
static const Graphics::Color BulletColors[COUNT] = 
{
  {0,0,1,1},
  {1,0,0,1},
  {1,1,0,1},
  {0.5f,0.5f,0.5f,0.5f}
};