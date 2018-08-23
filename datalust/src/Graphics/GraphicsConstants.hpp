/*************************************************************************/
/*!
\file   GraphicsConstants.cpp
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
/**
 * \brief 
 */
namespace Graphics
{
  /**
   * \brief 
   */
  constexpr float ASPECT = 16.0f / 9.0f;                                               // 16 by 9
  /**
   * \brief 
   */
  constexpr unsigned INTERNAL_HEIGHT  = 1080;                                          // 1080p
  /**
   * \brief 
   */
  constexpr unsigned INTERNAL_WIDTH = static_cast<unsigned>(INTERNAL_HEIGHT * ASPECT); // 1920

  /**
   * \brief 
   */
  constexpr float circle_uvs_[] = {
    -1.0f,   1.0f,
    1.0f,   1.0f,
    1.0f,  -1.0f,
    -1.0f,  -1.0f,
  };
}