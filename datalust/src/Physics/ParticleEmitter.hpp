/******************************************************************************/
/*!
  \file   ParticleEmitter.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
    
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.  
*/
/******************************************************************************/
#ifndef _PARTICLEEMITTER_HPP_
#define _PARTICLEEMITTER_HPP_

#include "Particle.hpp"
#include <GameObject/Component.h>
#include <vector>

// circle emitter
namespace KPE {
  class ParticleEmitter : public Component{

    glm::vec2 emitterSize_;  // the dimensions in which new particles will spawn

    glm::vec2 emissionPower_; // the starting velocity of a particle
    glm::vec2 powerVariance_; // random velocity added to particle

    float emissionSize_; // starting size of the particle
    float sizeVariance_; // amount the size may vary

    unsigned count_;     // private helper
    unsigned maxCount_;  // the number of particles the emitter spawns before being disabled. 0 = inf

    float particleLifetime_;  // how long a particle lives before dying
    float lifetimeVariance_;  // variance on particle lifetime

    float emissionRate_; //# of particle per second
    float rateVariance_; // average variation in particles/sec

    float emissionCounter_;

    float warmupTime_;
    bool firstUpdate_;

    bool  randomAngle_;     // sets whether particle should spawn at a random rotatiojn
    float angleVariance_;   // sets the amount the angle can vary from the starting angle
    float emissisonAngle_;  // sets the starting rotation of the particle
    
    std::vector<Particle> particles;

    constexpr bool isDone() const;

  public:

      // gets alive particles
     std::vector<Particle> &  GetParticles(); // l-value
     constexpr std::vector<Particle> const&  GetParticles() const; // r-value

    ParticleEmitter(GameObject& parent);

    ParticleEmitter* Clone_() const;

    void Update(float dt);

    virtual void Draw() const;
    //Accessors and Modifiers

    void setEmitterSize(glm::vec2 const& size);
    glm::vec2 const& getEmitterSize(void) const;

    void setPower(glm::vec2 const& power);
    glm::vec2 const& getPower(void) const;

    void setPowerVariance(glm::vec2 const& variance);
    glm::vec2 const& getPowerVariance(void) const;

    void setParticleSize(float size);
    float getParticleSize(void) const;

    void setSizeVariance(float variance);
    float getSizeVariance(void) const;

    void setMaxCount(unsigned count);
    unsigned getMaxCount(void) const;

    void setPLifetime(float lifetime);
    float getPLifetime(void) const;

    void setLifetimeVariance(float variance);
    float getLifetimeVariance(void) const;

    void setRate(float rate);
    float getRate(void) const;

    void setRateVariance(float variance);
    float getRateVariance(void) const;

    void setWarmuptime(float counter);
    float getWarmuptime(void) const;

    void setRandomAngle(bool b);
    bool isRandomAngle(void) const;

    void setAngle(float angle);

    float getAngle(void) const;

    void setAngleVariance(float variance);
    float getAngleVariance() const;

    void Reset(); // resets counts, destroys all particles, performs first update again

  private:
    float randomFloat(float min, float max);

    void UpdateTick(float dt, glm::vec2 const& origin);
  };
}

#endif