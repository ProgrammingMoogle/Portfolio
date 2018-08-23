/*************************************************************************/
/*!
\file   ColorFade.cpp
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
#include "Graphics/ColorFade.hpp"
/**
   * \brief 
   * \param start 
   * \param target 
   * \param targetT 
   */
  ColorFade::ColorFade(Graphics::Color const & start, Graphics::Color const & target, float targetT)
    :
    currentColor(start),
    targetColor(target),
    t(0.0f),
    target_t(targetT)
  {

  }

/**
   * \brief 
   * \return 
   */
  Graphics::Color ColorFade::interp() const
  {
    if (t >= 1.0f)
      return targetColor;

    return (1 - t) * currentColor + t * targetColor;
  }

/**
   * \brief 
   * \param current 
   * \param target 
   * \return 
   */
  Graphics::Color ColorFade::interp(Graphics::Color const & current, Graphics::Color const & target) const
  {
    if (t >= 1.0f)
      return target;

    return (1 - t) * current + t * target;
  }

/**
   * \brief 
   * \return 
   */
  constexpr float ColorFade::targetT() const
  {
    return target_t;
  }


/**
   * \brief 
   * \param dt 
   * \return 
   */
  bool ColorFade::fade(float dt)
  {
    t += dt / target_t;

    return t >= 1.0f;
  }

