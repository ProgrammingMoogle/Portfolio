#include "Graphics\ParticleAnimator.hpp" 
#include "Physics\ParticleEmitter.hpp"
#include "Physics\Particle.hpp"
#include "Physics\PhysicsConstants.hpp"
#include "GameObject\GameObject.h"
#include <cmath>


namespace KPE
{
  glm::vec2 ParticleAnimator::randomVec(glm::vec2 const& range)
  {
    return glm::vec2(
      randomFloat(-range.x, +range.x),
      randomFloat(-range.y, +range.y)
    );
  }

  float ParticleAnimator::randomFloat(float min, float max)
  {
    return min + static_cast<float>(rand()) /
      (static_cast<float>(RAND_MAX / (max - min)));
  }



  ParticleAnimator::ParticleAnimator(GameObject& parent) : Component(parent),
    torque_(0),
    force_(0, 0),
    growth_(0),
    gravity_(false)
  {

  }

  ParticleAnimator::~ParticleAnimator(void)
  {
  }

  Component * ParticleAnimator::Clone_() const
  {
    return new ParticleAnimator(*this);
  }

  void ParticleAnimator::Update(float dt)
  {
    ParticleEmitter * emitter = GetParent().Find<ParticleEmitter>();

    if (emitter == nullptr)
      return;

    std::vector<Particle> & particles = emitter->GetParticles();

    for (Particle & particle : particles)
    {
      particle.rotation_ += torque_ * dt;

      particle.size_     += growth_ * dt;
      if (growth_ < 0 && particle.size_ < 0) particle.size_ = 0;

      // not sure whether to distribute dt or keep the mults separate
      particle.velocity_ += dt * force_;
      particle.velocity_ += dt * randomVec(randomForce_);
      
      if (gravity_)
        particle.velocity_ += dt * GRAV_VEC;

      particle.velocity_ -= particle.velocity_ * dampening_ * dt;
    }
  }

  void  ParticleAnimator::setRandomForce(glm::vec2 const& rForce)
  {
    randomForce_.y = std::abs(rForce.y);
    randomForce_.x = std::abs(rForce.x);
  }

  void  ParticleAnimator::setForce(glm::vec2 const& force)
  {
    force_ = force;
  }

  void  ParticleAnimator::setTorque(float torque)
  {
    torque_ = torque;
  }

  void  ParticleAnimator::setGrowth(float growth)
  {
    growth_ = growth;
  }


  void  ParticleAnimator::setGravity(bool gravity)
  {
    gravity_ = gravity;
  }

  void ParticleAnimator::setDampening(float dampening)
  {
    dampening_ = std::abs(dampening);
  }

  constexpr glm::vec2 const&  ParticleAnimator::getRandomForce() const
  {
    return randomForce_; 
  }

  constexpr glm::vec2 const&  ParticleAnimator::getForce()       const
  {
    return force_;
  }

  constexpr            float  ParticleAnimator::getTorque()      const
  {
    return torque_;
  }

  float  ParticleAnimator::getGrowth()      const
  {
    return growth_;
  }

  constexpr             bool  ParticleAnimator::getGravity()     const
  {
    return gravity_;
  }

  constexpr float ParticleAnimator::getDampening() const
  {
    return dampening_;
  }
}