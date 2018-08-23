/*************************************************************************/
/*!
\file    ColorFade.hpp
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
  struct ColorFade
  {
  public:
    ColorFade(Graphics::Color const& start, Graphics::Color const& target, float targetT);

    bool fade(float dt);
    Graphics::Color interp() const;

    Graphics::Color interp(Graphics::Color const& current, Graphics::Color const& target) const;

    Graphics::Color currentColor;
    Graphics::Color targetColor;

    constexpr float targetT() const;

  private:
    float t;
    float target_t;
  };
