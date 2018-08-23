/*************************************************************************/
/*!
\file   EnemyPaceBehavior.h
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
#include "Behaviors/EnemyBehavior.hpp"
#include <Messaging\Messaging.hpp>

//This contains functions for making the attached object move 
//back and forth

//

/**
 * \brief 
 */
class EnemyPaceBehavior : public EnemyBehavior {
  struct PointNode {
    PointNode(glm::vec2 point, PointNode *prev = NULL, PointNode *next = NULL);
    glm::vec2 position_;
    PointNode* prev_;
    PointNode* next_;
  };

  PointNode* head_;       //Head of list
  PointNode* tail_;       //Tail of list
  PointNode* current_;    //Start point of enemy
  PointNode* heading_;    //Next point the enemy is headed
  glm::vec2 direction_;   //Direction enemy should be moving
  glm::vec2 prevPos_;     //
  float maxX_;            //
  float minX_;            //
  float maxY_;            //
  float minY_;            //
  bool  correctingSpeed_; //Using a special speed to hit the point?
  float timer_;           //Timer for correcting being stuck
  bool correctingStuck_;  //Is this object stuck and is correcting it?
  int correctionFlip_;    //Perpendicular direction flip
  bool reverses_;
  bool isReversing_;
public:
  EnemyPaceBehavior(GameObject &parent,
    bool shoots, bool shootsEnemy, glm::vec2 shootDir, float shootDelay, float bulletSpeed,
    bool reverses, float movespeed, std::vector<glm::vec2> && offsets);
  ~EnemyPaceBehavior();
  void Update(float dt);
  void addPoint(glm::vec2 const& offset);
  virtual Component* Clone_() const { return new EnemyPaceBehavior(*this); }
private:
  bool inRange(glm::vec2 const& curPos);
  void calculateData(PointNode* start, PointNode* end);
  void clear(void);

};