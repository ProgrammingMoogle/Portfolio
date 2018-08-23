/******************************************************************************/
/*!
  \file   ForceGenerator.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ForceGenerator.hpp"
#include "GameObject/GameObject.h"
#include "Transform\Transform.hpp"
#include "ForceEmitter.hpp"

/**
 * \brief 
 */
namespace KPE {
  /****************************************************************************/
  //ForceGenerator
  /****************************************************************************/
  /**
   * \brief 
   */
  ForceGenerator::ForceGenerator(void) {
    is_emitter_ = false;
  }

  /**
   * \brief 
   * \param is_emitter 
   */
  void ForceGenerator::setEmitter(bool is_emitter) {
    is_emitter_ = is_emitter;
  }

  /**
   * \brief 
   * \return 
   */
  bool ForceGenerator::isEmitterForce(void) const {
    return is_emitter_;
  }

  /****************************************************************************/
  //ForceGravity
  /****************************************************************************/
  /**
   * \brief 
   */
  ForceGravity::ForceGravity(void) 
  : gravity_(0,0) {

  }

  /**
   * \brief 
   * \return 
   */
  ForceGenerator* ForceGravity::clone(void) {
    return new ForceGravity(*this);
  }

  /**
   * \brief 
   * \param rhs 
   * \return 
   */
  bool ForceGravity::operator==(const ForceGenerator* rhs) const {
    const ForceGravity* temp = static_cast<const ForceGravity*>(rhs);
    if (gravity_ == temp->gravity_)
      return true;
    return false;
  }

  /**
   * \brief 
   * \param body 
   * \param dt 
   */
  void ForceGravity::updateForce(RigidBody* body, float dt) {
    //Don't handle infinite mass
    if (!(body->hasFiniteMass()))
      return;

    //Add force to body
    body->addForce(gravity_);
  }

  /**
   * \brief 
   * \return 
   */
  ForceType ForceGravity::getType(void) const{
    return FGravity;
  }

  /****************************************************************************/
  //ForceHorizontalDrag
  /****************************************************************************/
  /**
   * \brief 
   * \param k1 
   * \param k2 
   */
  ForceHorizontalDrag::ForceHorizontalDrag(float k1, float k2){
    k1_ = k1;
    k2_ = k2;
  }

  /**
   * \brief 
   * \return 
   */
  ForceGenerator* ForceHorizontalDrag::clone(void) {
    return new ForceHorizontalDrag(*this);
  }

  /**
   * \brief 
   * \param rhs 
   * \return 
   */
  bool ForceHorizontalDrag::operator==(const ForceGenerator* rhs) const {
    const ForceHorizontalDrag* temp = 
      static_cast<const ForceHorizontalDrag*>(rhs);
    if (k1_ == temp->k1_)
      if (k2_ == temp->k2_)
        return true;
    return false;
  }

  /**
   * \brief 
   * \param body 
   * \param dt 
   */
  void ForceHorizontalDrag::updateForce(RigidBody* body, float dt) {
    glm::vec2 force(0);
    glm::vec2 velocity(0);

    velocity = body->getVelocity();
    velocity.x *= pow(k1_, dt);

    body->setVelocity(velocity);
  }

  /**
   * \brief 
   * \return 
   */
  ForceType ForceHorizontalDrag::getType(void) const {
    return ForceType::FHorDrag;
  }

  /****************************************************************************/
  //ForceOmniDrag
  /****************************************************************************/
  /**
   * \brief 
   * \param k1 
   * \param k2 
   */
  ForceOmniDrag::ForceOmniDrag(float k1, float k2) {
    k1_ = k1;
    k2_ = k2;
  }

  /**
   * \brief 
   * \return 
   */
  ForceGenerator* ForceOmniDrag::clone(void) {
    return new ForceOmniDrag(*this);
  }

  /**
   * \brief 
   * \param rhs 
   * \return 
   */
  bool ForceOmniDrag::operator==(const ForceGenerator* rhs) const {
    const ForceOmniDrag* temp =
      static_cast<const ForceOmniDrag*>(rhs);
    if (k1_ == temp->k1_)
      if (k2_ == temp->k2_)
        return true;
    return false;
  }

  /**
   * \brief 
   * \param body 
   * \param dt 
   */
  void ForceOmniDrag::updateForce(RigidBody* body, float dt) {
    glm::vec2 force(0);
    glm::vec2 velocity(0);

    velocity = body->getVelocity();
    velocity.x *= pow(k1_, dt);
    velocity.y *= pow(k1_, dt);

    body->setVelocity(velocity);
  }

