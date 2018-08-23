/******************************************************************************/
/*!
  \file    GameObjectManager.h
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/07/2017

  \brief
    Interface of the GameObjectManager class
    
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H

#include <list>
#include <Engine/System.h>
#include <Util/Util.hpp>
#include <Messaging/Messaging.hpp>
#include "SlotMap.h"
#include "GameObject.h"

class GameState;

class GameObjectManager : public System
{
public:
	void GameObjectManager::DelaydDestruction(ObjectId key, float time);

  /***************************************************************************/
  /*!
      Default constructor for the GameObjectManager class
  */
  /***************************************************************************/
  GameObjectManager() = default;
  /***************************************************************************/
  /*!
      Copy constructor for the GameObjectManager class (doesn't exist)
  */
  /***************************************************************************/
  GameObjectManager(const GameObjectManager& o) = delete;
  
  /***************************************************************************/
  /*!
      Destructor for the GameObjectManager class loops throught the SlotMap
      calls the destructor for the SlotMap class implicityly, which calls the
      destructor for vector, which calls the destructor on each GameObject 
      inside the manager 
  */
  /***************************************************************************/
  ~GameObjectManager() = default;
  
  /***************************************************************************/
  /*!
      Update all of the GameObject in the ObjectManager
      
      \param float
        dt the time since last update or more likely a fixed value
  */
  /***************************************************************************/
  void Update(float dt);

  /***************************************************************************/
  /*!
      const lvalue ref version of the Add function bellow
      
      \param GameObject
        the object being added to the Manager
      
      \return
        return a refference t the GameObject that was added
  */
  /***************************************************************************/
  GameObject& Add(GameObject& obj);
  
  /***************************************************************************/
  /*!
      Add a GameObject Reff into the GameObject Managers slot map, returns a 
      refference so this call can be chained
      
      \param GameObject
        the object being added to the Manager
      
      \return
        return a refference t the GameObject that was added
  */
  /***************************************************************************/
  GameObject& Add(GameObject&& obj);

  /***************************************************************************/
  /*!
      Get an Object by name
      
      \param std::string
        the name of the object
      
      \return
        a refference to the object found, will throw if object wasn't found
  */
  /***************************************************************************/
  GameObject& Find(std::string name);

  /***************************************************************************/
  /*!
		Get an Object by ObjectId

		\param id
			the id of the object

		\return
			a refference to the object found, will throw if object wasn't found
  */
  /***************************************************************************/
  GameObject& Find(ObjectId id);

  /***************************************************************************/
  /*!
		Loops through and destroys all of the objects
  */
  /***************************************************************************/
  void Shutdown();

  /***************************************************************************/
  /*!
		Loops through and Serializes all of the objects
  */
  /***************************************************************************/
  void Serialize(const char* filename);

  /***************************************************************************/
  /*!
		Loops through and ataches a message handler to the apropriate objects
		all of the objects
  */
  /***************************************************************************/
  void RegisterMsg(void (*Register)(ObjectId, Messaging::MessageHandler *m_handler), Messaging::MessageHandler *m_handler);
  void UnRegisterMsg(void(*UnRegister)(ObjectId, Messaging::MessageHandler *m_handler), Messaging::MessageHandler *m_handler);

private:
  // the container that holds all of the object
  SlotMap objectManager_;
  // a container that holds keys into the SlotMap
  std::list<ObjectId> keyList_;
  // a list that contains all of the objects that were created while the 
  // manager was updating in order to delay the addition of the object
  std::list<GameObject> delayedCreation_;
  std::list <std::pair<ObjectId, float>> delayedDestruction_;
  // flag to prevent items being added while the manager is updating
  bool isUpdating_ = false;
};

#endif //GAMEOBJECTMANAGER_H