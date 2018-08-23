/******************************************************************************/
/*!
  \file   Physics.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef _PHYSICS_HPP_
#define _PHYSICS_HPP_

#include "ForceRegistry.hpp"
#include "CollisionDetect.hpp"
#include "ContactResolver.hpp"

/**
 * \brief 
 */
namespace KPE {

  class PhysicsEngine {
    bool paused_ = false;
    CollisionDetector narrowDetection_;
    ContactResolver contactResolver_;
  public:
    ForceRegistry forceRegistry_;
    void init();
    void update(float dt);
    void shutdown();
    void togglePause();
  };
}

#endif