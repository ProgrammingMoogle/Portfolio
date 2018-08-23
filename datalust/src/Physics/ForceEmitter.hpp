/******************************************************************************/
/*!
  \file   ForceEmitter.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef _FORCEEMITTER_HPP_
#define _FORCEEMITTER_HPP_

#include "Transform\Transform.hpp"
#include "ForceGenerator.hpp"
#include <vector>

/**
 * \brief 
 */
namespace KPE {

  /**
   * \brief 
   */
  class ForceEmitter : public Component {
  protected:
    ForceType force_;
    ForceGenerator* forceGen_;
    std::vector<RigidBody*> affectedBodies_;
  public: 
    ForceEmitter(GameObject& parent);
    virtual ~ForceEmitter(void);

    virtual ForceEmitter* Clone_() const = 0;

    void Update(float dt);

    void setForce(ForceType force, ForceGenerator* forceGen);

    void clear(void);

  protected:
    void checkBodies(void);
    void applyForceTo(RigidBody* body, ForceGenerator* forceGen);
    virtual bool inRange(
      Transform* bodyTrans, glm::vec2& emitterPos) const = 0;
  };


  //Emits a force in a circle
  class ForceEmitterCircle : public ForceEmitter {
    float radius_;
  public:
    ForceEmitterCircle(GameObject& parent);
    ~ForceEmitterCircle(void);

    ForceEmitterCircle* Clone_() const;

    //Accessors
    void setRadius(float radius);
    float getRadius(void) const;
  protected:
    bool inRange(
      Transform* bodyTrans, glm::vec2& emitterPos) const;
  };


  //Emits a force in a box
  class ForceEmitterBox : public ForceEmitter {
    glm::vec2 halfsize_;
  public:
    ForceEmitterBox(GameObject& parent);
    ~ForceEmitterBox(void);

    ForceEmitterBox* Clone_() const;

    //Accessors
    void setHalfsize(glm::vec2& halfsize);
    glm::vec2 getHalfsize(void) const;

  protected:
    bool inRange(
      Transform* bodyTrans, glm::vec2& emitterPos) const;
  };
}

#endif