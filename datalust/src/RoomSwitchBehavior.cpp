/*************************************************************************/
/*!
\file   RoomSwitchBehavior.cpp
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
#include "RoomSwitchBehavior.h"
#include "GameObject/GameObject.h"
#include "Messaging/Messaging.hpp"
#include "Engine/Engine.h"
#include <Transform/Transform.hpp>
#include "RSwitchComponent.h"
#include <boost/mpl/begin_end.hpp>
#include "Graphics/Camera.hpp"
#include "GameObject/Component.h"

/**
 * \brief 
 * \param parent 
 * \param pos 
 * \param _room 
 * \param _LEVELOBJECTS 
 */
RoomSwitchBehavior::RoomSwitchBehavior(GameObject &parent, glm::vec2 pos, unsigned _room, std::vector< std::vector< ObjectId > > _LEVELOBJECTS) : Component(parent), playerpos(pos), room(_room), manager_(engine.Find<GameStateManager>()->CurrentState()->Manager())
{
  LEVELOBJECTS.reserve(_LEVELOBJECTS.size());
  for (auto && level : _LEVELOBJECTS)
  {
    LEVELOBJECTS.emplace_back();
    for (auto && object : level)
      LEVELOBJECTS.back().emplace(object);
  }

  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();
  m_handler->Register(Messaging::ID::Collision, key, RoomSwitchBehavior::CollisionHandle);

  m_handler->Register(Messaging::ID::EnemyDeath, key, RoomSwitchBehavior::EnemyDeathActions);
  m_handler->Register(Messaging::ID::ObjectCreated, key, RoomSwitchBehavior::ObjectHandle);
  m_handler->Register(Messaging::ID::ObjectDestroyed, key, RoomSwitchBehavior::ObjectDelete);

  for (auto && level : LEVELOBJECTS)
  {
    int enemycount = 0;
    auto begin = level.begin();
    for (; begin != level.end(); )
    {
      try
      {
        if (manager_.Find(*begin).GetType() == "Enemy")
        {
          ++enemycount;
        }
        ++begin;

      }
      catch (int)
      {
        level.erase(begin++);
      }
    }
    enemyTotalPerRoom.push_back(enemycount);

  }
}


/**
 * \brief 
 */
RoomSwitchBehavior::~RoomSwitchBehavior()
{
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();
  m_handler->Unregister(Messaging::ID::Collision, key);

  m_handler->Unregister(Messaging::ID::EnemyDeath, key);

  m_handler->Unregister(Messaging::ID::ObjectCreated, key);
  m_handler->Unregister(Messaging::ID::ObjectDestroyed, key);

}

/**
 * \brief 
 */
void RoomSwitchBehavior::transition()
{
  transitionTimer = transitionDelay;

  ISTransitionActive = true;

  //start transition

  std::vector<std::reference_wrapper<GameObject>> trans;

  trans = ObjectFactory::Create("data/json/TransitionParticles.json", manager_);

  glm::vec2 centerofscreen(Graphics::Camera::getActiveCamera()->getCenter());

  trans[0].get().Find<Transform>()->setPosition(centerofscreen);

}

/**
 * \brief 
 * \param dt 
 */
