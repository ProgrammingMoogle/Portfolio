/******************************************************************************/
/*!
  \file   ForceRegistry.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef _FORCEREGISTRY_HPP_
#define _FORCEREGISTRY_HPP_

#include "ForceGenerator.hpp"
#include <vector>

/**
 * \brief 
 */
namespace KPE {
  /**
   * \brief 
   */
  class ForceRegistry {
    struct ForceRegistration {
      RigidBody* body_;
      ForceGenerator* fGen_;
    };

    std::vector<ForceRegistration> registry_;
  public:
    void add(RigidBody* body, ForceGenerator* forceGen);
    void remove(RigidBody* body, ForceGenerator* forceGen);
    void removeByType(RigidBody* body, ForceType type, bool emitter);
    void clear(void);
    void updateForces(float dt);
    
    void parse(RigidBody* body,
      std::string type, glm::vec2 vec1, glm::vec2 vec2, float val1, float val2);
  };
}

#endif