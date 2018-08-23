/******************************************************************************/
/*!
  \file   Behaviors.hpp
  \author Sarah Janzen
  \par    c.janzen\@digipen.edu
  \date   9/20/17
  \brief
    Behaviors class: inherits from Component

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef _BEHAVIORS_HPP_
#define _BEHAVIORS_HPP_
#include <Messaging/Messaging.hpp>
#include <GameObject/Component.h>
#include <GameObject/GameObject.h>

// Debug features for working with lua files; only use for debug
namespace BehaviorsDebug
{
  /**
    \breif Detects if the given lua script (must supply path
      beginning in project directory to lua file in lua file name)
      prints to stdout with specifics if the given lua file has memory allocation errors
      prints to stdout with specifics if the given lua file has syntax errors in it
      (it will also print syntax errors if it can't find the file;
       use BehaviorsDebug::PrintFile() to check if the given file name is valid)
  */
  bool LuaHasErrors(std::string const& lua_file_name);
  /**
    \breif given a filename, prints the contents of the file to stdout;
      way of detecting if path/filename is valid; uses system call;
      only use for debugging
  */
  void PrintFile(std::string const& file_name);
  /**
    \breif waits for user to press any key; uses system call; only use for debugging
  */
  void press_anykey(void);

  /**
    \breif 
      output operator for a vector of Messaging::ID
      will look something like <0,1,2,3,5,7,10>
  */
  std::ostream &operator<<(std::ostream & os, std::vector<Messaging::ID> const& msg_id_types);
}

class Behaviors : public Component
{
public:
  /**
    \breif 
      creates an empty behaviors component; 
      will not listen for anything until Add() is called;
  */
  Behaviors(GameObject& parent);

  /**
  \breif basic ctor, register listeners for Behaviors
  */
  Behaviors(GameObject& parent, std::vector<Messaging::ID> const& msg_id_types);

  /**
    \breif 
      creates listeners for the given message id types
      assumes that user will not try to add more than 
        one of the same type to one object
  */
  void Add(std::vector<Messaging::ID> const& msg_id_types);

  /**
    \breif 
      creates a listener for the given message id type
      throws an exception if trying to add a duplicate type
  */
  void Add(Messaging::ID const& msg_id_type);

  /**
    \breif 
      removes listeners for the given message id types
      assumes that user will not try to remove a type that 
      does not exist
  */
  void Remove(std::vector<Messaging::ID> const& msg_id_types);

  /**
    \breif 
      removes listener for the given message id type
      throws an exception if trying to 
  */
  void Remove(Messaging::ID const& msg_id_type);

  /**
    \breif dtor, unregisters all used message listeners
  */
  ~Behaviors(void);

  /**
    \breif required by Component class; does nothing
  */
  void Update(float dt);

  /**
    \breif required by Component class; does nothing
  */
  void Draw(void);

  /**
    \breif copy ctor for the Behavior class
  */
  Behaviors(Behaviors const& rhs) : Component(rhs), msg_id_types_(rhs.msg_id_types_) {}

private:
  //Messaging::MessageHandler msg_handler_; ///< Handler to intercept messages from Messaging
  std::vector<Messaging::ID> msg_id_types_; ///< Message ID types that will be regigistered/unregistered for this object

  /**
    \breif clone wars
  */
  Component* Clone_() const;

  //helper functions

  /**
  \breif Registers ALL necessary message listeners
  */
  void registerListeners_(std::vector<Messaging::ID> const& msg_id_types);  //cannot be marked const b/c GetKey() is not const
  /**
    \breif Unregisters ALL used message listeners
  */
  void unregisterListeners_(std::vector<Messaging::ID> const& msg_id_types); //cannot be marked const b/c GetKey() is not const
};

//testing
namespace SarahSandbox
{
  /**
    \breif function to test various functionalities of the Behavior class
  */
  void BehaviorsTest(void);
}

#endif
