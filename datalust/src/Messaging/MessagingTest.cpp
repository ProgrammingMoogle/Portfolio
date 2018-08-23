/**
  \file   MessagingTest.cpp
  \author Taylor Osmond

  \brief Implementation of messaging test

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Messaging.hpp"
#include "MessagingTest.hpp"
#include <GameObject/SlotMap.h>
#include <Engine/Engine.h>
#include <iostream>

namespace Messaging{
  namespace{
    void Test1(ObjectId obj, const Message& message)
    {
      std::cout << "message received, message contains " << message.message.test.i << std::endl;
    }
  }
}

MessagingTest::MessagingTest(std::string levels) : GameState(levels) {}

void MessagingTest::Init() 
{
  Messaging::MessageHandler *handler = engine.Find<Messaging::MessageHandler>(); //get handler
  handler->Register(Messaging::ID::Test, nullobj, Messaging::Test1); //register test function
  Messaging::Message msg; //create message
  msg.id = Messaging::ID::Test; //assign test id
  msg.message.test.i = 4; //insert whatever data to data in struct

  handler->Post(msg); //post the message

  //unregister
  handler->Unregister(Messaging::ID::Test, std::make_pair((size_t)-1, (unsigned)-1));

  handler->Post(msg); //test if unregister worked
  std::cin.get();
}