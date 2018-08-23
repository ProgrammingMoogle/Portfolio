/*************************************************************************/
/*!
\file   EnemyChaseBehavior.cpp
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
#include <Engine/Engine.h>
#include <GameObject/GameObject.h>
#include <Transform/Transform.hpp>
#include <Physics/RigidBody.hpp>
#include "EnemyChaseBehavior.h"
#include "HealthComponent.h"

#define FLANK_ANGLE 0.523f

/**
 * \brief 
 * \param parent 
 * \param movespeed 
 * \param shoots 
 * \param shootsEnemy 
 * \param shootDir 
 * \param shootDelay 
 * \param bulletSpeed 
 */
EnemyChaseBehavior::EnemyChaseBehavior(GameObject &parent, float movespeed,
  bool shoots, bool shootsEnemy, glm::vec2 shootDir, float shootDelay, float bulletSpeed)
  :
  EnemyBehavior{ parent, movespeed, shoots, shootsEnemy, shootDir, bulletSpeed, shootDelay },
originalSpeed_(movespeed), dmg_(1){
  //Find player transform pointer
  GameObjectManager& manager = engine.Find<GameStateManager>()->CurrentState()->Manager();
  playerTrans = manager.Find("Player1").Find<Transform>();
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  m_handler->Register(Messaging::ID::Collision, GetParent().GetKey(), EnemyBehavior::OnCollision<EnemyChaseBehavior>);

}


/**
 * \brief 
 */
EnemyChaseBehavior::~EnemyChaseBehavior() {
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  m_handler->Unregister(Messaging::ID::Collision, GetParent().GetKey());
}

/**
 * \brief 
 * \param dt 
 */
void EnemyChaseBehavior::Update(float dt) {
  glm::vec2 enemyPos = GetParent().Find<Transform>()->getPosition();

  // If this enemy is still active
  if (!GetParent().IsDestroyed()) {
    EnemyBehavior::Update(dt);
    // If the player's transform still exists
    if (playerTrans) {
      // Check current timer
      if (timer_ > 0) {
        //Execute behavior
        behaviorFilter(enemyPos);
        //Decrement timer
        timer_ -= dt;
      }
      else {
        //Set a new behavior
        setBehaviorType();
      }
    }
  }
}



//Private Functions

  //Analyzes data in order to figure out the best aciton to take
/**
 * \brief 
 */
void EnemyChaseBehavior::setBehaviorType(void) {
  int random = rand();
  int state = random % 1000;
  float timer = static_cast<float>(random % 3000);
  timer /= 1000.0f;
  

  if (random < 550) {
    movespeed = originalSpeed_;
    currentBehavior_ = Chase;
    timer_ = timer;
  }
  else if (random >= 600 && random < 800) {
    movespeed = originalSpeed_ * 1.5f;
    currentBehavior_ = FlankRight;
    timer_ = timer;
  }
  else if (random >= 800 && random < 1000) {
    movespeed = originalSpeed_ * 1.5f;
    currentBehavior_ = FlankLeft;
    timer_ = timer;
  }


  //Find last distance travelled
  float distanceTravelled = 
    length(GetParent().Find<Transform>()->getOldPosition() 
      - GetParent().Find<Transform>()->getPosition());

  //If the distanced travelled is almost nothing (as in it hit something)
  if (distanceTravelled < 0.1f) {
    //Default to a chase
    currentBehavior_ = Chase;
    timer_ = .5f;
  }
  else {
    //If not figure out the best action to take
    //Check distance to player
    //If too far, dash
    //Else check angle to player

  }
}


/**
 * \brief 
 * \param enemyPos 
 */
void EnemyChaseBehavior::behaviorFilter(glm::vec2& enemyPos) {
  glm::vec2 playerDirection;

  switch (currentBehavior_) {
  case Chase:
    behaviorDefault(enemyPos, playerDirection);
    break;
  case FlankRight:
    behaviorFlankRight(enemyPos, playerDirection);
    break;
  case FlankLeft:
    behaviorFlankLeft(enemyPos, playerDirection);
    break;
  }

  //Set speed
  playerDirection *= movespeed;

  //Grab this objects, rigidbody and set it's velocity
  KPE::RigidBody* enemyBody = GetParent().Find<KPE::RigidBody>();
  if (enemyBody) {
    //Only if a rigidbody exists
    enemyBody->setVelocity(playerDirection);
  }
}

/**
 * \brief 
 * \param enemyPos 
 * \param playerDirection 
 */
void EnemyChaseBehavior::behaviorDefault(glm::vec2& enemyPos, glm::vec2& playerDirection) {
  //Grab player position, find direction
  glm::vec2 playerPos = playerTrans->getPosition();
  playerDirection = playerPos - enemyPos;
  playerDirection = normalize(playerDirection);
}

/**
 * \brief 
 * \param enemyPos 
 * \param playerDirection 
 */
void EnemyChaseBehavior::behaviorFlankRight(glm::vec2& enemyPos, glm::vec2& playerDirection) {
  //Grab player position, find direction
  glm::vec2 playerPos = playerTrans->getPosition();
  playerDirection = playerPos - enemyPos;
  playerDirection = normalize(playerDirection);

  //Go to the right
  float modifier = FLANK_ANGLE * -1.0f;
  float xTemp = playerDirection.x;

  playerDirection.x = xTemp * cos(modifier) - playerDirection.y * sin(modifier);
  playerDirection.y = xTemp * sin(modifier) + playerDirection.y * cos(modifier);
  
}

/**
 * \brief 
 * \param enemyPos 
 * \param playerDirection 
 */
void EnemyChaseBehavior::behaviorFlankLeft(glm::vec2& enemyPos, glm::vec2& playerDirection) {
  //Grab player position, find direction
  glm::vec2 playerPos = playerTrans->getPosition();
  playerDirection = playerPos - enemyPos;
  playerDirection = normalize(playerDirection);

  //Go to the left
  float modifier = FLANK_ANGLE;
  float xTemp = playerDirection.x;

  playerDirection.x = xTemp * cos(modifier) - playerDirection.y * sin(modifier);
  playerDirection.y = xTemp * sin(modifier) + playerDirection.y * cos(modifier);
}