  /**
   * \brief 
   * \return 
   */
  ForceType ForceOmniDrag::getType(void) const {
    return ForceType::FOmniDrag;
  }

  /****************************************************************************/
  //ForceAnchoredSpring
  /****************************************************************************/
  /**
   * \brief 
   * \param anchor 
   * \param springConstant 
   * \param restLength 
   */
  ForceAnchoredSpring::ForceAnchoredSpring(
    glm::vec2 anchor, float springConstant, float restLength) :
  anchor_(anchor), springConstant_(springConstant), restLength_(restLength){

  }

  /**
   * \brief 
   * \return 
   */
  ForceGenerator* ForceAnchoredSpring::clone(void) {
    return new ForceAnchoredSpring(*this);
  }

  /**
   * \brief 
   * \param rhs 
   * \return 
   */
  bool ForceAnchoredSpring::operator==(const ForceGenerator* rhs) const {
    const ForceAnchoredSpring* temp =
      static_cast<const ForceAnchoredSpring*>(rhs);
    if (anchor_ == temp->anchor_)
      if (springConstant_ == temp->springConstant_)
        if (restLength_ == temp->restLength_)
          return true;
    return false;
  }

  /**
   * \brief 
   * \param anchor 
   */
  void ForceAnchoredSpring::setAnchor(glm::vec2 anchor) {
    anchor_ = anchor;
  }

  /**
   * \brief 
   * \return 
   */
  glm::vec2 ForceAnchoredSpring::getAnchor(void) const {
    return anchor_;
  }

  /**
   * \brief 
   * \param body 
   * \param dt 
   */
  void ForceAnchoredSpring::updateForce(RigidBody* body, float dt) {
    glm::vec2 force = body->GetParent().Find<Transform>()->getPosition();

    force -= anchor_;

    float magnitude = (float)force.length();
    magnitude = (restLength_ - magnitude) * springConstant_;

    glm::normalize(force);
    force *= magnitude;
    body->addForce(force);
  }

  /**
   * \brief 
   * \return 
   */
  ForceType ForceAnchoredSpring::getType(void) const {
    return ForceType::FAnchorSpring;
  }

  /****************************************************************************/
  //ForceDirectedPush
  /****************************************************************************/
  /**
   * \brief 
   * \param normal 
   * \param power 
   */
  ForceDirectionalPush::ForceDirectionalPush(glm::vec2& normal, float power)
  : normal_(normal), power_(power){
  }

  /**
   * \brief 
   * \return 
   */
  ForceGenerator* ForceDirectionalPush::clone(void) {
    return new ForceDirectionalPush(*this);
  }

  /**
   * \brief 
   * \param rhs 
   * \return 
   */
  bool ForceDirectionalPush::operator==(const ForceGenerator* rhs) const {
    const ForceDirectionalPush* temp =
      static_cast<const ForceDirectionalPush*>(rhs);
    if (normal_ == temp->normal_)
      if (power_ == temp->power_)
        return true;
    return false;
  }

  /**
   * \brief 
   * \param normal 
   */
  void ForceDirectionalPush::setNormal(glm::vec2& normal) {
    normal_ = normal;
  }
  /**
   * \brief 
   * \return 
   */
  glm::vec2 ForceDirectionalPush::getNormal(void) const {
    return normal_;
  }
  /**
   * \brief 
   * \param power 
   */
  void ForceDirectionalPush::setPower(float power) {
    power_ = power;
  }
  /**
   * \brief 
   * \return 
   */
  float ForceDirectionalPush::getPower(void) const {
    return power_;
  }

  /**
   * \brief 
   * \param body 
   * \param dt 
   */
  void ForceDirectionalPush::updateForce(RigidBody* body, float dt) {
    glm::vec2 force = normal_;
    force *= power_;
    body->addForce(force);
  }

  /**
   * \brief 
   * \return 
   */
  ForceType ForceDirectionalPush::getType(void) const {
    return FDirection;
  }


  /****************************************************************************/
  //ForceProximityPush
  /****************************************************************************/
  /**
   * \brief 
   * \param anchor 
   * \param normal 
   * \param radius 
   * \param power 
   */
  ForceProximity::ForceProximity(
    glm::vec2& anchor, glm::vec2& normal, float radius, float power)
    : anchor_(anchor), normal_(normal), radius_(radius), power_(power) {
  }

