/******************************************************************************/
/*!
  \file    Engine.h
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/29/2017

  \brief
    Interface for the Game's engine
    
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef ENGINE_H
#define ENGINE_H

#include <typeindex>
#include <typeinfo>
#include <vector>
#include <map>
#include <cstddef>
#include <GameStateManager/GameStateManager.h>
#include <GameObject/GameObjectManager.h>
#include <Graphics/Graphics.hpp>
#include <Input/Input.hpp>
#include <Messaging/Messaging.hpp>
#include <Audio/Audio.hpp>
#include <Physics/Physics.hpp>
#include "System.h"
    
class Engine
{
public:

  /***************************************************************************/
  /*!
    Default constructor for the Engine class, this is sufficient since the
    Engine has no private data members
  */
  /***************************************************************************/
  Engine();

  /***************************************************************************/
  /*!
    make sure that the compiler doesn't create a copy constructor for the
    class
  */
  /***************************************************************************/
  Engine(const Engine& o) = delete;

  /***************************************************************************/
  /*!
    Default destructor for the Engine class, this is sufficient since the
    Engine has no private data members
  */
  /***************************************************************************/
  ~Engine() = default;

  /***************************************************************************/
  /*!
    Initialize the game engine

    \returns bool
      returns true if all of the objects were intitialized succecfully, else
      returns false
  */
  /***************************************************************************/
  bool Init();

  /***************************************************************************/
  /*!
    Update Each system connected to the engine
  */
  /***************************************************************************/
  void Update(float dt);

  /***************************************************************************/
  /*!
    shutdown the game engine
  */
  /***************************************************************************/
  void Shutdown();
  
  /***************************************************************************/
  /*!
    \return
      returns whether the gsm is running
  */
  /***************************************************************************/
  bool IsRunning();
  
  /**************************************************************************/
  /*!
	Takes a pointer to a system (so it doesn't slice the object) and
	stores it in an unordered_map so its easily accesible

	\param component
	the component that is being added into the vector

	\return
	return a refference to the componenet found
  */
  /**************************************************************************/
  template<typename T>
  System& Add(T* system);

  /***************************************************************************/
  /*!
	index into a map of systems and return the system at typeindex <T>

	\return
		the component that was found
  */
  /***************************************************************************/
  template<typename T>
  T* Find();
  

  KPE::PhysicsEngine* GetPhysicsEngine();

  /***************************************************************************/
  /*!
  this frame's delta-time

  \return
  dt member
  */
  /***************************************************************************/
  float dt() const;

  /***************************************************************************/
  /*!
  returns the total runtime of the engine in seconds

  \return
  total_time_ member
  */
  /***************************************************************************/
  float runTime() const;


  void Quit();

private:
  bool isRunning_ = true;
  float dt_;         // current-frame delta
  float total_time_; // runtime of the engine
	std::vector<System*> Systems_;
	// a container for all of the systems in the engine
	std::map<std::type_index, size_t> Index_;
	KPE::PhysicsEngine * physicsEngine_;
};

extern Engine engine;

template<typename T>
System& Engine::Add(T* system) {
	Systems_.push_back(system);
	Index_[std::type_index(typeid(T))] = Systems_.size() - 1;
	return *this->Systems_;
}

// find the component of Type type, but const
template<typename T>
T * Engine::Find() {
	return static_cast<T*>(Systems_[Index_[std::type_index(typeid(T))]]);
}

#endif //ENGINE_H