#include "BaseAI_Behavior.h"
#include <Engine/Engine.h>
#include <Messaging/Messaging.hpp>
#include <Transform/Transform.hpp>
#include <Graphics/Sprite.hpp>
#include <Behaviors/HUDBehavior.hpp>
#include <Audio/Audio.hpp>
#ifndef PI
#define	PI		3.1415926f
#endif

BaseAI_Behavior::BaseAI_Behavior(GameObject& parent) : Component(parent), _timer(0.0f), _timer2(0.0f), _randValue(0.0f)
{
}

void BaseAI_Behavior::MoveAI(float DT)
{
  float MovementSpeed = 5.0f;

  float xDiff = GetParent().Find<Transform>()->getPosition().x - GetParent().Find<Transform>()->getPosition().x;
  float yDiff = GetParent().Find<Transform>()->getPosition().y - GetParent().Find<Transform>()->getPosition().y;
  float angle = (float)(atan2(yDiff, xDiff));

  _timer -= DT;

  if (_timer <= 0.0f)
  {
    glm::vec2 currv = GetParent().Find<KPE::RigidBody>()->getVelocity();
    if (_randValue == -1.0f)
    {
      currv.x = 0;
      currv.y = 0;

      GetParent().Find<KPE::RigidBody>()->setVelocity(currv);
      _randValue = RandomFloatRange(0.0, 1.0) * 10.0f;
      _timer2 = 0.5f;
    }
    if (_randValue == -2.0f)
    {
      currv.x = 1.0f;
      currv.y = 1.0f;

      GetParent().Find<KPE::RigidBody>()->setVelocity(currv);
    }

    if (_randValue >= 0.0f)//north
    {

      currv.x = cosf(angle) * MovementSpeed * RandomFloatRange(0.0, 1.0);
      currv.y = sinf(angle) * MovementSpeed * RandomFloatRange(0.0, 1.0);
      _angle = angle;
      _randValue = -2.0f;
    }
    else if (_randValue <= 4.0f && _randValue >= 0.0f)//south
    {
      currv.x = -(cosf(angle) * MovementSpeed * RandomFloatRange(0.0, 1.0));
      currv.y = -(sinf(angle) * MovementSpeed * RandomFloatRange(0.0, 1.0));
      GetParent().Find<KPE::RigidBody>()->setVelocity(currv);
      _angle = angle + PI;
      _randValue = -2.0f;
    }
    else if (_randValue <= 7.0f && _randValue >= 0.0f)//east
    {
      currv.x = (cosf(angle * PI / 2) * MovementSpeed * DT);
      currv.y = (sinf(angle * PI / 2) * MovementSpeed * DT);
      _angle = angle * PI / 2.0f;
      _randValue = -2.0f;
    }
    else if (_randValue <= 10.0f && _randValue >= 0.0f)//west
    {
      currv.x = (float)(cosf(angle * -(PI / 2)) * MovementSpeed * DT);
      currv.y = (float)(sinf(angle * -(PI / 2)) * MovementSpeed * DT);
      _angle = angle * -(PI / 2.0f);
      _randValue = -2.0f;
    }

    _timer2 -= DT;

    if (_timer <= 0.0f)
    {
      _randValue = -1.0f;
      _timer = 1.2f + RandomFloatRange(0.0, 1.0);
    }
  }
}


float BaseAI_Behavior::RandomFloatRange(float begin, float end)
{
  float randfloat = (float)rand() / RAND_MAX; // a random float between 0 and 1
  randfloat *= (end - begin);
  randfloat += begin;
  return randfloat;
}