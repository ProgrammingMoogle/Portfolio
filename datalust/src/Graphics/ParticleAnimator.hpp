// ParticleAnimator.hpp
// Patrick Cook
// Defines how particles change over their lifetime

#ifndef PARTICLE_ANIMATOR_HPP
#define PARTICLE_ANIMATOR_HPP

#include "GameObject\Component.h"
#include <glm\vec2.hpp>

namespace KPE {
  class ParticleAnimator : public Component
  {
  public:
    // ctors and dtors
    ParticleAnimator(GameObject& parent);

    virtual ~ParticleAnimator(void);

    virtual Component * Clone_() const;

    virtual void Update(float dt);

    void setRandomForce(glm::vec2 const& rForce);
    void setForce(glm::vec2 const& force);
    void setTorque(float torque);
    void setGrowth(float growth);
    void setGravity(bool gravity);
    void setDampening(float dampening);

    constexpr glm::vec2 const& getRandomForce() const;
    constexpr glm::vec2 const& getForce() const;
    constexpr float getTorque() const;
    float getGrowth() const;
    constexpr bool getGravity() const;
    constexpr float getDampening() const;

  private:

    float torque_;          // affects particle rotational velocity

    glm::vec2 force_;       // affects particle velocity

    float growth_;          // affects particle size

    bool gravity_;          // if set, applies global gravity vector to particles

    glm::vec2 randomForce_; // randomness on top of the force

    float dampening_;       // factor to reduce velocity by every second


    static float randomFloat(float min, float max);
    static glm::vec2 randomVec(glm::vec2 const& range);

  };
}


#endif //PARTICLE_ANIMATOR_HPP