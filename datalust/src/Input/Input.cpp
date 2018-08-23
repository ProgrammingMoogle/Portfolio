/**
  \file   Input.cpp
  \author Taylor Osmond

  \brief Implementation of Input system

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Input.hpp"
#include <Messaging/Messaging.hpp>
#include <Engine/Engine.h>
#include "Graphics/Window.hpp"
//#include "BulletType.h"


namespace Input {
  //default inputs array to null
  InputHandler::InputHandler()
  {
    controller1.axis.fill(std::pair<Command*, Command*>(NULL, NULL));
    controller1.axis_states.fill(std::make_pair(false, false));
    controller1.buttons.fill(NULL);
    controller1.buttons_states.fill(false);

    // find connected controllers and assign them to players 1 and 2
    for (int i = 0; i < (size_t)sf::Joystick::Count; ++i)
    {
      if (sf::Joystick::isConnected(i))
      {
        if (joy1 == -1)
          joy1 = i;
      }
    }
  }

  float controller::deadzone = 65.0f;

  //implementation for test pressed
  void TestCommand::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Test;
    msg.message.test.i = 0;
    msg.message.test.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }

  //implementation for test held
  void TestCommand::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Test;
    msg.message.test.i = 1;
    msg.message.test.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }

  //implementation for test released
  void TestCommand::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Test;
    msg.message.test.i = 2;
    msg.message.test.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }

  AttackCommand::AttackCommand(int player, BulletType c) : player_(player), _color(c) {}

  void AttackCommand::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.message.attack.player = player_;
    msg.message.attack.color = _color;
    msg.id = Messaging::ID::Attack;
    msg.message.attack.Mouse_state = Messaging::MouseState::Released;
    handler->Post(msg);
  }

  void AttackCommand::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.message.attack.player = player_;
    msg.message.attack.color = _color;
    msg.id = Messaging::ID::Attack;
    msg.message.attack.Mouse_state = Messaging::MouseState::Held;
    handler->Post(msg);
  }

  void AttackCommand::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.message.attack.player = player_;
    msg.message.attack.color = _color;
    msg.id = Messaging::ID::Attack;
    msg.message.attack.Mouse_state = Messaging::MouseState::Pressed;
    handler->Post(msg);
  }


  //move left constructor implementation
  MoveLeftCommand::MoveLeftCommand(int player) : player_(player) {}

  //implementation for move left pressed
  void MoveLeftCommand::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MoveLeft;
    msg.message.move_left.control_stick_value = 1;
    msg.message.move_left.key_state = Messaging::KeyState::Pressed;
    msg.message.move_left.player = player_;
    handler->Post(msg);
  }

  //implementation for move left held
  void MoveLeftCommand::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MoveLeft;
    msg.message.move_left.control_stick_value = 1;
    msg.message.move_left.key_state = Messaging::KeyState::Held;
    msg.message.move_left.player = player_;
    handler->Post(msg);
  }

  //implementation for move left released
  void MoveLeftCommand::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MoveLeft;
    msg.message.move_left.control_stick_value = 0;
    msg.message.move_left.key_state = Messaging::KeyState::Released;
    msg.message.move_left.player = player_;
    handler->Post(msg);
  }

  void P1Win::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::P1Win;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }

  //implementation for move left held
  void P1Win::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::P1Win;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }

  //implementation for move left released
  void P1Win::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::P1Win;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  //move right constructor implementation
  MoveRightCommand::MoveRightCommand(int player) : player_(player) {}

  //implementation for move right pressed
  void MoveRightCommand::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MoveRight;
    msg.message.move_right.control_stick_value = 1;
    msg.message.move_right.key_state = Messaging::KeyState::Pressed;
    msg.message.move_right.player = player_;
    handler->Post(msg);
  }

  //implementation for move right held
  void MoveRightCommand::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MoveRight;
    msg.message.move_right.control_stick_value = 1;
    msg.message.move_right.key_state = Messaging::KeyState::Held;
    msg.message.move_right.player = player_;
    handler->Post(msg);
  }

  //implementation for move right released
  void MoveRightCommand::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MoveRight;
    msg.message.move_right.control_stick_value = 0;
    msg.message.move_right.key_state = Messaging::KeyState::Released;
    msg.message.move_right.player = player_;
    handler->Post(msg);
  }

  //move up constructor implementation
  MoveUpCommand::MoveUpCommand(int player) : player_(player) {}

  //implementation for move up pressed
  void MoveUpCommand::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MoveUp;
    msg.message.move_up.control_stick_value = 1;
    msg.message.move_up.key_state = Messaging::KeyState::Pressed;
    msg.message.move_up.player = player_;
    handler->Post(msg);
  }

  //implementation for move up held
  void MoveUpCommand::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MoveUp;
    msg.message.move_up.control_stick_value = 1;
    msg.message.move_up.key_state = Messaging::KeyState::Held;
    msg.message.move_up.player = player_;
    handler->Post(msg);
  }

  //implementation for move up released
  void MoveUpCommand::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MoveUp;
    msg.message.move_up.control_stick_value = 0;
    msg.message.move_up.key_state = Messaging::KeyState::Released;
    msg.message.move_up.player = player_;
    handler->Post(msg);
  }

  //move down constructor implementation
  MoveDownCommand::MoveDownCommand(int player) : player_(player) {}

  //implementation for move down pressed
  void MoveDownCommand::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MoveDown;
    msg.message.move_down.control_stick_value = 1;
    msg.message.move_down.key_state = Messaging::KeyState::Pressed;
    msg.message.move_down.player = player_;
    handler->Post(msg);
  }

  //implementation for move down held
  void MoveDownCommand::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MoveDown;
    msg.message.move_down.control_stick_value = 1;
    msg.message.move_down.key_state = Messaging::KeyState::Held;
    msg.message.move_down.player = player_;
    handler->Post(msg);
  }

  //implementation for move down released
  void MoveDownCommand::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MoveDown;
    msg.message.move_down.control_stick_value = 0;
    msg.message.move_down.key_state = Messaging::KeyState::Released;
    msg.message.move_down.player = player_;
    handler->Post(msg);
  }


  //implementation for menu up pressed
  void MenuUpCommand::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuUp;
    msg.message.menu_up.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }

  //implementation for menu up held
  void MenuUpCommand::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuUp;
    msg.message.menu_up.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }

  //implementation for menu up released
  void MenuUpCommand::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuUp;
    msg.message.menu_up.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }

  //implementation for menu down pressed
  void MenuDownCommand::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuDown;
    msg.message.menu_down.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }

  //implementation for menu down held
  void MenuDownCommand::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuDown;
    msg.message.menu_down.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }

  //implementation for menu down released
  void MenuDownCommand::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuDown;
    msg.message.menu_down.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }

  //implementation for menu left pressed
  void MenuLeftCommand::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuLeft;
    msg.message.menu_left.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }

  //implementation for menu left held
  void MenuLeftCommand::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuLeft;
    msg.message.menu_left.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }

  //implementation for menu left released
  void MenuLeftCommand::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuLeft;
    msg.message.menu_left.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }

  //implementation for menu right pressed
  void MenuRightCommand::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuRight;
    msg.message.menu_right.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }

  //implementation for menu right held
  void MenuRightCommand::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuRight;
    msg.message.menu_right.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }

  //implementation for menu right released
  void MenuRightCommand::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuRight;
    msg.message.menu_right.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }

  //implementation for menu select pressed
  void MenuSelectCommand::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuSelect;
    msg.message.menu_select.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }

  //implementation for menu select held
  void MenuSelectCommand::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuSelect;
    msg.message.menu_select.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }

  //implementation for menu select released
  void MenuSelectCommand::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuSelect;
    msg.message.menu_select.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }

  //implementation for menu back pressed
  void MenuBackCommand::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuBack;
    msg.message.menu_back.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }

  //implementation for menu back held
  void MenuBackCommand::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuBack;
    msg.message.menu_back.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }

  //implementation for menu back released
  void MenuBackCommand::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::MenuBack;
    msg.message.menu_back.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }

  void DebugCommand::Pressed(Messaging::MessageHandler * handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Debug;
    msg.message.debug.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void DebugCommand::Held(Messaging::MessageHandler * handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Debug;
    msg.message.debug.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void DebugCommand::Released(Messaging::MessageHandler * handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Debug;
    msg.message.debug.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }


  void Pause::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Pause;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void Pause::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Pause;
    msg.message.win.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void Pause::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Pause;
    msg.message.win.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }


  //*I*************************************************************
  //Cheat Inputs
  //*I*************************************************************
  void CheatF1::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF1;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void CheatF1::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF1;
    msg.message.win.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void CheatF1::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF1;
    msg.message.win.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }
  //Cheat Inputs
  void CheatF2::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF2;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void CheatF2::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF2;
    msg.message.win.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void CheatF2::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF2;
    msg.message.win.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }
  //Cheat Inputs
  void CheatF3::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF3;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void CheatF3::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF3;
    msg.message.win.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void CheatF3::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF3;
    msg.message.win.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }
  //Cheat Inputs
  void CheatF4::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF4;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void CheatF4::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF4;
    msg.message.win.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void CheatF4::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF4;
    msg.message.win.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }
  //Cheat Inputs
  void CheatF5::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF5;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void CheatF5::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF5;
    msg.message.win.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void CheatF5::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF5;
    msg.message.win.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }
  //Cheat Inputs
  void CheatF6::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF6;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void CheatF6::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF6;
    msg.message.win.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void CheatF6::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF6;
    msg.message.win.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }
  //Cheat Inputs
  void CheatF7::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF7;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void CheatF7::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF7;
    msg.message.win.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void CheatF7::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::CheatF7;
    msg.message.win.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }

  //Pause Menu Options
  void Menu::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Menu;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void Menu::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Menu;
    msg.message.win.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void Menu::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Menu;
    msg.message.win.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }
  void Quit::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Quit;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void Quit::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Quit;
    msg.message.win.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void Quit::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Quit;
    msg.message.win.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }
  void Confirm::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Confirm;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void Confirm::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Confirm;
    msg.message.win.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void Confirm::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Confirm;
    msg.message.win.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }
  void Deny::Pressed(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Deny;
    msg.message.win.key_state = Messaging::KeyState::Pressed;
    handler->Post(msg);
  }
  void Deny::Held(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Deny;
    msg.message.win.key_state = Messaging::KeyState::Held;
    handler->Post(msg);
  }
  void Deny::Released(Messaging::MessageHandler* handler)
  {
    Messaging::Message msg;
    msg.id = Messaging::ID::Deny;
    msg.message.win.key_state = Messaging::KeyState::Released;
    handler->Post(msg);
  }


  //initializes all used keys
  bool InputHandler::Init()
  {
    //inputs_[(size_t)sf::Keyboard::Key::T] = new TestCommand;
    //inputs_[(size_t)sf::Keyboard::Key::Up] = new MenuUpCommand;
    //inputs_[(size_t)sf::Keyboard::Key::Down] = new MenuDownCommand;
    //inputs_[(size_t)sf::Keyboard::Key::Return] = new MenuSelectCommand;
    //inputs_[(size_t)sf::Keyboard::Key::P] = new MenuBackCommand;
    //inputs_[(size_t)sf::Keyboard::Key::F1] = new P1Win;
    //inputs_[(size_t)sf::Keyboard::Key::A] = new MoveLeftCommand(1);
    //inputs_[(size_t)sf::Keyboard::Key::D] = new MoveRightCommand(1);
    //inputs_[(size_t)sf::Keyboard::Key::W] = new MoveUpCommand(1);
    //inputs_[(size_t)sf::Keyboard::Key::S] = new MoveDownCommand(1);
    //inputs_[(size_t)sf::Keyboard::Key::Delete] = new DebugCommand;

    //keyboard movement
    _minputs.emplace(sf::Keyboard::Key::T, std::make_pair(new TestCommand, false));
    _minputs.emplace(sf::Keyboard::Key::Up, std::make_pair(new MenuUpCommand, false));
    _minputs.emplace(sf::Keyboard::Key::Down, std::make_pair(new MenuDownCommand, false));
    _minputs.emplace(sf::Keyboard::Key::Return, std::make_pair(new MenuSelectCommand, false));
    _minputs.emplace(sf::Keyboard::Key::X, std::make_pair(new MenuBackCommand, false));
    _minputs.emplace(sf::Keyboard::Key::P, std::make_pair(new Pause, false));
    //_minputs.emplace(sf::Keyboard::Key::F1, std::make_pair(new P1Win, false));
    _minputs.emplace(sf::Keyboard::Key::A, std::make_pair(new MoveLeftCommand(1), false));
    _minputs.emplace(sf::Keyboard::Key::D, std::make_pair(new MoveRightCommand(1), false));
    _minputs.emplace(sf::Keyboard::Key::W, std::make_pair(new MoveUpCommand(1), false));
    _minputs.emplace(sf::Keyboard::Key::S, std::make_pair(new MoveDownCommand(1), false));
    _minputs.emplace(sf::Keyboard::Key::Delete, std::make_pair(new DebugCommand, false));

    //Cheat Options
    _minputs.emplace(sf::Keyboard::Key::F1, std::make_pair(new CheatF1, false));
    _minputs.emplace(sf::Keyboard::Key::F2, std::make_pair(new CheatF2, false));
    _minputs.emplace(sf::Keyboard::Key::F3, std::make_pair(new CheatF3, false));
    _minputs.emplace(sf::Keyboard::Key::F4, std::make_pair(new CheatF4, false));
    _minputs.emplace(sf::Keyboard::Key::F5, std::make_pair(new CheatF5, false));
    _minputs.emplace(sf::Keyboard::Key::F6, std::make_pair(new CheatF6, false));
    _minputs.emplace(sf::Keyboard::Key::F7, std::make_pair(new CheatF7, false));

    //Pause menu options
    _minputs.emplace(sf::Keyboard::Key::M, std::make_pair(new Menu, false));
    _minputs.emplace(sf::Keyboard::Key::R, std::make_pair(new RestartLevel, false));
    _minputs.emplace(sf::Keyboard::Key::Q, std::make_pair(new Quit, false));
    _minputs.emplace(sf::Keyboard::Key::N, std::make_pair(new Deny, false));
    _minputs.emplace(sf::Keyboard::Key::Y, std::make_pair(new Confirm, false));
    _minputs.emplace(sf::Keyboard::Key::N, std::make_pair(new Quit, false));

    //mouse movement
    //mouse_[(size_t)sf::Mouse::Button::Left] = new AttackCommand(1, BulletType::BLUE); 
    //mouse_[(size_t)sf::Mouse::Button::Right] = new AttackCommand(1, BulletType::RED);
    //mouse_[(size_t)sf::Mouse::Button::Middle] = new AttackCommand(1, BulletType::COINS);

    _mMouse.emplace(sf::Mouse::Button::Left, std::make_pair(new AttackCommand(1, BulletType::RED), false)); // O = blue 1 = red 2 = coin
    _mMouse.emplace(sf::Mouse::Button::Right, std::make_pair(new AttackCommand(1, BulletType::BLUE), false));
    _mMouse.emplace(sf::Mouse::Button::Middle, std::make_pair(new AttackCommand(1, BulletType::COINS), false));

    //controller1.buttons[0] = new JumpCommand(1);
    controller1.buttons[2] = new AttackCommand(1, BulletType::BLUE);
    controller1.buttons[7] = new MenuSelectCommand();
    controller1.axis[(size_t)sf::Joystick::Axis::X].first = new MoveLeftCommand(1);
    controller1.axis[(size_t)sf::Joystick::Axis::X].second = new MoveRightCommand(1);
    controller1.axis[(size_t)sf::Joystick::Axis::PovY].first = new MenuDownCommand();
    controller1.axis[(size_t)sf::Joystick::Axis::PovY].second = new MenuUpCommand();

    return true;
  }

  //deletes all used keys initialized
  void InputHandler::Shutdown()
  {
    {//free mouse and keyboard inputs

      auto it = _minputs.begin();
      while (it != _minputs.end())
      {
        delete (*it).second.first;

        ++it;
      }
      _minputs.clear();



      auto it2 = _mMouse.begin();
      while (it2 != _mMouse.end())
      {
        delete (*it2).second.first;

        ++it2;
      }
      _mMouse.clear();
    }

    {
      auto it = controller1.axis.begin();
      while (it != controller1.axis.end())
      {
        if (it->first)
          delete it->first;

        if (it->second)
          delete it->second;
        ++it;
      }
    }

    {
      auto it = controller1.buttons.begin();
      while (it != controller1.buttons.end())
      {
        if (*it)
          delete *it;

        ++it;
      }
    }
  }


  //sets flags/calls founctions for inputs based on given event
  void InputHandler::GetInput(sf::Event event)
  {
    switch (event.type)
    {
      //KEY PRESSED
    case sf::Event::EventType::KeyPressed:
    {
      //if the key is recognized AND the key at that index is not currently held...
                                                        //key_held_states_[(size_t)event.key.code] <- old 
                                                       //_minputs[event.key.code].second 
      std::pair<Command*, bool> fp_pair = FindCommand(event.key.code);
      if ((event.key.code != sf::Keyboard::Unknown) && (fp_pair.second == false))
      {

        if (fp_pair.first == nullptr)
          break;
        //if the key is registered, call pressed
        //if (inputs_[(size_t)event.key.code] )
        //_minputs[event.key.code].first;
        if (fp_pair.first != nullptr)
          fp_pair.first->Pressed(engine.Find<Messaging::MessageHandler>());

        //set held flag to true
        fp_pair.second = true;
      }
      break;
    }
    //KEY RELEASED
    case sf::Event::EventType::KeyReleased:
    {
      //if the key is recognized...
      std::pair<Command*, bool> fp_pair = FindCommand(event.key.code);
      if (event.key.code != sf::Keyboard::Unknown)
      {
        //if the key is registered, call released
        //if (inputs_[(size_t)event.key.code])
        //_minputs[event.key.code].first
        if (fp_pair.first != nullptr)
          fp_pair.first->Released(engine.Find<Messaging::MessageHandler>());

        //set held flag to false
        fp_pair.second = false;
      }
      break;
    }
    //MOUSE WHELL SCROLLED
    case sf::Event::EventType::MouseWheelScrolled:
    {
      //std::pair<Command*, bool> mp_pair = FindCommand(event.mouseButton.button);

      //mouse_[(size_t)event.mouseButton.button] ->Pressed(engine.Find<Messaging::MessageHandler>() );
      //if (mp_pair.first != nullptr)
       // mp_pair.first->Pressed(engine.Find<Messaging::MessageHandler>());
      break;
    }
    //MOUSE BUTTON PRESSED
    case sf::Event::EventType::MouseButtonPressed:
    {
      //mouse_[(size_t)event.mouseButton.button]->Pressed(engine.Find<Messaging::MessageHandler>());
      std::pair<Command*, bool> mp_pair = FindCommand(event.mouseButton.button);
      if (mp_pair.first != nullptr)
        mp_pair.first->Pressed(engine.Find<Messaging::MessageHandler>());
      break;
    }
    //MOUSE BUTTON RELEASED
    case sf::Event::EventType::MouseButtonReleased:
    {
      //if the key is recognized...
      std::pair<Command*, bool> mp_pair = FindCommand(event.mouseButton.button);
      if (event.mouseButton.button != sf::Mouse::Button::Left)
      {
        //if the key is registered, call released
        //if (mouse_[(size_t)event.mouseButton.button])
         // mouse_[(size_t)event.mouseButton.button]->Released(engine.Find<Messaging::MessageHandler>());
        if (mp_pair.first != nullptr)
          mp_pair.first->Released(engine.Find<Messaging::MessageHandler>());
        //set held flag to false
        //mouse_held_states_[(size_t)event.mouseButton.button] = false;
        mp_pair.second = false;
        Graphics::Window* w = engine.Find<Graphics::Manager>()->window();
        //w->set_cursor("data\\img\\spr\\MiddleClickReticle.cur");
      }
      break;
    }

    //MOUSE MOVED
    case sf::Event::EventType::MouseMoved:
    {
      //todo add mouse movement support
      break;
    }

    //JOYSTICK BUTTON PRESSED
    case sf::Event::EventType::JoystickButtonPressed:
    {
      //todo add joystick button support
      if (event.joystickButton.joystickId == joy1)
      {
        if (controller1.buttons_states[(size_t)event.joystickButton.button] == false)
        {
          //if the key is registered, call pressed
          if (controller1.buttons[(size_t)event.joystickButton.button])
            controller1.buttons[(size_t)event.joystickButton.button]->Pressed(engine.Find<Messaging::MessageHandler>());

          //set held flag to true
          controller1.buttons_states[(size_t)event.joystickButton.button] = true;
        }
      }
    }
    break;

    //JOYSTICK BUTTON RELEASED
    case sf::Event::EventType::JoystickButtonReleased:
    {
      //todo add joystick button support
      if (event.joystickButton.joystickId == joy1)
      {
        //if the key is registered, call pressed
        if (controller1.buttons[(size_t)event.joystickButton.button])
          controller1.buttons[(size_t)event.joystickButton.button]->Released(engine.Find<Messaging::MessageHandler>());

        //set held flag to true
        controller1.buttons_states[(size_t)event.joystickButton.button] = false;
      }

      break;
    }
    //JOYSTICK MOVED
    case sf::Event::EventType::JoystickMoved:
    {
      //todo add joystick movement support
      if (event.joystickMove.joystickId == joy1)
      {
        if (event.joystickMove.position > controller::deadzone && controller1.axis_states[(size_t)event.joystickMove.axis].second == false)
        {
          if (controller1.axis[(size_t)event.joystickButton.button].second)
            controller1.axis[(size_t)event.joystickButton.button].second->Pressed(engine.Find<Messaging::MessageHandler>());

          controller1.axis_states[(size_t)event.joystickMove.axis].second = true;
        }
        else if (event.joystickMove.position < controller::deadzone)
        {
          if (controller1.axis[(size_t)event.joystickButton.button].second)
            controller1.axis[(size_t)event.joystickButton.button].second->Released(engine.Find<Messaging::MessageHandler>());

          controller1.axis_states[(size_t)event.joystickMove.axis].second = false;
        }

        if (event.joystickMove.position < -controller::deadzone && controller1.axis_states[(size_t)event.joystickMove.axis].first == false)
        {
          if (controller1.axis[(size_t)event.joystickButton.button].first)
            controller1.axis[(size_t)event.joystickButton.button].first->Pressed(engine.Find<Messaging::MessageHandler>());

          controller1.axis_states[(size_t)event.joystickMove.axis].first = true;
        }
        else if (event.joystickMove.position > -controller::deadzone)
        {
          if (controller1.axis[(size_t)event.joystickButton.button].first)
            controller1.axis[(size_t)event.joystickButton.button].first->Released(engine.Find<Messaging::MessageHandler>());

          controller1.axis_states[(size_t)event.joystickMove.axis].first = false;
        }
      }

      break;
    }
    //JOYSTICK CONNECTED
    case sf::Event::EventType::JoystickConnected:
    {
      //todo figure out what to do with this
      event.joystickConnect.joystickId;
      int i = 0;
      break;
    }
    //JOYSTICK DISCONNECTED
    case sf::Event::EventType::JoystickDisconnected:
    {
      //todo figure out what to do with this too
      break;
    }
    default:
    {
      break;
    }
    }
  }


  //needed to delete allocations
  InputHandler::~InputHandler()
  {
    //delete _tc;
  }

  std::pair<Command*, bool>& InputHandler::FindCommand(sf::Keyboard::Key type)
  {
    auto findresult = _minputs.find(type);

    if (findresult != _minputs.end())
    {
      return (*findresult).second;
    }
    static  std::pair<Command*, bool> badidea =  std::make_pair(nullptr, false);
    
    return badidea;
  }

  std::pair<Command*, bool>& InputHandler::FindCommand(sf::Mouse::Button type)
  {
    auto findresult = _mMouse.find(type);

    if (findresult != _mMouse.end())
    {
      return (*findresult).second;
    }
    static  std::pair<Command*, bool> badidea = std::make_pair(nullptr, false);

    return badidea;
  }

  //Executes all held keys
  void InputHandler::Update(float dt)
  {

    //for (int i = 0; i < (size_t)sf::Keyboard::Key::KeyCount; ++i, ++it)
    for (auto&& mp : _minputs)
    {
      //if the key is held, call it
      //if (key_held_states_[i] && inputs_[i])
      if (mp.second.second)
        mp.second.first->Held(engine.Find<Messaging::MessageHandler>());
    }
  }

  void RestartLevel::Pressed(Messaging::MessageHandler *)
  {
  }

  void RestartLevel::Held(Messaging::MessageHandler *)
  {
  }
  void RestartLevel::Released(Messaging::MessageHandler * handler)
  {
    Messaging::Message m{};
    m.id = Messaging::ID::Reset;
    engine.Find<Messaging::MessageHandler>()->Post(m);
  }
}