/******************************************************************************/
/*!
  \file   Particle.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Particle.hpp"

namespace KPE {
  Particle::Particle(      
      glm::vec2 const& offset,
      glm::vec2 const& velocity,
      float rotation,
      float lifetime,
      float size
  )
  : 
    offset_(offset), 
    velocity_(velocity),
    rotation_(rotation),
    lifetime_(lifetime),
    size_(size)
  {
  }

  void Particle::update(float dt) {
    lifetime_ -= dt;

    offset_ += velocity_ * DYNAMIC_MULT * dt;
  }

}