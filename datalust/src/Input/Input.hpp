/**
  \file   Input.hpp
  \author Taylor Osmond

  \brief Interface for input system.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _INPUT_HPP_
#define _INPUT_HPP_

#include <array>
#include <SFML/Window.hpp>
#include <Messaging/Messaging.hpp>
#include <Engine/System.h>
#include "BulletType.h"
#include <map>

namespace Input{
  ///Command base class
  class Command{
    public:
      virtual ~Command() {} ///< virtual destructor
	  virtual void Pressed(Messaging::MessageHandler* handler) = 0; ///< pressed command
      virtual void Held(Messaging::MessageHandler* handler) = 0; ///< held command
	  virtual void Released(Messaging::MessageHandler* handler) = 0; ///< released command
  };

  ///Test command class
  class TestCommand : public Command{
  public:
	virtual void Pressed(Messaging::MessageHandler* handler); ///<sends test message
    virtual void Held(Messaging::MessageHandler* handler); ///< sends test message
	virtual void Released(Messaging::MessageHandler* handler); ///< sends test message
  };

  //COLOR ENUM
  //enum COLOR {RED, BLUE}; 0 == RED 1 === BLUE
  ///Attck Command
  class AttackCommand : public Command {
  public:
	  AttackCommand(int player, BulletType c);
	  virtual void Pressed(Messaging::MessageHandler* handler); ///<sends test message
	  virtual void Held(Messaging::MessageHandler* handler); ///< sends test message
	  virtual void Released(Messaging::MessageHandler* handler); ///< sends test message
    BulletType GetColor() { return  _color; }
  private:
	  int player_;
    BulletType _color;
  };



  ///Move left command class
  class MoveLeftCommand : public Command{
  public:
    MoveLeftCommand(int player);
	virtual void Pressed(Messaging::MessageHandler* handler); ///<sends move left (pressed) message
    virtual void Held(Messaging::MessageHandler* handler); ///< sends move left (held) message
	virtual void Released(Messaging::MessageHandler* handler); ///< sends move left (released) message
  private:
    int player_; ///< the player sending the command
  };

  ///Move right command class
  class MoveRightCommand : public Command{
  public:
    MoveRightCommand(int player);
	virtual void Pressed(Messaging::MessageHandler* handler); ///< sends move right (Pressed) message
    virtual void Held(Messaging::MessageHandler* handler); ///< sends move right (held) message
	virtual void Released(Messaging::MessageHandler* handler); ///< sends move right (Released) message
  private:
    int player_; ///< the player sending the command
  };

  ///Move left command class
  class MoveUpCommand : public Command {
  public:
    MoveUpCommand(int player);
    virtual void Pressed(Messaging::MessageHandler* handler); ///<sends move left (pressed) message
    virtual void Held(Messaging::MessageHandler* handler); ///< sends move left (held) message
    virtual void Released(Messaging::MessageHandler* handler); ///< sends move left (released) message
  private:
    int player_; ///< the player sending the command
  };

  ///Move left command class
  class MoveDownCommand : public Command {
  public:
    MoveDownCommand(int player);
    virtual void Pressed(Messaging::MessageHandler* handler); ///<sends move left (pressed) message
    virtual void Held(Messaging::MessageHandler* handler); ///< sends move left (held) message
    virtual void Released(Messaging::MessageHandler* handler); ///< sends move left (released) message
  private:
    int player_; ///< the player sending the command
  };


  ///Menu Move Up command class
  class MenuUpCommand : public Command{
  public:
	virtual void Pressed(Messaging::MessageHandler* handler); ///< sends menu up (pressed) message
    virtual void Held(Messaging::MessageHandler* handler); ///< sends menu up (held) message
	virtual void Released(Messaging::MessageHandler* handler); ///< sends menu up (released) message
  };

  ///Menu Move Down command class
  class MenuDownCommand : public Command{
  public:
	  virtual void Pressed(Messaging::MessageHandler* handler); ///< sends menu down (pressed) message
	  virtual void Held(Messaging::MessageHandler* handler); ///< sends menu down (held) message
	  virtual void Released(Messaging::MessageHandler* handler); ///< sends menu down (released) message
  };

  ///Menu Move Left command class
  class MenuLeftCommand : public Command{
  public:
	  virtual void Pressed(Messaging::MessageHandler* handler); ///< sends menu left (pressed) message
	  virtual void Held(Messaging::MessageHandler* handler); ///< sends menu left (held) message
	  virtual void Released(Messaging::MessageHandler* handler); ///< sends menu left (released) message
  };

  ///Menu Move Right command class
  class MenuRightCommand : public Command{
  public:
	  virtual void Pressed(Messaging::MessageHandler* handler); ///< sends menu right (pressed) message
	  virtual void Held(Messaging::MessageHandler* handler); ///< sends menu right (held) message
	  virtual void Released(Messaging::MessageHandler* handler); ///< sends menu right (released) message
  };

  ///Menu Select command class
  class MenuSelectCommand : public Command{
  public:
	  virtual void Pressed(Messaging::MessageHandler* handler); ///< sends menu select (pressed) message
	  virtual void Held(Messaging::MessageHandler* handler); ///< sends menu select (held) message
	  virtual void Released(Messaging::MessageHandler* handler); ///< sends menu select (released) message
  };

  ///Menu Back command class
  class MenuBackCommand : public Command{
  public:
	  virtual void Pressed(Messaging::MessageHandler* handler); ///< sends menu back (pressed) message
	  virtual void Held(Messaging::MessageHandler* handler); ///< sends menu back (held) message
	  virtual void Released(Messaging::MessageHandler* handler); ///< sends menu back (released) message
  };

  class DebugCommand : public Command {
  public:
	  virtual void Pressed(Messaging::MessageHandler* handler); ///< sends menu back (pressed) message
	  virtual void Held(Messaging::MessageHandler* handler); ///< sends menu back (held) message
	  virtual void Released(Messaging::MessageHandler* handler); ///< sends menu back (released) message
  };

  class P1Win : public Command {
  public:
	  virtual void Pressed(Messaging::MessageHandler* handler); ///< sends menu back (pressed) message
	  virtual void Held(Messaging::MessageHandler* handler); ///< sends menu back (held) message
	  virtual void Released(Messaging::MessageHandler* handler); ///< sends menu back (released) message
  };

  class ToggleFSCommand : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };

  class Pause : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };

  //Cheat inputs

  class CheatF1 : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };
  class CheatF2 : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };
  class CheatF3 : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };
  class CheatF4 : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };
  class CheatF5 : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };
  class CheatF6 : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };
  class CheatF7 : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };

  //Pause Menu Options
  class Menu : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };
  class Quit : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };
  class Confirm : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };
  class Deny : public Command {
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };

  class RestartLevel : public Command{
  public:
    virtual void Pressed(Messaging::MessageHandler *handler);
    virtual void Held(Messaging::MessageHandler *handler);
    virtual void Released(Messaging::MessageHandler *handler);
  };


  struct controller {
    std::array<Command*, ( size_t )sf::Joystick::ButtonCount> buttons;
    std::array<std::pair<Command*, Command*>, ( size_t )sf::Joystick::AxisCount> axis;
    std::array<bool, ( size_t )sf::Joystick::ButtonCount> buttons_states;
    std::array<std::pair<bool, bool>, ( size_t )sf::Joystick::AxisCount> axis_states;
    static float deadzone;
  };

  ///Input handler 
  class InputHandler : public System{
  public:
    InputHandler();           ///< Constructor
    bool Init();              ///< Init function
    void Update(float dt);    ///< Update function
    void Shutdown();          ///< Shutdown function
    void GetInput(sf::Event); ///< Gets Input
    ~InputHandler();
  private:
    //JFI: because I dont need to make code count :P
    std::map<sf::Keyboard::Key, std::pair<Command*, bool> > _minputs;

    std::map<sf::Mouse::Button, std::pair<Command*, bool> > _mMouse;
    
    std::pair<Command*, bool>& FindCommand(sf::Keyboard::Key type);
    std::pair<Command*, bool>& FindCommand(sf::Mouse::Button type);

    //std::array<Command*, (size_t)sf::Keyboard::Key::KeyCount> inputs_; 

    ///array of bools, indexed by the sfml key enum. (true = held, false = not held)
   // std::array<bool, (size_t)sf::Keyboard::Key::KeyCount> key_held_states_;

    //same as above but for mouse
    //std::array<Command*, (size_t)sf::Mouse::Button::ButtonCount> mouse_;

    //std::array<bool, (size_t)sf::Mouse::Button::ButtonCount> mouse_held_states_;

    
    //TestCommand* _tc;
    controller controller1;

    int joy1 = -1;

	///message handler
    Messaging::MessageHandler *handler_;
  };
}

#endif