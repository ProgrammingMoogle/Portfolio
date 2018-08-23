#pragma once
/******************************************************************************/
/*!
  \file    Engine.cpp
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/29/2017

  \brief
  definitions for the Game's engine

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Engine.h"
#include "Trace/Trace.hpp"

Engine engine;


Engine::Engine() :
  Systems_{ new Trace::TraceHandler(),
        new Audio::AudioManager(),
        new Messaging::MessageHandler(),
              new Graphics::Manager(),
        new GameStateManager(),
        new Input::InputHandler()
},
Index_{ {std::type_index(typeid(Trace::TraceHandler)), 0},
      {std::type_index(typeid(Audio::AudioManager)), 1 },
      {std::type_index(typeid(Messaging::MessageHandler)), 2},
      {std::type_index(typeid(Graphics::Manager)), 3},
      {std::type_index(typeid(GameStateManager)), 4 },
      {std::type_index(typeid(Input::InputHandler)), 5}
} {};

bool Engine::Init()
{
  std::cout << "Init start" << std::endl;
  // init physics
  std::cout << "Physics Init" << std::endl;
  physicsEngine_ = new KPE::PhysicsEngine;
  std::cout << "Physics Init Concluded" << std::endl;

  physicsEngine_->init();

  // initialize all of the engine systems
  for (auto i : Systems_)
  {
    std::cout << i << "Init start" << std::endl;
    i->Init();
    std::cout << i << "Init Concluded" << std::endl;
  }
  return true;
}

void Engine::Update(float dt)
{
  dt_ = dt;
  total_time_ += dt;

  physicsEngine_->update(1.0f / 60.0f);

  // Update all of the engine systems
  for (auto i : Systems_)
    i->Update(dt);
}

bool Engine::IsRunning() {
  return Find<GameStateManager>()->IsRunning() && isRunning_;
}

void Engine::Shutdown()
{
  // Shutdown all of the engine systems
  for (auto i = Systems_.rbegin(); i != Systems_.rend(); ++i)
  {
    (**i).Shutdown();
    delete *i;
  }
  //physicsEngine_->shutdown();
  delete physicsEngine_;
}

KPE::PhysicsEngine
* Engine::GetPhysicsEngine() {
  return physicsEngine_;
}

float Engine::dt() const
{
  return dt_;
}

float Engine::runTime() const
{
  return total_time_;
}

void Engine::Quit()
{
  isRunning_ = false;
}
