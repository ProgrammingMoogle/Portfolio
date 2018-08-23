/*************************************************************************/
/*!
\file   EnemyDoorBehavior.cpp
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
#include "EnemyDoorBehavior.h"
#include "GameObject/GameObject.h"
#include "GameStateManager/GameStateManager.h"
#include "Engine/Engine.h"
#include "RoomSwitchBehavior.h"


/**
 * \brief 
 * \param parent 
 */
EnemyDoorBehavior::EnemyDoorBehavior(GameObject &parent): Component(parent), manager_(engine.Find<GameStateManager>()->CurrentState()->Manager())
{
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();
  m_handler->Register(Messaging::ID::RoomClear, key, EnemyDoorBehavior::DeleteDoor);
}


/**
 * \brief 
 */
EnemyDoorBehavior::~EnemyDoorBehavior()
{
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();
  m_handler->Unregister(Messaging::ID::RoomClear, key);
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void EnemyDoorBehavior::DeleteDoor(ObjectId obj, const Messaging::Message& message)
{
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);

  if (object.IsActive())
  {
    object.Find<EnemyDoorBehavior>()->ondelete();
  }
}

void EnemyDoorBehavior::ondelete()
{
  GetParent().Destroy();
}

void EnemyDoorBehavior::Update(float dt)
{
#if 0
  if (GetParent().IsActive())
  {
    try {
      GameObject&  Listener = manager_.Find("SwitchListener");

      int EnemyCount = Listener.Find<RoomSwitchBehavior>()->GetEnemyCount();

      if (EnemyCount == 0)
   
        GetParent().Destroy();
      
    }
    catch (int)
    {
      //do nothing
    }
  }
#endif
}
