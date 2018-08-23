/******************************************************************************/
/*!
  \file   Particle.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
    
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef _PARTICLE_HPP_
#define _PARTICLE_HPP_

#include "PhysicsConstants.hpp"
#include "Graphics\ParticleGraphic.hpp"

namespace KPE{
  struct Particle {
    // no reason to keep these private
    glm::vec2 velocity_;
    glm::vec2 offset_;

    float rotation_;

    float lifetime_;

    float size_;

    Particle(
      glm::vec2 const& offset,
      glm::vec2 const& velocity,
      float rotation,
      float lifetime,
      float size
    );

    void update(float dt);
  };
}

#endif