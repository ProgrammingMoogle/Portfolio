/******************************************************************************/
/*!
  \file   ParticleEmitter.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <algorithm>
#include "Transform\Transform.hpp"
#include "GameObject\GameObject.h"
#include "Graphics\ParticleAnimator.hpp"
#include "ParticleSystem.hpp"
#include "ParticleEmitter.hpp"
#include <cmath>

static constexpr float pi = 3.14159265358979323846264338327950288419716939937510582f;

namespace KPE {


  static float randomFloat(float min, float max)
  {
    return min + static_cast<float>(rand()) /
      (static_cast<float>(RAND_MAX / (max - min)));
  }

  static float randomFloatRange(float range)
  {
    return randomFloat(-range, +range);
  }

  static glm::vec2 randomCircle(glm::vec2 const& size)
  {
    const float angle = randomFloat(0, pi * 2.0f);
    
    // sqrt is required to uniformly generate points
    return glm::vec2(
      std::cos(angle) * std::sqrt(randomFloat(0, 0.25f * size.x * size.x)),
      std::sin(angle) * std::sqrt(randomFloat(0, 0.25f * size.y * size.y))
    );
  }


  static glm::vec2 randomVec(glm::vec2 const& range)
  {
    return glm::vec2(
      randomFloatRange(range.x),
      randomFloatRange(range.y)
    );
  }



  constexpr bool ParticleEmitter::isDone() const
  {
    return count_ != 0 && maxCount_ >= count_;
  }

  struct ParticleSorter
  {
    // newer particles draw on top of older ones
    constexpr bool operator()(Particle const& lhs, Particle const& rhs) const
    {
      return lhs.lifetime_ < rhs.lifetime_;
    }
  };

  ParticleEmitter::ParticleEmitter(GameObject& parent)
    : Component(parent),
    count_(0),
    maxCount_(0),
    particleLifetime_(1.0f),
    emissionRate_(0),
    emissionCounter_(0),
    emissisonAngle_(0),
    emissionPower_(0),
    particles()
  {
    particles.reserve(128);
  }


  ParticleEmitter* ParticleEmitter::Clone_() const {
    return new ParticleEmitter(*this);
  }

  void ParticleEmitter::UpdateTick(float dt, glm::vec2 const& origin)
  {
    //Update existing particles
    for (Particle & particle : particles)
      particle.update(dt);


    // Erase-Remove idiom
    particles.erase(
      std::remove_if(particles.begin(), particles.end(),
        [](Particle const& particle) { return particle.lifetime_ <= 0.0f; }
      ),

      particles.end()
    );

    if(!isDone())
      emissionCounter_ += dt;

    while (emissionCounter_ >= 1.0f / emissionRate_ && !isDone())
    {

      emissionCounter_ -= 1.0f / std::abs(emissionRate_ + randomFloatRange(rateVariance_));

      particles.emplace_back(
        Particle(
          // translation
          origin + randomCircle(emitterSize_),

          // velocity
          emissionPower_ + randomVec(powerVariance_),

          // rotation
          ((randomAngle_)
            ? randomFloat(0.0f, 2.0f * pi)                         // completely random
            : emissisonAngle_ + randomFloatRange(angleVariance_)), // configurable

          // liftime
          particleLifetime_ + randomFloatRange(lifetimeVariance_),

          // unary scale
          emissionSize_ + randomFloatRange(sizeVariance_)
        )
      );

      maxCount_++;
    }

    // Sort particles by lifetime (draw order)
    std::sort(particles.begin(), particles.end(), ParticleSorter());
  }

  void ParticleEmitter::Update(float dt) {



    //Generate new particles

    // Remark: Rather than a timer, 
    // maybe having a number of particles we wish to draw  is better - Patrick


    Transform * transform = GetParent().Find<Transform>();
    ParticleSystem *system = GetParent().Find<ParticleSystem>();

    const glm::vec2 systemOrigin =
      ((transform != nullptr && (system != nullptr && system->getSpace() != ParticleSystem::SystemSpace::Object))
        ? transform->getPosition() // in world space, add position
        : glm::vec2(0, 0));        // in system space, spawn from 0,0

    if (firstUpdate_)
    {
      // coarse time step
      constexpr float timeSlice = 1.0f / 15.0f;

      float remaining = warmupTime_;

      ParticleAnimator *animator = GetParent().Find<ParticleAnimator>();

      while (remaining > 0)
      {
        UpdateTick(timeSlice, systemOrigin);
        if (animator != nullptr) animator->Update(timeSlice);
        remaining -= timeSlice;

      }

      firstUpdate_ = false;
    }

    UpdateTick(dt, systemOrigin);
  }

  float ParticleEmitter::randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) /
      (static_cast<float>(RAND_MAX / (max - min)));
  }

  void ParticleEmitter::setMaxCount(unsigned count)
  {
    count_ = count;
  }

  unsigned ParticleEmitter::getMaxCount(void) const
  {
    return count_;
  }

  void  ParticleEmitter::Draw() const
  {
  }

  void ParticleEmitter::setEmitterSize(glm::vec2 const & size)
  {
    emitterSize_ = glm::vec2(std::abs(size.x), std::abs(size.y));
  }

  glm::vec2 const & ParticleEmitter::getEmitterSize(void) const
  {
    return emitterSize_;
  }

  void ParticleEmitter::setPLifetime(float lifetime) {
    particleLifetime_ = lifetime;
    particles.reserve(static_cast<size_t>(std::ceil(getPLifetime() * getRate() * 1.1f)));
  }

  float ParticleEmitter::getPLifetime(void) const {
    return particleLifetime_;
  }

  void ParticleEmitter::setLifetimeVariance(float variance)
  {
    lifetimeVariance_ = std::abs(variance);
  }

  float ParticleEmitter::getLifetimeVariance(void) const
  {
    return lifetimeVariance_;
  }

  void ParticleEmitter::setRate(float rate) {
    emissionRate_ = std::abs(rate);
    particles.reserve(static_cast<size_t>(std::ceil(getPLifetime() * getRate() * 1.1f)));
  }

  float ParticleEmitter::getRate(void) const {
    return  emissionRate_;
  }

  void ParticleEmitter::setRateVariance(float variance)
  {
    rateVariance_ = std::abs(variance);
  }

  float ParticleEmitter::getRateVariance(void) const
  {
    return rateVariance_;
  }

  void ParticleEmitter::setWarmuptime(float counter) {
    warmupTime_ = counter;
  }

  float ParticleEmitter::getWarmuptime(void) const {
    return warmupTime_;
  }

  void ParticleEmitter::setRandomAngle(bool b)
  {
    randomAngle_ = b;
  }

  bool ParticleEmitter::isRandomAngle(void) const
  {
    return randomAngle_;
  }

  void ParticleEmitter::setAngle(float angle) {
    emissisonAngle_ = pi * angle / 180.0f;
  }

  float ParticleEmitter::getAngle(void) const {
    return emissisonAngle_;
  }

  void ParticleEmitter::setAngleVariance(float variance)
  {
    angleVariance_ = pi * variance / 180.0f;
  }

  float ParticleEmitter::getAngleVariance() const
  {
    return 180.0f * angleVariance_ / pi;
  }

  void ParticleEmitter::setPower(glm::vec2 const& power) {
    emissionPower_ = power;
  }

  glm::vec2 const& ParticleEmitter::getPower(void) const {
    return emissionPower_;
  }

  void ParticleEmitter::setPowerVariance(glm::vec2 const & variance)
  {
    powerVariance_ = glm::vec2(std::abs(variance.x), std::abs(variance.y));
  }

  glm::vec2 const & ParticleEmitter::getPowerVariance(void) const
  {
    return powerVariance_;
  }

  void ParticleEmitter::setParticleSize(float size)
  {
    emissionSize_ = size;
  }

  float ParticleEmitter::getParticleSize(void) const
  {
    return emissionSize_;
  }

  void ParticleEmitter::setSizeVariance(float variance)
  {
    sizeVariance_ = std::abs(variance);
  }

  float ParticleEmitter::getSizeVariance(void) const
  {
    return sizeVariance_;
  }

  std::vector<Particle>&
    ParticleEmitter::GetParticles()
  {
    return particles;
  }

  constexpr std::vector<Particle> const&
    ParticleEmitter::GetParticles() const
  {
    return particles;
  }

  void ParticleEmitter::Reset()
  {
    particles.clear();
    maxCount_ = 0;
    firstUpdate_ = true;
  }
}
