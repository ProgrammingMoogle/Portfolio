/******************************************************************************/
/*!
  \file    GameObjectManager.cpp
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/07/2017

  \brief
    definition of the GameObjectManager class

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <fstream>
#include "Engine/Engine.h"
#include "Messaging/Messaging.hpp"
#include "GameObjectManager.h"
#include "ObjectFactory.h"

// delay the destruction of objects
void GameObjectManager::DelaydDestruction(ObjectId const key, float time)
{
  delayedDestruction_.emplace_back(key, time);
}

void GameObjectManager::Update(float dt)
{
  // set the isUpdating flag to be true so that objects cannot be added to the 
  // GameObjectManager untill the it has finished
  isUpdating_ = true;
  // loop through the list of keys into the SlotMap of GameObjects and update each object
  keyList_.erase(std::remove_if(keyList_.begin(), keyList_.end(), [&](ObjectId key) {
    try
    {
      GameObject& update = objectManager_.get_object(key);
      // attempt to grab the object at pos 
      // if it was a success update the object
      update.Update(dt);
      return false;
    }
    catch (int)
    {
      // if the object didn't exist or the key had bad data get rid of the key
      return true;
    }
  }), keyList_.end());

  // set the isUpdating_ flag to be false
  isUpdating_ = false;

  // loop through the delayed destruction list and decrement the time for each object
  if (!delayedDestruction_.empty())
  {
    delayedDestruction_.erase(std::remove_if(delayedDestruction_.begin(), delayedDestruction_.end(), [&](std::pair<ObjectId, float>& obj) {
      // decrement dt from the pair 	
      obj.second -= dt;
      // if the dt is less than or equal to 0 then flag the object for destruction
      if (obj.second <= 0)
      {
        objectManager_.get_object(obj.first).Destroy();
        return true;
      }
      return false;
    }), delayedDestruction_.end());
  }

  // loop through the delayed creation loop and add all of the objects whos 
  // addition had been delayed
  if (!delayedCreation_.empty())
  {
    for (auto it = delayedCreation_.begin(); it != delayedCreation_.end(); ++it)
    {
      Add(*it);
    }
    delayedCreation_.clear();
  }

  // now that the list has been updated loop through and destory all of the 
  // objects that have been flaged for destruction

  keyList_.erase(std::remove_if(keyList_.begin(), keyList_.end(), [&](ObjectId key) {
    try
    {
      // check if the GameObject has been flagged for destruction, if it has
      // erase it, if the key is bad delete the key
      if ((objectManager_.get_object(key)).IsDestroyed())
      {
        Messaging::Message m;
        m.id = Messaging::ID::ObjectDestroyed;
        Messaging::ObjectRefVec v{ objectManager_.get_object(key) };
        m.message.obj_created.createdObjects = &v;

        engine.Find<Messaging::MessageHandler>()->Post(m);
        m.message.obj_created.createdObjects;
        objectManager_.Erase(key);
        return true;
      }
    }
    catch (int)
    {
      // if the object didn't exist or the key had bad data get rid of the key
      return true;
    }
    return false;
  }), keyList_.end());
}

// add function that takes a const lvalue reff, calls the lvalue insert
GameObject& GameObjectManager::Add(GameObject & obj)
{
  // if the object is updating insert it into the delyaedCreation container, so
  // it can be added once the GameObjectManager has finished updating
  if (isUpdating_)
  {
    delayedCreation_.push_back(std::move(obj));
    return delayedCreation_.back();
  }
  // else insert the new object into the slot map
  else
  {
    keyList_.push_back(objectManager_.Insert(obj));
    GameObject & ret = objectManager_.get_object(keyList_.back());
    ret.SetObjectId(keyList_.back());

    Messaging::ObjectRefVec created{ ret };
    Messaging::Message m{};
    m.id = Messaging::ID::ObjectCreated;
    m.message.obj_created.createdObjects = &created;
    engine.Find<Messaging::MessageHandler>()->Post(m);

    ret.Start();

    return ret;
  }
}

// add function that takes a rvalue ref, calls the rvalue insert
GameObject& GameObjectManager::Add(GameObject&& obj)
{
  // if the object is updating insert it into the delyaedCreation container, so
  // it can be added once the GameObjectManager has finished updating
  if (isUpdating_)
  {
    // move the object into the list
    delayedCreation_.push_back(std::move(obj));
    return delayedCreation_.back();
  }
  // else insert the new object into the slot map  
  else
  {
    keyList_.push_back(objectManager_.Insert(std::move(obj)));
    GameObject & ret = objectManager_.get_object(keyList_.back());
    ret.SetObjectId(keyList_.back());

    Messaging::ObjectRefVec created{ ret };
    Messaging::Message m{};
    m.id = Messaging::ID::ObjectCreated;
    m.message.obj_created.createdObjects = &created;
    engine.Find<Messaging::MessageHandler>()->Post(m);

    ret.Start();

    return ret;
  }
}

// find an object by name
GameObject& GameObjectManager::Find(std::string name) {
  return objectManager_.get_object(name);
}

GameObject& GameObjectManager::Find(ObjectId id) {
  return objectManager_.get_object(id);
}

void GameObjectManager::Shutdown() {
  objectManager_.Clear();
  keyList_.clear();
  delayedCreation_.clear();
}

void GameObjectManager::Serialize(const char* filename)
{
  std::ofstream outFile(filename, std::fstream::trunc);
  outFile.close();
  for (auto i : keyList_)
  {
    ObjectFactory::Serialize(objectManager_.get_object(i), filename);
  }
}

void GameObjectManager::RegisterMsg(void(*Register)(ObjectId, Messaging::MessageHandler *m_handler),
  Messaging::MessageHandler *m_handler)
{
  std::for_each(keyList_.begin(), keyList_.end(), [&](ObjectId key) {
    GameObject& obj = objectManager_.get_object(key);
    if (obj.PInput())
      Register(key, m_handler);
  });
}

void GameObjectManager::UnRegisterMsg(void(*UnRegister)(ObjectId, Messaging::MessageHandler *m_handler),
  Messaging::MessageHandler *m_handler)
{
  std::for_each(keyList_.begin(), keyList_.end(), [&](ObjectId key) {
    GameObject& obj = objectManager_.get_object(key);
    if (obj.PInput())
      UnRegister(key, m_handler);
  });
}
