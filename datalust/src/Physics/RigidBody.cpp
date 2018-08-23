/******************************************************************************/
/*!
  \file   RigidBody.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "RigidBody.hpp"

#include <Transform/Transform.hpp>
#include <GameObject/GameObject.h>
#include <assert.h>
#include <float.h>

/**
 * \brief 
 */
namespace KPE {
  
  /****************************************************************************/
  /*!
    \brief
      Main Contructor for RigidBody
    \param parent
      Parent object
  */
  /****************************************************************************/
  RigidBody::RigidBody(GameObject& parent) 
  : Component(parent), velocity_(0), forceAccumulator_(0), acceleration_(0){
    //Dynamics Multiplier
    dMultiplier_ = DYNAMIC_MULT;

    //Default Infinite Mass
    inverseMass_ = 0;
    //Default Damping
    linearDamping_ = LINEAR_DAMP_BASE;
    angularDamping_ = ANGULAR_DAMP_BASE;

    rotation_ = 0;
    torqueAccumulator_ = 0;

    forcesActive_ = false;

    //Default Restitution
    restitution_ = 0.0f;
  }

  //**Methods**//
  /****************************************************************************/
  /*!
    \brief
      Update for RigidBody component
    \param dt
      Time passed
  */
  /****************************************************************************/
  void RigidBody::Update(float dt) {
    //Update Acceleration (Apply Forces)
    lastFrameAcceleration_ = acceleration_;
   /*
    if (forcesActive_)
      lastFrameAcceleration_ += 
        forceAccumulator_ * inverseMass_ * sqrt(dMultiplier_);
      */

    //Update velocities
    velocity_ += lastFrameAcceleration_ * dt;
    //Rotational acceleration

    //Apply Damping
    velocity_ *= pow(linearDamping_, dt);
    rotation_ *= pow(angularDamping_, dt);

    //Hard cap velocity
    if (velocity_.length() > 50.0f) {
      normalize(velocity_);
      velocity_ *= 50.0f;
    }

    //Apply to Transform
    Transform* transform = GetParent().Find<Transform>();
    //Update Position
    transform->setPosition(transform->getPosition() + (velocity_ * dt * dMultiplier_));
    //Update Orientation
    transform->setOrientation(transform->getOrientation() + (rotation_ * dt * dMultiplier_));

    //Cleanup
    clearAccumulators();
  }

    //Forces
  /**
   * \brief 
   * \return 
   */
  bool RigidBody::forcesAreActive(void) {
    return forcesActive_;
  }
  /**
   * \brief 
   */
  void RigidBody::forcesActivate(void) {
    forcesActive_ = true;
  }
  /**
   * \brief 
   */
  void RigidBody::forcesDeactivate(void) {
    forcesActive_ = false;
  }

  /****************************************************************************/
  /*!
    \brief
      Clears accumulated forces
  */
  /****************************************************************************/
  void RigidBody::clearAccumulators(void) {
    forceAccumulator_.x = 0;
    forceAccumulator_.y = 0;
  }
  /****************************************************************************/
  /*!
    \brief
      Adds a force to body
    \param force
      Force to add to force accumulator
  */
  /****************************************************************************/
  void RigidBody::addForce(const glm::vec2& force) {
    forceAccumulator_.x += force.x;
    forceAccumulator_.y += force.y;
  }

  //**Accessors**//
    //Mass
  /**
   * \brief 
   * \param mass 
   */
  void RigidBody::setMass(float mass) {
    if (mass < 0.00001f)
      inverseMass_ = 0.0f;
    else
      inverseMass_ = 1.0f / mass;
  }
  /**
   * \brief 
   * \param inverseMass 
   */
  void RigidBody::setInverseMass(float inverseMass) {
    inverseMass_ = inverseMass;
  }
  /**
   * \brief 
   * \return 
   */
  float RigidBody::getMass(void) const {
    return inverseMass_ / 1.0f;
  }
  /**
   * \brief 
   * \return 
   */
  float RigidBody::getInverseMass(void) const {
    return inverseMass_;
  }
  /**
   * \brief 
   * \return 
   */
  bool RigidBody::hasFiniteMass(void) const {
    if (inverseMass_ > 0)
      return true;
    return false;
  }

    //Damping
  /**
   * \brief 
   * \param linearDamping 
   */
  void RigidBody::setLinearDamping(float linearDamping) {
    linearDamping_ = linearDamping;
  }
  /**
   * \brief 
   * \param angularDamping 
   */
  void RigidBody::setAngularDamping(float angularDamping) {
    angularDamping_ = angularDamping;
  }
  /**
   * \brief 
   * \return 
   */
  float RigidBody::getLinearDamping(void) const {
    return linearDamping_;
  }
  /**
   * \brief 
   * \return 
   */
  float RigidBody::getAngularDamping(void) const {
    return angularDamping_;
  }

    //Velocity
  /**
   * \brief 
   * \param xVelocity 
   * \param yVelocity 
   */
  void RigidBody::setVelocity(float xVelocity, float yVelocity) {
    velocity_.x = xVelocity;
    velocity_.y = yVelocity;
  }
  /**
   * \brief 
   * \param velocity 
   */
  void RigidBody::setVelocity(glm::vec2& velocity) {
    velocity_ = velocity;
  }
  /**
   * \brief 
   * \return 
   */
  glm::vec2 RigidBody::getVelocity(void) const {
    return velocity_;
  }
  /**
   * \brief 
   * \param velocity 
   */
  void RigidBody::addVelocity(const glm::vec2& velocity) {
    velocity_ += velocity;
  }

    //Angular Velocity
  /**
   * \brief 
   * \param rotation 
   */
  void RigidBody::setRotation(float rotation) {
    rotation_ = rotation;
  }
  /**
   * \brief 
   * \return 
   */
  float RigidBody::getRotation(void) const {
    return rotation_;
  }
  
  //Acceleration
  /**
   * \brief 
   * \param acceleration 
   */
  void RigidBody::setAcceleration(glm::vec2& acceleration) {
    acceleration_ = acceleration;
  }

  /**
   * \brief 
   * \return 
   */
  glm::vec2 RigidBody::getAcceleration(void) const {
    return acceleration_;
  }

  /**
   * \brief 
   * \return 
   */
  glm::vec2 RigidBody::getLastAcceleration(void) const {
    return lastFrameAcceleration_;
  }

  //Restitution
  /**
   * \brief 
   * \param restitution 
   */
  void RigidBody::setRestitution(float restitution) {
    if (restitution > 1.0f)
      restitution_ = 1.0f;
    else if (restitution < 0.0f)
      restitution_ = 0.0f;
    else 
      restitution_ = restitution;
  }

  /**
   * \brief 
   * \return 
   */
  float RigidBody::getRestitution(void) const {
    return restitution_;
  }

  //Component Virtual
  /**
   * \brief 
   * \return 
   */
  Component* RigidBody::Clone_() const {
    return new RigidBody(*this);
  }
} //namespace KPE