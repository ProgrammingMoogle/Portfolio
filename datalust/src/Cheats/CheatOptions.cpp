/******************************************************************************/
/*!
  \file   CheatOptions.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "CheatOptions.hpp"
#include <Engine/Engine.h>
#include <Graphics/Window.hpp>
#include <Graphics/Renderer.hpp>
#include <Audio/Audio.hpp>
#include <Behaviors/Player1Behavior.hpp>
#include <Graphics/Window.hpp>
#include <Transform/Transform.hpp>
#include "RoomSwitchBehavior.h"
#include "GoalBehavior.h"
#include "GameStateManager\GameStateManager.h"

/**
 * \brief 
 * \param parent 
 */
CheatOptions::CheatOptions(GameObject& parent): 
Component(parent)
{
  cheatActive_ = isFullscreen_ = godMode_ = musicMuted_ = false;

  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();

  m_handler->Register(Messaging::ID::CheatF1, key, CheatOptions::InputF1);
  m_handler->Register(Messaging::ID::CheatF2, key, CheatOptions::InputF2);
  m_handler->Register(Messaging::ID::CheatF3, key, CheatOptions::InputF3);
  m_handler->Register(Messaging::ID::CheatF4, key, CheatOptions::InputF4);
  m_handler->Register(Messaging::ID::CheatF5, key, CheatOptions::InputF5);
  m_handler->Register(Messaging::ID::CheatF6, key, CheatOptions::InputF6);
  m_handler->Register(Messaging::ID::CheatF7, key, CheatOptions::InputF7);

  //Place the console at the top left corner
  //placeInCorner();

}
/**
 * \brief 
 */
CheatOptions::~CheatOptions() {
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();

  m_handler->Unregister(Messaging::ID::CheatF1, key);
  m_handler->Unregister(Messaging::ID::CheatF2, key);
  m_handler->Unregister(Messaging::ID::CheatF3, key);
  m_handler->Unregister(Messaging::ID::CheatF4, key);
  m_handler->Unregister(Messaging::ID::CheatF5, key);
  m_handler->Unregister(Messaging::ID::CheatF6, key);
  m_handler->Unregister(Messaging::ID::CheatF7, key);

  Graphics::Renderer::getCurrentRenderer()->window()->deactivateCheats();
}
/**
 * \brief 
 * \param dt 
 */
