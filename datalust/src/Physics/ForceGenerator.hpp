/******************************************************************************/
/*!
  \file   ForceGenerator.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef _FORCEGENERATOR_HPP_
#define _FORCEGENERATOR_HPP_

#include "RigidBody.hpp"
#include <string>

/**
 * \brief 
 */
namespace KPE {

  /**
   * \brief 
   */
  class ForceEmitter;

  /**
   * \brief 
   */
  class ForceGenerator {
    bool is_emitter_;
  public:
    ForceGenerator(void);
    virtual ForceGenerator* clone(void) = 0;
    virtual bool operator==(const ForceGenerator* rhs) const = 0;
    virtual void updateForce(RigidBody* body, float dt) = 0;
    virtual ForceType getType(void) const = 0;
    void setEmitter(bool is_emitter);
    bool isEmitterForce(void) const;
  };
  
  class ForceGravity : public ForceGenerator {
    glm::vec2 gravity_;
  public:
    ForceGravity(void);
    ForceGenerator* clone(void);
    bool operator==(const ForceGenerator* rhs) const;
    void updateForce(RigidBody* body, float dt);
    ForceType getType(void) const;
  };

  class ForceHorizontalDrag : public ForceGenerator {
    float k1_;
    float k2_;
  public:
    ForceHorizontalDrag(float k1, float k2);
    ForceGenerator* clone(void);
    bool operator==(const ForceGenerator* rhs) const;
    void updateForce(RigidBody* body, float dt);
    ForceType getType(void) const;
  };

  class ForceOmniDrag : public ForceGenerator {
    float k1_;
    float k2_;
  public:
    ForceOmniDrag(float k1, float k2);
    ForceGenerator* clone(void);
    bool operator==(const ForceGenerator* rhs) const;
    void updateForce(RigidBody* body, float dt);
    ForceType getType(void) const;
  };

  class ForceAnchoredSpring : public ForceGenerator {
    glm::vec2 anchor_;
    float springConstant_;
    float restLength_;
  public:
    ForceAnchoredSpring(glm::vec2 anchor, float springConstant, float restLength);
    ForceGenerator* clone(void);
    bool operator==(const ForceGenerator* rhs) const;
    void setAnchor(glm::vec2 anchor);
    glm::vec2 getAnchor(void) const;
    void updateForce(RigidBody* body, float dt);
    ForceType getType(void) const;
  };

  class ForceDirectionalPush : public ForceGenerator {
    glm::vec2 normal_;
    float power_;
  public:
    ForceDirectionalPush(glm::vec2& normal, float power);
    ForceGenerator* clone(void);
    bool operator==(const ForceGenerator* rhs) const;
    void setNormal(glm::vec2& normal);
    glm::vec2 getNormal(void) const;
    void setPower(float power);
    float getPower(void) const;

    void updateForce(RigidBody* body, float dt);
    ForceType getType(void) const;
  };

  //Increasing effectivness based on proximity
  class ForceProximity : public ForceGenerator {
    glm::vec2 anchor_;
    glm::vec2 normal_;
    float radius_;
    float power_;
  public:
    ForceProximity(glm::vec2& anchor, glm::vec2& normal, float radius, float power);
    ForceGenerator* clone(void);
    bool operator==(const ForceGenerator* rhs) const;
    void setAnchor(glm::vec2& anchor);
    glm::vec2 getAnchor(void) const;
    void setNormal(glm::vec2& normal);
    glm::vec2 getNormal(void) const;
    void setRadius(float radius);
    float getRadius(void) const;
    void setPower(float power);
    float getPower(void) const;

    void updateForce(RigidBody* body, float dt);
    ForceType getType(void) const;
  };

  ForceType stringToType(std::string type);

  ForceGenerator* makeForceGenerator(ForceType type, 
    glm::vec2 vec1, glm::vec2 vec2, float val1, float val2);

  ForceGenerator* makeForceGenerator(std::string type,
    glm::vec2 vec1, glm::vec2 vec2, float val1, float val2);

}
#endif