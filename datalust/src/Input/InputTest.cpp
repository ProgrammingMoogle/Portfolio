/**
  \file   InputTest.cpp
  \author Taylor Osmond

  \brief Implementation of input test

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Input.hpp"
#include <Messaging/Messaging.hpp>
#include <GameObject/SlotMap.h>
#include <Engine/Engine.h>
#include "InputTest.hpp"
#include <iostream>

namespace Input{
  namespace{
    void TestMenuUp(ObjectId obj, const Messaging::Message& message)
    {
		std::cout << "Menu UP ";
		if (message.message.menu_up.key_state == Messaging::KeyState::Pressed)
			std::cout << "Pressed" << std::endl;
		else if (message.message.menu_up.key_state == Messaging::KeyState::Held)
			std::cout << "Held" << std::endl;
		else if (message.message.menu_up.key_state == Messaging::KeyState::Released)
			std::cout << "Released" << std::endl;
    }

    void TestMenuDown(ObjectId obj, const Messaging::Message& message)
    {
      std::cout << "Menu Down ";
	  if (message.message.menu_down.key_state == Messaging::KeyState::Pressed)
		  std::cout << "Pressed" << std::endl;
	  else if (message.message.menu_down.key_state == Messaging::KeyState::Held)
		  std::cout << "Held" << std::endl;
	  else if (message.message.menu_down.key_state == Messaging::KeyState::Released)
		  std::cout << "Released" << std::endl;
    }

    void TestMenuLeft(ObjectId obj, const Messaging::Message& message)
    {
      std::cout << "Menu Left ";
	  if (message.message.menu_left.key_state == Messaging::KeyState::Pressed)
		  std::cout << "Pressed" << std::endl;
	  else if (message.message.menu_left.key_state == Messaging::KeyState::Held)
		  std::cout << "Held" << std::endl;
	  else if (message.message.menu_left.key_state == Messaging::KeyState::Released)
		  std::cout << "Released" << std::endl;
    }

    void TestMenuRight(ObjectId obj, const Messaging::Message& message)
    {
      std::cout << "Menu Right ";
	  if (message.message.menu_right.key_state == Messaging::KeyState::Pressed)
		  std::cout << "Pressed" << std::endl;
	  else if (message.message.menu_right.key_state == Messaging::KeyState::Held)
		  std::cout << "Held" << std::endl;
	  else if (message.message.menu_right.key_state == Messaging::KeyState::Released)
		  std::cout << "Released" << std::endl;
    }

    void TestMenuSelect(ObjectId obj, const Messaging::Message& message)
    {
      std::cout << "Menu Select ";
	  if (message.message.menu_select.key_state == Messaging::KeyState::Pressed)
		  std::cout << "Pressed" << std::endl;
	  else if (message.message.menu_select.key_state == Messaging::KeyState::Held)
		  std::cout << "Held" << std::endl;
	  else if (message.message.menu_select.key_state == Messaging::KeyState::Released)
		  std::cout << "Released" << std::endl;
    }

    void TestMenuBack(ObjectId obj, const Messaging::Message& message)
    {
      std::cout << "Menu Back ";
	  if (message.message.menu_back.key_state == Messaging::KeyState::Pressed)
		  std::cout << "Pressed" << std::endl;
	  else if (message.message.menu_back.key_state == Messaging::KeyState::Held)
		  std::cout << "Held" << std::endl;
	  else if (message.message.menu_back.key_state == Messaging::KeyState::Released)
		  std::cout << "Released" << std::endl;
    }
  }
}

InputTest::InputTest(std::string levels) : GameState(levels)  {}

void InputTest::Init()
{
  Messaging::MessageHandler *m_handler = engine.Find<Messaging::MessageHandler>(); //get message handler
  m_handler->Register(Messaging::ID::MenuUp, nullobj, Input::TestMenuUp);          //register menu up
  m_handler->Register(Messaging::ID::MenuDown, nullobj, Input::TestMenuDown);	   //register menu down
  m_handler->Register(Messaging::ID::MenuLeft, nullobj, Input::TestMenuLeft);      //register menu left
  m_handler->Register(Messaging::ID::MenuRight, nullobj, Input::TestMenuRight);	   //register menu right
  m_handler->Register(Messaging::ID::MenuSelect, nullobj, Input::TestMenuSelect);  //register menu select
  m_handler->Register(Messaging::ID::MenuBack, nullobj, Input::TestMenuBack);      //register menu back
}