void CheatOptions::Update(float dt) {

  if (GetParent().IsActive())
  {
   if (HasWon)
   {
     timer -= dt;
     if (timer <= 0.0f)
     {
       engine.Find<GameStateManager>()->DelayedPopState();
       engine.Find<GameStateManager>()->DelayedPushState(1);
     }
   }
  }
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void CheatOptions::InputF1(ObjectId obj, const Messaging::Message& message) 
{
  if (Graphics::Renderer::getCurrentRenderer()->window()->isPaused()) {
    return;
  }

  if (message.message.cheatF1.key_state == Messaging::KeyState::Released) {
    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find(obj);
    if (object.GetName() == "CheatOptions")
    {
      object.Find<CheatOptions>()->showCredits();
    }
  }
}
/**
 * \brief 
 * \param obj 
 * \param message 
 */
void CheatOptions::InputF2(ObjectId obj, const Messaging::Message& message) {
  if (Graphics::Renderer::getCurrentRenderer()->window()->isPaused()) {
    return;
  }

  if (message.message.cheatF2.key_state == Messaging::KeyState::Released) {
    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find(obj);
    if (object.GetName() == "CheatOptions")
    {
      object.Find<CheatOptions>()->toggleFullscreen();
    }
  }
}
/**
 * \brief 
 * \param obj 
 * \param message 
 */
void CheatOptions::InputF3(ObjectId obj, const Messaging::Message& message) {
  if (Graphics::Renderer::getCurrentRenderer()->window()->isPaused()) {
    return;
  }

  if (message.message.cheatF3.key_state == Messaging::KeyState::Released) {
    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find(obj);
    if (object.GetName() == "CheatOptions")
    {
      object.Find<CheatOptions>()->muteMusic();
    }
  }
}
/**
 * \brief 
 * \param obj 
 * \param message 
 */
void CheatOptions::InputF4(ObjectId obj, const Messaging::Message& message) {
  if (Graphics::Renderer::getCurrentRenderer()->window()->isPaused()) {
    return;
  }

  if (message.message.cheatF4.key_state == Messaging::KeyState::Released) {
    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find(obj);
    if (object.GetName() == "CheatOptions")
    {
      object.Find<CheatOptions>()->skipLevel();
    }
  }
}
/**
 * \brief 
 * \param obj 
 * \param message 
 */
void CheatOptions::InputF5(ObjectId obj, const Messaging::Message& message) {
  if (Graphics::Renderer::getCurrentRenderer()->window()->isPaused()) {
    return;
  }

  if (message.message.cheatF5.key_state == Messaging::KeyState::Released) {
    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find(obj);
    if (object.GetName() == "CheatOptions")
    {
      object.Find<CheatOptions>()->winGame();
    }
  }
}
/**
 * \brief 
 * \param obj 
 * \param message 
 */
void CheatOptions::InputF6(ObjectId obj, const Messaging::Message& message) {
  if (Graphics::Renderer::getCurrentRenderer()->window()->isPaused()) {
    return;
  }

  if (message.message.cheatF6.key_state == Messaging::KeyState::Released) {
    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find(obj);
    if (object.GetName() == "CheatOptions")
    {
      object.Find<CheatOptions>()->loseGame();
    }
  }
}
/**
 * \brief 
 * \param obj 
 * \param message 
 */
void CheatOptions::InputF7(ObjectId obj, const Messaging::Message& message) {
  if (Graphics::Renderer::getCurrentRenderer()->window()->isPaused()) {
    return;
  }

  if (message.message.cheatF7.key_state == Messaging::KeyState::Released) {
    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find(obj);
    if (object.GetName() == "CheatOptions")
    {
      object.Find<CheatOptions>()->godMode();
    }
  }
}


/**
 * \brief 
 */
void CheatOptions::showCredits()
{
  engine.Find<GameStateManager>()->DelayedPopState();
  engine.Find<GameStateManager>()->DelayedPushState(5);
}

/**
 * \brief 
 */
void CheatOptions::toggleFullscreen()
{
  engine.Find<Graphics::Manager>()->window()->setFullscreen();
}

/**
 * \brief 
 */
void CheatOptions::muteMusic()
{
  if (!musicMuted_) {
    musicMuted_ = true;
    prevMusicVol_ = engine.Find<Audio::AudioManager>()->GetMusicVolume();
    engine.Find<Audio::AudioManager>()->SetMusicVolume(0);
  }
  else {
    musicMuted_ = false;
    engine.Find<Audio::AudioManager>()->SetMusicVolume(prevMusicVol_);
  }
}

/**
 * \brief 
 */
void CheatOptions::skipLevel() {
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
    GameObject& RoomSwitch = manager.Find("SwitchListener");
    GameObject& Goal = RoomSwitch.Find<RoomSwitchBehavior>()->find_goal_block();
    Goal.Find<GoalBehavior>()->change_level();

}

/**
 * \brief 
 */
void CheatOptions::winGame() 
{
  if (HasWon == false) {
    ObjectFactory::Create("data/json/Player1Wins.json", engine.Find<GameStateManager>()->CurrentState()->Manager());
    HasWon = true;
    timer = delay;
  }
}

/**
 * \brief 
 */
void CheatOptions::godMode()
{
  //Find player's health component
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& player = manager.Find("Player1");

  if (godMode_ == false) {
    player.Find<Player1Behavior>()->activateGodMode();
	godMode_ = true;
  }
  else if(godMode_ == true) {
    player.Find<Player1Behavior>()->deactivateGodMode();
	godMode_ = false;
  }
}

/**
 * \brief 
 */
void CheatOptions::loseGame() {

  if (HasWon == false) {
    ObjectFactory::Create("data/json/PlayerDies.json", engine.Find<GameStateManager>()->CurrentState()->Manager());
    HasWon = true;
    timer = delay;
  }
}

/**
 * \brief 
 */
void CheatOptions::placeInCorner() {
  Graphics::Window* window = engine.Find<Graphics::Manager>()->window();
  glm::vec2 pos((window->width() / 2.0f) * -1.0f, window->height() / 2.0f);

  GetParent().Find<Transform>()->setPosition(pos);
}