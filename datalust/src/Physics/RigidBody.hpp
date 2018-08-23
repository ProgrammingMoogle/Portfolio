#pragma once
/******************************************************************************/
/*!
  \file   RigidBody.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
    
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef _RIGIDBODY_HPP_
#define _RIGIDBODY_HPP_

#include <GameObject/Component.h>
#include "PhysicsConstants.hpp"

/**
 * \brief 
 */
namespace KPE {

  /**
   * \brief 
   */
  class RigidBody : public Component {
    //Dynamic Multiplier
    float dMultiplier_;
    
    //InverseMass < 0 is infinite 0 is float max
    float inverseMass_;

    //Damping
    float linearDamping_;
    float angularDamping_;

    //Velocity
    glm::vec2 velocity_;
    float rotation_;

    //Force Accumulators
    bool forcesActive_;
    glm::vec2 forceAccumulator_;
    float torqueAccumulator_;

    //Acceleration
    glm::vec2 acceleration_;
    glm::vec2 lastFrameAcceleration_;

    //Restitution (Bounciness) [0-1.0]
    float restitution_;
  public:
    RigidBody(GameObject& parent);

    //**Methods**//
    void Update(float dt);

      //Forces
    bool forcesAreActive(void);
    void forcesActivate(void);
    void forcesDeactivate(void);

    void clearAccumulators(void);
    void addForce(const glm::vec2& force);

    //**Accessors**//
      //Mass
    void setMass(float mass);
    void setInverseMass(float inverseMass);
    float getMass(void) const;
    float getInverseMass(void) const;
    bool hasFiniteMass(void) const;

      //Damping
    void setLinearDamping(float linearDamping);
    void setAngularDamping(float angularDamping);
    float getLinearDamping(void) const;
    float getAngularDamping(void) const;

      //Velocity
    void setVelocity(float xVelocity, float yVelocity);
    void setVelocity(glm::vec2& velocity);
    glm::vec2 getVelocity(void) const;
    void addVelocity(const glm::vec2& velocity);

      //Angular Velocity
    void setRotation(float rotation);
    float getRotation(void) const;

      //Acceleration
    void setAcceleration(glm::vec2& acceleration);
    glm::vec2 getAcceleration(void) const;
    glm::vec2 getLastAcceleration(void) const;

      //Restitution
    void setRestitution(float restitution);
    float getRestitution(void) const;

    //Component Virtual
    //Component_::TypeEnum GetComponentType() const;

  private:
    //Component Virtual
    Component* Clone_() const;
  };
} //namespace KPE
#endif