/*************************************************************************/
/*!
\file   FadeOut.hpp
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
#include "Graphics/ColorFade.hpp"

/**
 * \brief 
 */
class FadeOut : public Component
{
public:
  FadeOut(GameObject& parent, float interval = 0);

  virtual void setInterval(float interval);

  virtual void setColor(Graphics::Color const& c);
  virtual void setColor(Graphics::Color && c);

  virtual void Update(float dt);

  virtual Component* Clone_() const;

protected:
  ColorFade fade;
};