void RoomSwitchBehavior::Update(float dt)
{

  if (GetParent().IsActive())
  {
    collided = false;

    if (currentroom != nextroom) //&& !ISTransitionActive)
    {
#if 0
    timer -= dt;
      if (timer > 0)
      {
        nextroom = currentroom;
      }
      else
      {

        if (!ISTransitionActive)
          transition();
        else
          transitionTimer -= dt;

        if (transitionTimer <= 0.0f)}
#endif
        changeroom();

        currentroom = nextroom;
      }
    }
  
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void RoomSwitchBehavior::CollisionHandle(ObjectId obj, const Messaging::Message& message)
{
  if (!(message.message.collision.body_->GetParent().GetType() == "Player" && message.message.collision.otherBody_->GetParent().GetType() == "RoomChange"))
    return;
  engine.Find<GameStateManager>()->CurrentState()->Manager().Find(obj).Find<RoomSwitchBehavior>()->onCollision(message);
}

/**
 * \brief 
 * \param id 
 * \param message 
 */
void RoomSwitchBehavior::ObjectDelete(ObjectId id, const Messaging::Message &message)
{
    GameObject & obj = engine.Find<GameStateManager>()->CurrentState()->Manager().Find(id);

  if (obj.GetType() != "SwitchListener")
    return;

  obj.Find<RoomSwitchBehavior>()->ObjectDeath(message);
}

/**
 * \brief 
 * \param id 
 * \param message 
 */
void RoomSwitchBehavior::ObjectHandle(ObjectId id, const Messaging::Message &message)
{
  GameObject & obj = engine.Find<GameStateManager>()->CurrentState()->Manager().Find(id);

  if (obj.GetType() != "SwitchListener")
    return;

  obj.Find<RoomSwitchBehavior>()->onObjectCreate(*message.message.obj_created.createdObjects);
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void RoomSwitchBehavior::EnemyDeathActions(ObjectId obj, const Messaging::Message& message)
{
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);

  if (object.GetType() == "SwitchListener")
  {
    object.Find<RoomSwitchBehavior>()->EnemyDeath(message);
  }
}

/**
 * \brief 
 * \param message 
 */
void RoomSwitchBehavior::EnemyDeath(const Messaging::Message& message)
{
  --enemyTotalPerRoom[currentroom];

  if (enemyTotalPerRoom[currentroom] <= 0)
  {
    //enemy clear event
    Messaging::Message m;
    m.id = Messaging::ID::RoomClear;
    engine.Find<Messaging::MessageHandler>()->Post(m);
  }

}

/**
 * \brief 
 * \param message 
 */
void RoomSwitchBehavior::ObjectDeath(const Messaging::Message & message)
{
  if (currentroom == INVALID_ROOM) return;
  auto find  = LEVELOBJECTS[currentroom].find(message.message.obj_created.createdObjects->front().get().GetKey());
  if (find != LEVELOBJECTS[currentroom].end())
    LEVELOBJECTS[currentroom].erase(find);
}

/**
 * \brief 
 * \param message 
 */
void RoomSwitchBehavior::onCollision(const Messaging::Message& message)
{

  if (collided) return;

  collided = true;

  GameObject& player = message.message.collision.body_->GetParent();


  GameObject& rswitch = message.message.collision.otherBody_->GetParent();

  RSwitchComponent* r = rswitch.Find<RSwitchComponent>();

  SetPlayerPos(r->GetPlayerPos());
  (*message.message.collision.pass_) = false;

  Transform* t = player.Find<Transform>();
  t->setPosition(playerpos);

  nextroom = r->GetRoom() - 1;
}

/**
 * \brief 
 * \param objects 
 */
void RoomSwitchBehavior::onObjectCreate(Messaging::ObjectRefVec & objects)
{
  if (currentroom == INVALID_ROOM) return;
  for (auto && refObj : objects)
    LEVELOBJECTS[currentroom].emplace(refObj.get().GetKey());
}

/**
 * \brief 
 */
void RoomSwitchBehavior::changeroom() {
  typedef std::vector<ObjectId>::iterator ObjIter;


  //first deactivate gameobjects and clean up
  if (currentroom != INVALID_ROOM)
  {
    std::set<ObjectId>& currlevel = LEVELOBJECTS[currentroom];
    
    auto begin = currlevel.begin();
    for (; begin != currlevel.end();)
    {
      try
      {
        manager_.Find(*begin).DeActivate();
        ++begin;
      }
      catch (int)
      {
        currlevel.erase(begin++);
      }
    }
  }


  std::set<ObjectId>& nextlevel = LEVELOBJECTS[nextroom];
  auto begin = nextlevel.begin();
  for (; begin != nextlevel.end();)
  {
    int enemycount = 0;
    try
    {
      manager_.Find(*begin).Activate();
      ++begin;
    }
    catch (int)
    {
      nextlevel.erase(begin++);
    }
  }

  //timer = delay;
}

/**
 * \brief 
 * \return 
 */
GameObject& RoomSwitchBehavior::find_goal_block()
{
  static GameObject empty{"Empty", "Empty"};

  auto level = LEVELOBJECTS.rbegin();
  for (; level != LEVELOBJECTS.rend(); ++level)
  {
    for (auto begin = level->begin(); begin != level->end();/*no inc*/)
    {
      int enemycount = 0;
      try
      {
        GameObject & obj = manager_.Find(*begin);
        if (obj.GetName() == "GoalBlock")
          return obj;
        ++begin;
      }
      catch (int)
      {
        level->erase(begin++);
      }
    }
  }

  return empty;
}
