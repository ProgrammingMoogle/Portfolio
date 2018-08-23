/******************************************************************************/
/*!
  \file   PhysicsConstants.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef _PHYSICSCONSTANTS_HPP_
#define _PHYSICSCONSTANTS_HPP_

#include <glm/glm.hpp>

/**
 * \brief 
 */
namespace KPE {
  //!Gravity Vector (y value is gravity)
  extern glm::vec2 GRAV_VEC;
  //!Damping should be near 1.0f
  extern float LINEAR_DAMP_BASE;

  extern float ANGULAR_DAMP_BASE;

  #define SINGLE_TICK (1.0f / 60.0f)

  //Dynamics Multiplier: Default multiplier
  extern float DYNAMIC_MULT;

  enum BoundType {
    BoundBox,
    BoundHitBox,
    BoundHurtBox
  };

  enum ForceType {
    FGravity,
    FHorDrag,
    FOmniDrag,
    FAnchorSpring,
    FDirection,
    FProximity
  };

}

#endif