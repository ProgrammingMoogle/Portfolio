/******************************************************************************/
/*!
  \file    GameStateManager.cpp
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/07/2017

  \brief
	GameStateManager.cpp contains the implementation for the GameStateManager.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include<utility>
#include <GameStates/StubState.h>
#include <GameStates/DemoState.h>
#include <GameStates/AnimationState.h>
#include <Audio/AudioTest.hpp>
#include <Engine/Engine.h>
#include "GameStateManager.h"
#include "Graphics/Window.hpp"
#include "Behaviors\Player1Behavior.hpp"

// sets the isUpdating_ function to be false ans pushes a nullptr as the first 
// item on the stack

GameStateManager::GameStateManager() : levels_()
{
  paused_ = false;
	gameStateManager_.push(nullptr);
}

bool GameStateManager::Init() {
	PushState(0);
	return true;
}

void GameStateManager::Update(float dt)
{
  if (paused_) {
    return;
  }

  while (MachineUpdateQueue.size() > 0)
  {
    auto && command = MachineUpdateQueue.front();

    switch (command.op)
    {

    case  StateOperation::EnumPushState:{
      pushState_(command.newState);
    }break;

    case StateOperation::EnumPopState: {
      popState_();
    }break;

    case StateOperation::EnumResetState: {
      resetState_();
    }break;

    default: {
    }break;
    }

    MachineUpdateQueue.pop();
  }

	// set the isUpdating_ flag
	isUpdating_ = true;

	// if the state on the top of the stack isn't null atempt to update it
	if (gameStateManager_.top())
	{
		try
		{
			gameStateManager_.top()->Update(dt);
		}
		// if any functions that manipulate the stack are called during this 
		// time it will throw, so that it can be manipulated outside the update 
		// call
		catch (const stateQuit_&)
		{
			quit_();
		}
		catch (const stateReset_&)
		{
			resetState_();
		}
		catch (const stateChange_& state)
		{
			//Exiting PauseState Function
			if (state.get_state() == 1337)
			{
				//Shutdown Pause Menu
				popState_();
				
				//Goes to the next highest active state (in the case of Gameplay. It is DemoState.)

				//Is unable to return to the previous state and can't figure out why since my understanding of a stack is
				//after popping the top, it goes to the state before it

				//made everything a shared_ptr don't know if it affects anything much

				//works changeState_(2)
				changeState_(2);
			}
			else if (state.get_state() == 42)
			{
				isUpdating_ = false;
        while (gameStateManager_.size() > 1)
        {
          gameStateManager_.top()->Shutdown();
          gameStateManager_.pop();
        }
			}
			else
			{
				(state.get_state() == -1) ? changeState_() : changeState_(state.get_state());
			}
		}
		catch (const statePush_& state)
		{
			pushState_(state.get_state());
		}
		catch (const statePop_&)
		{
			popState_();
		}
	}
	isUpdating_ = false;
}

GameStateManager::~GameStateManager()
{
	Quit();
}

void GameStateManager::Quit()
{
  isUpdating_ = false;
	// make sure that the GameStateManager isn't updating and perform the Quit 
	// function
	if (isUpdating_)
		stateQuit_();
	quit_();
}

void GameStateManager::ResetState()
{
	// make sure that the GameStateManager isn't updating and perform the 
	// ResetState function
	if (isUpdating_)
		throw stateReset_();
	resetState_();
}

void GameStateManager::ChangeState(const int level)
{
	// make sure that the GameStateManager isn't updating and perform the 
	// ChangeState function
	if (isUpdating_)
		throw stateChange_(level);
	changeState_(level);
}

void GameStateManager::ChangeState()
{
	if (isUpdating_)
		throw stateChange_(-1);
	changeState_();
}

void GameStateManager::PushState(const int newState)
{
	// make sure that the GameStateManager isn't updating and perform the 
	// PushState function
	if (isUpdating_)
		throw statePush_(newState);
	pushState_(newState);
}

void GameStateManager::PopState()
{
	// make sure that the GameStateManager isn't updating and perform the 
	// PopState function
	if (isUpdating_)
		throw statePop_();
	popState_();
}

void GameStateManager::DelayedPopState()
{
  MachineUpdateQueue.emplace(StateOperation::EnumPopState);
}

void GameStateManager::DelayedPushState(int new_state)
{
  MachineUpdateQueue.emplace(StateOperation::EnumPushState, new_state);
}

void GameStateManager::DelayedResetState()
{
  MachineUpdateQueue.emplace(StateOperation::EnumResetState);
}

void GameStateManager::quit_()
{
	// loop through the stack untill you hit the nullptr value at the begining
	// and call the pop_stack member function
	while (gameStateManager_.top())
	{
		popState_();
	}
}

void GameStateManager::resetState_()
{
	// restart the state, shut it off and on again
	gameStateManager_.top()->Shutdown();
	engine.GetPhysicsEngine()->forceRegistry_.clear();
	gameStateManager_.top()->Init();
}

void GameStateManager::changeState_(const int level)
{
	// replace the state at the top of the stack with a different state
	popState_();
	pushState_(level);
}

void GameStateManager::changeState_()
{
	popState_();
	pushState_();
}

void GameStateManager::pushState_(const int level)
{
	// push a new state on the top of the stack and Initialize the state
	gameStateManager_.push(std::make_shared<GameState>(levels_.SetNextLevel(level)));
	gameStateManager_.top()->Init();
}

void GameStateManager::pushState_()
{
	gameStateManager_.push(std::make_shared<GameState>(levels_.SetNextLevel()));
	gameStateManager_.top()->Init();
}

void GameStateManager::popState_()
{
	// call the Shutdown on the state so it can clean up and then terminate it
	gameStateManager_.top()->Shutdown();
	gameStateManager_.pop();
}

GameState* GameStateManager::CurrentState()
{
	// return the GameState that is curently atop the stack
	return gameStateManager_.top().get();
}

bool GameStateManager::IsRunning()
{
	// return whether the GameStateManager is running or not
	if (CurrentState())
		return true;
	return false;
}

void GameStateManager::PauseState(const int Level)
{
  paused_ = true;
  pauseObj = &(ObjectFactory::Create("data/json/pause.json", 
    gameStateManager_.top()->Manager())).front().get();
  //Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();

  try {
    GameObject& player = CurrentState()->Manager().Find("Player1");
    ObjectId key = player.Find<Player1Behavior>()->GetParent().GetKey();
    player.Find<Player1Behavior>()->RemoveInput(key);
    playerPaused = true;
  }
  catch (int)
  {
  
  }
}

void GameStateManager::UnpauseState(const int level) 
{
  paused_ = false;
  pauseObj->Destroy();

  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();

  if(playerPaused)
  {
    GameObject& player = CurrentState()->Manager().Find("Player1");

    ObjectId key = player.Find<Player1Behavior>()->GetParent().GetKey();

    player.Find<Player1Behavior>()->ReturnInput(key);

    playerPaused = false;
  }

}

GameStateManager::stateChange_::stateChange_(const int newState) : newState_(newState)
{
}

const int GameStateManager::stateChange_::get_state() const
{
	return newState_;
}

GameStateManager::statePush_::statePush_(const int newState) : newState_(newState)
{
}

const int GameStateManager::statePush_::get_state() const
{
	return newState_;
}
