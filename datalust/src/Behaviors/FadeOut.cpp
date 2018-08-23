/*************************************************************************/
/*!
\file   FadeOut.cpp
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
#include "Graphics/GraphicsTypes.hpp"
#include "Graphics/Sprite.hpp"
#include "Behaviors/FadeOut.hpp"

/**
 * \brief 
 * \param parent 
 * \param interval 
 */
FadeOut::FadeOut(GameObject& parent, float interval) :
  Component{ parent },
  fade{ Graphics::Color{}, Graphics::Color{}, interval } 
{}

/**
 * \brief 
 * \param interval 
 */
void FadeOut::setInterval(float interval)
{
  fade = ColorFade{ std::move(fade.currentColor), std::move(fade.targetColor), std::move(interval) };
}

/**
 * \brief 
 * \param c 
 */
void FadeOut::setColor(Graphics::Color const& c)
{
  fade.currentColor = c;
}

/**
 * \brief 
 * \param c 
 */
void FadeOut::setColor(Graphics::Color && c)
{
  fade.currentColor = std::move(c);
}

/**
 * \brief 
 * \param dt 
 */
void FadeOut::Update(float dt)
{
  fade.fade(dt);
  GetParent().Find<Graphics::Sprite>()->setBlendColor(fade.interp());

}

/**
 * \brief 
 * \return 
 */
Component* FadeOut::Clone_() const
{
  return new FadeOut(*this);
}