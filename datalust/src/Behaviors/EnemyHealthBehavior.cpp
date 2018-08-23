/*************************************************************************/
/*!
\file   EnemyHealthBehavior.h
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
#include "EnemyHealthBehavior.h"
#include <Engine/Engine.h>
#include "HealthComponent.h"


//You must have an id attached to this behavior
//Enemies should never have more than 1 hp behavior
/**
 * \brief 
 * \param parent 
 * \param id 
 * \param totalHP 
 */
EnemyHealthBehavior::EnemyHealthBehavior(GameObject &parent, ObjectId id, int totalHP) :
Component(parent), enemyID_(id), totalHP_(totalHP), color_(BulletType::NONE) {
}


/**
 * \brief s
 */
EnemyHealthBehavior::~EnemyHealthBehavior() {

}

/**
 * \brief 
 * \param dt 
 */
void EnemyHealthBehavior::Update(float dt) {

  // If this enemy is still active
  if (!GetParent().IsDestroyed()) {
    //Check if this enemy is dead
    if (totalHP_ <= 0) {
      //Death Animation?
      //jfi:VV
      /*TODO: DROP COINS*/

      //Delete this enemy
      GetParent().Destroy();
    }
  }
}

/**
 * \brief 
 * \param id 
 * \param message 
 */
void EnemyHealthBehavior::CollisionHandler(ObjectId id, const Messaging::Message& message) {
  
}

/**
 * \brief 
 * \param message 
 */
void EnemyHealthBehavior::onCollision(const Messaging::Message& message) {
  //Change this enemy's color
  KPE::RigidBody* otherBody = message.message.collision.otherBody_;

  if (otherBody->GetParent().GetType() == "Player1")
  {
    otherBody->GetParent().Find<HealthComponent>()->DoDamage(1);//enemy damage
  }


}