/*************************************************************************/
/*!
\file   RoomSwitchBehavior.h
\author studio 200
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
#include "GameObject/Component.h"
#include <glm/detail/type_vec2.hpp>
#include <set>
#include "GameObject/GameObjectManager.h"

/**
 * \brief 
 */
namespace Messaging {
  struct Message;
}

/**
 * \brief 
 */
class RoomSwitchBehavior :
  public Component
{
  glm::vec2 playerpos;
  unsigned room;
  bool roomswitch = false;
  GameObjectManager& manager_;
  std::vector< std::set< ObjectId > >  LEVELOBJECTS;
  float timer = 0.0f;
  float delay = 0.6f;
  bool collided = false;
  float transitionDelay = 0.1f;
  float transitionTimer = 0.0;
  bool ISTransitionActive = false;
  std::vector<int> enemyTotalPerRoom;

  static constexpr const int INVALID_ROOM{ -1 };
  int currentroom = INVALID_ROOM;
  int nextroom = 0;
public:
  RoomSwitchBehavior(GameObject &parent, glm::vec2 pos, unsigned _room, std::vector< std::vector< ObjectId > > _LEVELOBJECTS);
  ~RoomSwitchBehavior();
  virtual Component* Clone_() const { return new RoomSwitchBehavior(*this); }
  void transition();
  virtual void Update(float dt);
  static void CollisionHandle(ObjectId, const Messaging::Message&);
  static void ObjectDelete(ObjectId, const Messaging::Message&);
  static void ObjectHandle(ObjectId, const Messaging::Message&);
  static void EnemyDeathActions(ObjectId, const Messaging::Message&);
  void EnemyDeath(const Messaging::Message& message);
  void ObjectDeath(const Messaging::Message& message);
  void onCollision(const Messaging::Message& message);
  void onObjectCreate(Messaging::ObjectRefVec & objects);
  void changeroom();
  int GetEnemyCount() const { return enemyTotalPerRoom[room -1]; }
  void SetPlayerPos(glm::vec2 _pos) { playerpos = _pos; }
  void SetRoom(int _room) { room = _room; }
  GameObject& find_goal_block();
  

};

