#pragma once
/******************************************************************************/
/*!
  \file    GameState.cpp
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/07/2017

  \brief  
    
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <Engine/Engine.h>
#include <Input/Input.hpp>
#include <Transform/Transform.hpp>
#include "GameState.h"
#include <Behaviors/CameraBehavior.hpp>
#include <Behaviors/Player1Behavior.hpp>
#include <Behaviors/Player2Behavior.hpp>
#include "GameStateManager/GameStateManager.h"
#include <GameObject/GameObjectManager.h>
namespace InputPlayer
{
	void InputEsc(ObjectId obj, const Messaging::Message& message);
	void RegisterPInput(ObjectId key, Messaging::MessageHandler *m_handler);
	void UnRegisterPInput(ObjectId key, Messaging::MessageHandler *m_handler);
}

void GameState::Init()
{
	ObjectFactory::Create(filename_.c_str(), manager_);
	Messaging::MessageHandler *m_handler = engine.Find<Messaging::MessageHandler>();
	m_handler->Register(Messaging::ID::MenuSelect, nullobj, InputPlayer::InputEsc);
}

void GameState::Update(float dt) {
  Manager().Update(dt);
	
	if (Reset())
	{
		Reset(false);
		engine.Find<GameStateManager>()->ResetState();
	}

	if (NewState_.first)
	{
		NewState_.first = false;
		GameStateManager* managaer = engine.Find<GameStateManager>();
		managaer->ChangeState(NewState_.second);
	}
}

void GameState::Shutdown() {
	//manager_.Serialize(filename_.c_str());
	Messaging::MessageHandler *m_handler = engine.Find<Messaging::MessageHandler>();
	m_handler->Unregister(Messaging::ID::MenuSelect, nullobj);
	m_handler->Unregister(Messaging::ID::P1Win, nullobj);
	//manager_.UnRegisterMsg(InputPlayer::UnRegisterPInput, m_handler);
	engine.Find<Audio::AudioManager>()->StopMusic(); //REMOVE LATER
	manager_.Shutdown();
}

