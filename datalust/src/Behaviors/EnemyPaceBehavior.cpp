/*************************************************************************/
/*!
\file   EnemyPaceBehavior.cpp
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
#include <Engine/Engine.h>
#include <GameObject/GameObject.h>
#include <Transform/Transform.hpp>
#include <Physics/RigidBody.hpp>
#include "EnemyPaceBehavior.h"

#define DISTANCE_BUFFER 1.0f
#define POINT_BUFFER 3.0f
#define CORRECTION_TIMER 3.0f

/**
 * \brief 
 * \param point 
 * \param prev 
 * \param next 
 */
EnemyPaceBehavior::PointNode::PointNode(glm::vec2 point, PointNode *prev, PointNode *next) :
  position_(point),
  prev_{ prev },
  next_{ next } 
{
}

// PEC: Moved everything within reason to initializer list
/**
 * \brief 
 * \param parent 
 * \param shoots 
 * \param shootsEnemy 
 * \param shootDir 
 * \param shootDelay 
 * \param bulletSpeed 
 * \param reverses 
 * \param _movespeed 
 * \param offsets 
 */
EnemyPaceBehavior::EnemyPaceBehavior(GameObject &parent,
  bool shoots, bool shootsEnemy, glm::vec2 shootDir, float shootDelay, float bulletSpeed,
  bool reverses, float _movespeed, std::vector<glm::vec2> && offsets) :
  //Init base behavior
  EnemyBehavior{ parent, _movespeed, shoots, shootsEnemy, shootDir, bulletSpeed, shootDelay },
  //Init Linked List
  head_{new PointNode{GetParent().Find<Transform>()->getPosition()}}, tail_{head_}, current_{head_},
  //Starting point of the enemy is the first point
  prevPos_{head_->position_},
  //Init Min/Max values
  maxX_{0}, minX_{0}, maxY_{0}, minY_{0},
  //Init correction info
  correctingSpeed_{ false }, correctingStuck_{false}, correctionFlip_{1},
  //Init reverse info
  reverses_(reverses), isReversing_{false}
{
  // push back each offset onto the linked list
  for (auto && offset : offsets)
  {
    addPoint(offset);
  }

  if (offsets.empty())
    return;

  heading_ = head_->next_;
  direction_ = glm::normalize(heading_->position_ - current_->position_);

  //Set Max/Min
  calculateData(head_, NULL);

  auto m_handler = engine.Find<Messaging::MessageHandler>();
  m_handler->Register(Messaging::ID::Collision, GetParent().GetKey(), EnemyBehavior::OnCollision<EnemyPaceBehavior>);

}


/**
 * \brief 
 */
EnemyPaceBehavior::~EnemyPaceBehavior() {
  auto m_handler = engine.Find<Messaging::MessageHandler>();
  m_handler->Unregister(Messaging::ID::Collision, GetParent().GetKey());

  clear();
}

/**
 * \brief 
 * \param dt 
 */
void EnemyPaceBehavior::Update(float dt) {
  //Check if this enemy still exists
  if (GetParent().IsDestroyed()) {
    return;
  }

  EnemyBehavior::Update(dt);

  //Check if this enemy has moved (check if stuck)
  glm::vec2 movedDistance = GetParent().Find<Transform>()->getPosition() - prevPos_;
  prevPos_ = GetParent().Find<Transform>()->getPosition();

  //Find distance to current heading
  glm::vec2 distanceToHeading = heading_->position_ - 
                                GetParent().Find<Transform>()->getPosition();                

  //Moving in a special direction to fix if stuck
  if (correctingStuck_) {
    if (timer_ >= 0.0f) {
      timer_ -= dt;
      return;
    }
    else {
      correctingStuck_ = false;
      correctionFlip_ *= -1;
    }
  }


  //Enemy has not moved a distance equal to the movespeed
  if (abs(length(movedDistance)) < movespeed - DISTANCE_BUFFER &&
      correctingSpeed_ == false) {
    //Fixing stuck
    correctingStuck_ = true;
    
    //Pick a perpendicular direction from current heading and move that way
    direction_ = normalize(distanceToHeading);
    float tempVal = direction_.x;
    direction_.x = direction_.y;
    direction_.y = tempVal * -1.0f;

    if (correctionFlip_ > 0) {
      //Does Nothing
    }
    else {
      //Reverses the direction
      direction_ *= -1.0f;
    }

    //Set Timer
    timer_ = CORRECTION_TIMER;
  }
  //Enemy has moved the correct amount of distance
  else {
    correctingSpeed_ = false;

    //Check if the object has encountered the next point
    if (inRange(GetParent().Find<Transform>()->getPosition())) {
      //Set heading to the next point
      if (heading_->next_ != NULL && isReversing_ == false) {
        current_ = heading_;
        heading_ = heading_->next_;
      }
      else if (heading_->prev_ != NULL && isReversing_ == true) {
        current_ = heading_;
        heading_ = heading_->prev_;
      }
      else {
        //Check if reversing
        if (reverses_) {
          if (!isReversing_) {
            current_ = tail_;
            heading_ = tail_->prev_;
            isReversing_ = true;
          }
          else {
            current_ = head_;
            heading_ = head_->next_;
            isReversing_ = false;
          }
        }
        else {
          //Reset to first point
          current_ = head_;
          heading_ = head_->next_;
          isReversing_ = false;
        }
      }
      //Set new direction
      direction_ = normalize(heading_->position_ - GetParent().Find<Transform>()->getPosition());
      //Set velocity
      GetParent().Find<KPE::RigidBody>()->setVelocity(direction_ * movespeed);
    }
    //Not yet at next point
    else {
      //Check if the distance is less than the current movespeed * dt
      if (length(distanceToHeading) < movespeed * dt) {
        //Set velocity to a special value
        GetParent().Find<KPE::RigidBody>()->setVelocity(distanceToHeading / dt);
        correctingSpeed_ = true;
      }
      else {
        direction_ = normalize(heading_->position_ - GetParent().Find<Transform>()->getPosition());
        //Set velocity in the direction of the next point
        GetParent().Find<KPE::RigidBody>()->setVelocity(direction_ * movespeed);
      }
    }
  }

}

void EnemyPaceBehavior::addPoint(glm::vec2 const& offset) {
  tail_->next_ = new PointNode(head_->position_ + offset, tail_);
  tail_ = tail_->next_;
  //calculateData();
}

bool EnemyPaceBehavior::inRange(glm::vec2 const& curPos) {
  if (curPos.x >= heading_->position_.x - POINT_BUFFER &&
      curPos.x <= heading_->position_.x + POINT_BUFFER &&
      curPos.y >= heading_->position_.y - POINT_BUFFER &&
      curPos.y <= heading_->position_.y + POINT_BUFFER) {
    return true;
  }

  return false;
}

void EnemyPaceBehavior::calculateData(PointNode* start, PointNode* end) {
  // Reset Min/Max
  minX_ = maxX_ = start->position_.x;
  minY_ = maxY_ = start->position_.y;

  //Walk list, find highest lowest values
  PointNode* walker = start->next_;
  while (walker != end) {
    minX_ = std::min(minX_, walker->position_.x);
    maxX_ = std::max(maxX_, walker->position_.x);

    minY_ = std::min(minY_, walker->position_.y);
    maxY_ = std::max(maxY_, walker->position_.y);

    walker = walker->next_;
  }

}

void EnemyPaceBehavior::clear() {
  PointNode* current = head_;
  PointNode* next = NULL;

  while (current != NULL) {
    next = current->next_;

    delete current;

    current = next;
  }
  
}