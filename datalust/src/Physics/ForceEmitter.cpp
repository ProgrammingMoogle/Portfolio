/******************************************************************************/
  /*!
  \file   ForceEmitter.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ForceEmitter.hpp"
#include "Colliders.hpp"
#include "GameObject\GameObject.h"
#include "Engine\Engine.h"
#include <algorithm>

/**
 * \brief 
 */
namespace KPE {
  /****************************************************************************/
  //ForceEmitter
  /****************************************************************************/
  /**
   * \brief 
   * \param parent 
   */
  ForceEmitter::ForceEmitter(GameObject& parent)
  : Component(parent){
    force_ = ForceType::FGravity;
    forceGen_ = NULL;
  }

  /**
   * \brief 
   */
  ForceEmitter::~ForceEmitter(void) {
    //Delete the force
    delete forceGen_;
    //Remove all applied forces from this emitter
    clear();
  }

  /**
   * \brief 
   * \param dt 
   */
  void ForceEmitter::Update(float dt) {
    //Grab emitter's position
    glm::vec2 pos = GetParent().Find<Transform>()->getPosition();
    //Grab List of Colliders
    std::vector<Colliders*>* registry = Colliders::getCollidersRegistry();

    //Check if affected bodies are still in range
    checkBodies();

    //Check if any rigidbodies in range
    auto iter = registry->begin();
    for (; iter != registry->end(); ++iter) {
      Transform* trans = (*iter)->GetParent().Find<Transform>();

      //Find if the rigidbody is within radius
      if (inRange(trans, pos)) {
        RigidBody* body = (*iter)->GetParent().Find<RigidBody>();
        //Only apply forces to bodies of finite mass
        if (body->hasFiniteMass()) {
          //Check if already in list
          bool exists = false;
          auto iterAffected = affectedBodies_.begin();
          for (; iterAffected != affectedBodies_.end(); ++iterAffected) {
            if (body == (*iterAffected)) {
              exists = true;
              break;
            }
          }
          if (!exists) {
            applyForceTo(body, forceGen_);
          }
        }
      }
    }
  }


  /**
   * \brief 
   * \param force 
   * \param forceGen 
   */
  void ForceEmitter::setForce(ForceType force, ForceGenerator* forceGen) {
    if (forceGen_ != forceGen) {
      ForceType temp = force_;
      //Remove forces and reapply new forces
      auto iter = affectedBodies_.begin();
      for (; iter != affectedBodies_.end(); ++iter) {
        engine.GetPhysicsEngine()->forceRegistry_.remove(
          (*iter), forceGen_);
        applyForceTo((*iter), forceGen_);
      }
      delete forceGen_;
      force_ = force;
      forceGen_ = forceGen;
    }
  }

  /**
   * \brief 
   */
  void ForceEmitter::clear(void) {
    auto iter = affectedBodies_.begin();
    for (; iter != affectedBodies_.end(); ++iter) {
      engine.GetPhysicsEngine()->forceRegistry_.remove(
        (*iter), forceGen_);
    }
  }

  /**
   * \brief 
   * \param body 
   * \param forceGen 
   */
  void ForceEmitter::applyForceTo(RigidBody* body, ForceGenerator* forceGen) {
    forceGen->setEmitter(true);
    engine.GetPhysicsEngine()->forceRegistry_.add(body, forceGen);
    affectedBodies_.push_back(body);
  }

  /**
   * \brief 
   */
  void ForceEmitter::checkBodies(void) {
    //Grab emitter's position
    glm::vec2 pos = GetParent().Find<Transform>()->getPosition();

    affectedBodies_.erase(std::remove_if(affectedBodies_.begin(), affectedBodies_.end(), 
      [this, &pos](RigidBody*& body) {
        Transform* trans = body->GetParent().Find<Transform>();
        //Find if the rigidbody is not within radius
        if (!(this->inRange(trans, pos))) {
          engine.GetPhysicsEngine()->forceRegistry_.remove(body, forceGen_);
          return true;
        }
        return false;
      }), affectedBodies_.end());

  }

  /****************************************************************************/
  //ForceEmitterCircle
  /****************************************************************************/
  /**
   * \brief 
   * \param parent 
   */
  ForceEmitterCircle::ForceEmitterCircle(GameObject& parent) :
  ForceEmitter(parent){

  }

  /**
   * \brief 
   */
  ForceEmitterCircle::~ForceEmitterCircle(void) {

  }

  /**
   * \brief 
   * \return 
   */
  ForceEmitterCircle* ForceEmitterCircle::Clone_() const {
    return new ForceEmitterCircle(*this);
  }

  /**
   * \brief 
   * \param radius 
   */
  void ForceEmitterCircle::setRadius(float radius) {
    radius_ = radius;
  }

  /**
   * \brief 
   * \return 
   */
  float ForceEmitterCircle::getRadius(void) const {
    return radius_;
  }

  /**
   * \brief 
   * \param bodyTrans 
   * \param emitterPos 
   * \return 
   */
  bool ForceEmitterCircle::inRange(Transform* bodyTrans, glm::vec2& emitterPos) const {
    float length = glm::length(bodyTrans->getPosition() - emitterPos);
    if (length < radius_) {
      return true;
    }
    return false;
  }

  /****************************************************************************/
  //ForceEmitterBox
  /****************************************************************************/
  /**
   * \brief 
   * \param parent 
   */
  ForceEmitterBox::ForceEmitterBox(GameObject& parent)
  : ForceEmitter(parent){

  }

  /**
   * \brief 
   */
  ForceEmitterBox::~ForceEmitterBox(void) {

  }

  /**
   * \brief 
   * \return 
   */
  ForceEmitterBox* ForceEmitterBox::Clone_() const {
    return new ForceEmitterBox(*this);
  }


  //Accessors
  /**
   * \brief 
   * \param halfsize 
   */
  void ForceEmitterBox::setHalfsize(glm::vec2& halfsize) {
    halfsize_ = halfsize;
  }
  /**
   * \brief 
   * \return 
   */
  glm::vec2 ForceEmitterBox::getHalfsize(void) const {
    return halfsize_;
  }

  /**
   * \brief 
   * \param bodyTrans 
   * \param emitterPos 
   * \return 
   */
  bool ForceEmitterBox::inRange(Transform* bodyTrans, glm::vec2& emitterPos) const {
    //Check if within range
    glm::vec2 bodyPos = bodyTrans->getPosition();

    if (bodyPos.x < emitterPos.x + halfsize_.x &&
        bodyPos.x > emitterPos.x - halfsize_.x &&
        bodyPos.y < emitterPos.y + halfsize_.y &&
        bodyPos.y > emitterPos.y - halfsize_.y) {
      return true;
    }

    return false;
  }

}