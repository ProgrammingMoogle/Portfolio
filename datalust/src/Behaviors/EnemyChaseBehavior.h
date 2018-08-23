/*************************************************************************/
/*!
\file   EnemyChaseBehavior.h
\author Studio200
\par
\par    DigiPen login:
\par    Course: GAME 200
\par   Copyright � 2017 DigiPen (USA) Corp. and its owners. All Rights Reserved.
\date   9/28/2017
\brief
Functions include:
*/
/*************************************************************************/
#pragma once
#include <GameObject\Component.h>
#include "Behaviors/EnemyBehavior.hpp"
#include <Messaging\Messaging.hpp>

//This contains functions for making the attached object move 
//towards the player

/**
 * \brief 
 */
class EnemyChaseBehavior : public EnemyBehavior {
  enum BehaviorType {
    Chase,
    FlankRight,
    FlankLeft
  };
  //Base Data
  BehaviorType currentBehavior_;
  float timer_;
  Transform* playerTrans;
  glm::vec2 playerPos;
  float movespeed_;
  float originalSpeed_;
  float dmg_;
public:
  EnemyChaseBehavior(GameObject &parent, float movespeed,
    bool shoots, bool shootsEnemy, glm::vec2 shootDir, float shootDelay, float bulletSpeed
    );
  ~EnemyChaseBehavior();
  void Update(float dt);
  virtual Component* Clone_() const { return new EnemyChaseBehavior(*this); }
private:
  void setBehaviorType(void);
  void behaviorFilter(glm::vec2& enemyPos);
  void behaviorDefault(glm::vec2& enemyPos, glm::vec2& playerDirection);
  void behaviorFlankRight(glm::vec2& enemyPos, glm::vec2& playerDirection);
  void behaviorFlankLeft(glm::vec2& enemyPos, glm::vec2& playerDirection);
};