  /**
   * \brief 
   * \return 
   */
  ForceGenerator* ForceProximity::clone(void) {
    return new ForceProximity(*this);
  }

  /**
   * \brief 
   * \param rhs 
   * \return 
   */
  bool ForceProximity::operator==(const ForceGenerator* rhs) const {
    const ForceProximity* temp =
      static_cast<const ForceProximity*>(rhs);
    if (anchor_ == temp->anchor_)
      if (normal_ == temp->normal_)
        if (radius_ == temp->radius_)
          if (power_ == temp->power_)
            return true;
    return false;
  }

  /**
   * \brief 
   * \param anchor 
   */
  void ForceProximity::setAnchor(glm::vec2& anchor) {
    anchor_ = anchor;
  }

  /**
   * \brief 
   * \return 
   */
  glm::vec2 ForceProximity::getAnchor(void) const {
    return anchor_;
  }

  /**
   * \brief 
   * \param normal 
   */
  void ForceProximity::setNormal(glm::vec2& normal) {
    normal_ = normal;
  }
  /**
   * \brief 
   * \return 
   */
  glm::vec2 ForceProximity::getNormal(void) const {
    return normal_;
  }
  /**
   * \brief 
   * \param radius 
   */
  void ForceProximity::setRadius(float radius) {
    radius_ = radius;
  }
  /**
   * \brief 
   * \return 
   */
  float ForceProximity::getRadius(void) const {
    return radius_;
  }

  /**
   * \brief 
   * \param power 
   */
  void ForceProximity::setPower(float power) {
    power_ = power;
  }
  /**
   * \brief 
   * \return 
   */
  float ForceProximity::getPower(void) const {
    return power_;
  }

  /**
   * \brief 
   * \param body 
   * \param dt 
   */
  void ForceProximity::updateForce(RigidBody* body, float dt) {
    glm::vec2 distance = body->GetParent().Find<Transform>()->getPosition();

    distance -= anchor_;

    float magnitude = (float)distance.length();
    magnitude = (radius_ - magnitude) * power_;
  
    glm::vec2 force = normal_;
    force *= abs(magnitude);

    body->addForce(force);
  }

  /**
   * \brief 
   * \return 
   */
  ForceType ForceProximity::getType(void) const {
    return FProximity;
  }

  /**
   * \brief 
   * \param type 
   * \return 
   */
  ForceType stringToType(std::string type) {
    if (type == "ForceGravity") {
      return FGravity;
    }
    else if (type == "ForceHorizontalDrag") {
      return FHorDrag;
    }
    else if (type == "ForceOmniDrag") {
      return FOmniDrag;
    }
    else if (type == "ForceAnchoredSpring") {
      return FAnchorSpring;
    }
    else if (type == "ForceDirection") {
      return FDirection;
    }
    else if (type == "ForceProximity") {
      return FProximity;
    }
    // necessary to silence warning `not all control paths return a value`, but execution should never reach this point
    return (ForceType)-1;
  }
  
  /**
   * \brief 
   * \param type 
   * \param vec1 
   * \param vec2 
   * \param val1 
   * \param val2 
   * \return 
   */
  ForceGenerator* makeForceGenerator(ForceType type,
    glm::vec2 vec1, glm::vec2 vec2, float val1, float val2) {
    ForceGenerator* forceGen;

    if (type == FGravity) {
      forceGen = new ForceGravity();
    }
    else if (type == FHorDrag) {
      forceGen = new ForceHorizontalDrag(val1, val2);
    }
    else if (type == FOmniDrag) {
      forceGen = new ForceOmniDrag(val1, val2);
    }
    else if (type == FAnchorSpring) {
      forceGen = new ForceAnchoredSpring(vec1, val1, val2);
    }
    else if (type == FDirection) {
      forceGen = new ForceDirectionalPush(vec1, val1);
    }
    else if (type == FProximity) {
      forceGen = new ForceProximity(vec1, vec2, val1, val2);
    }

    return forceGen;
  }

  /**
   * \brief 
   * \param type 
   * \param vec1 
   * \param vec2 
   * \param val1 
   * \param val2 
   * \return 
   */
  ForceGenerator* makeForceGenerator(std::string type,
    glm::vec2 vec1, glm::vec2 vec2, float val1, float val2) {
    return makeForceGenerator(stringToType(type), vec1, vec2, val1, val2);
  }
}