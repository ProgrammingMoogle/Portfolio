/**
  \file   Messaging.hpp
  \author Taylor Osmond

  \brief Interface for message passing system.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _MESSAGING_HPP_
#define _MESSAGING_HPP_

#include <vector>
#include <functional>
#include <array>
#include <list>
#include <utility>
#include "GameObject/GameObject.h"
#include <GameObject/SlotMap.h>
#include <Engine/System.h>
#include <Graphics/Sprite.hpp>
#include <Physics/RigidBody.hpp>
#include <Physics/BoundArea.hpp>
namespace Messaging {
  ///IDs of all message types
  enum class ID {
    //Add IDs Here//
    Test,       ///< Test ID
    //Key inputs
    MoveLeft,   ///< Move Left ID
    MoveRight,  ///< Move Right ID
    MoveUp,
    MoveDown,
    Jump,       ///< Jump ID
    SJump,
    Attack,		  ///< Attck ID
    MenuUp,     ///< Menu Up ID
    MenuDown,   ///< Menu Down ID
    MenuLeft,   ///< Menu Left ID
    MenuRight,  ///< Menu Right ID
    MenuSelect, ///< Menu Select ID
    MenuBack,   ///< Menu Back ID
    WindowClose, ///< User pressed the [X] in the top right corner
    //Other
    AnimationUpdate, ///< An animation updated
    Debug, ///< Debug message
    Collision,
    Damage,
    P1Win,
	  Pause,
    PauseRender,
    CheatF1,
    CheatF2,
    CheatF3,
    CheatF4,
    CheatF5,
    CheatF6,
    CheatF7,
    ObjectCreated,
    ObjectDestroyed,
    EnemyDeath,
    RoomClear,
    LostFocus,
    Menu,
    Quit,
    Confirm,
    Deny,
    Reset,
    Last       ///< The final ID, used in array creation
  };

  ///Key state for inputs
  enum class KeyState {
    Pressed,  ///< Pressed state
    Held,     ///< Held state
    Released, ///< Released state
  };

  enum  class MouseState
  {
    Pressed,  ///< Pressed state
    Held,     ///< Held state
    Released, ///< Released state
  };
  //make yo onion structs here??

  ///Test struct containing an int
  struct Test {
    int i;
    KeyState key_state;
  };

  struct Attack {
    int player;
    MouseState Mouse_state;
    unsigned int color;
  };

  ///struct containing float value of control stick for left
  struct MoveLeft {
    float control_stick_value;
    KeyState key_state;
    int player;
  };

  ///struct containing float value of control stick for right
  struct MoveRight {
    float control_stick_value;
    KeyState key_state;
    int player;
  };

  struct Pause {
    KeyState key_state;
  };

  /// pause rendering event
  struct PauseRender
  {
    bool isPaused;
  };

  ///struct containing float value of control stick for right
  struct MoveUp {
    float control_stick_value;
    KeyState key_state;
    int player;
  };

  ///struct containing float value of control stick for right
  struct MoveDown {
    float control_stick_value;
    KeyState key_state;
    int player;
  };

  ///struct for jump command
  struct Jump {
    KeyState key_state;
    int player;
  };

  ///struct for jump command
  struct SJump {
    KeyState key_state;
    int player;
  };

  ///struct for menu up command
  struct MenuUp {
    KeyState key_state;
  };

  ///struct for menu down command
  struct MenuDown {
    KeyState key_state;
  };

  ///struct for menu left command
  struct MenuLeft {
    KeyState key_state;
  };

  ///struct for menu right command
  struct MenuRight {
    KeyState key_state;
  };

  ///struct for menu select command
  struct MenuSelect {
    KeyState key_state;
  };

  ///struct for menu back command
  struct MenuBack {
    KeyState key_state;
  };

  struct AnimationUpdate {
    Graphics::Sprite * sprite;
    const char * animation;
    int frame;
    bool is_playing;
  };

  struct Damage {
    ObjectId* Player;
  };

  struct P1Win {
    KeyState key_state;
  };
  struct Debug {
    KeyState key_state;
    int function;
  };

  struct Collision {
    bool hit_;
    bool* pass_;
    float penetration_;
    glm::vec2 normal_;
    KPE::RigidBody* body_;      //Hit Body
    KPE::RigidBody* otherBody_; //Hurt Body
  };


  //Cheat Keys
  struct CheatF1 {
    KeyState key_state;
  };
  struct CheatF2 {
    KeyState key_state;
  };
  struct CheatF3 {
    KeyState key_state;
  };
  struct CheatF4 {
    KeyState key_state;
  };
  struct CheatF5 {
    KeyState key_state;
  };
  struct CheatF6 {
    KeyState key_state;
  };
  struct CheatF7 {
    KeyState key_state;
  };

  //Pause Menu Options
  struct Menu {
    KeyState key_state;
  };
  struct Quit {
    KeyState key_state;
  };
  struct Confirm {
    KeyState key_state;
  };
  struct Deny {
    KeyState key_state;
  };


  typedef std::vector<std::reference_wrapper<GameObject>> ObjectRefVec;
  struct ObjectsCreated
  {
    ObjectRefVec *createdObjects;
  };



  ///A message containing an ID and a message
  struct Message {
    ID id; ///< the enum ID of the message

    ///union of the messages to be sent
    union {
      //put yo onion structs here//
      MoveLeft move_left;
      MoveRight move_right;
      MoveUp move_up;
      MoveDown move_down;
      Jump jump;
      SJump sjump;
      Attack attack;
      MenuUp menu_up;
      MenuDown menu_down;
      MenuLeft menu_left;
      MenuRight menu_right;
      MenuSelect menu_select;
      MenuBack menu_back;
      AnimationUpdate animation_update;
      P1Win win;
      Debug debug;
      Collision collision;
      Damage damage;
      Test test;
      //PauseRender render_pause;
	    Pause pause;
      CheatF1 cheatF1;
      CheatF1 cheatF2;
      CheatF1 cheatF3;
      CheatF1 cheatF4;
      CheatF1 cheatF5;
      CheatF1 cheatF6;
      CheatF1 cheatF7;
      Menu menu;
      Quit quit;
      Confirm confirm;
      Deny deny;
      ObjectsCreated obj_created;
    } message;
  };

  ///Contains the Post, Register, and Unregister functions, as well as the callbacks_ array
  class MessageHandler : public	System {
  public:
    /// Post a user created message to be handled
    void Post(Message msg);

    /// Register a user with the messaging system (handler cannot be NULL)
    void Register(ID id, ObjectId obj, void(*handler)(ObjectId, const Message&));

    /// Unregister a user from the messageing system
    void Unregister(ID id, ObjectId obj);
  private:
    ///An array of lists of callback functions and the objects to call them on, indexable by message ID
    std::array<std::list<std::pair<ObjectId, void(*)(ObjectId, const Message&)>>, (size_t)ID::Last> callbacks_;
  };
}

#endif //Messaging