/**
  \file   MenuBehaviors.cpp
  \author Zayd Gaudet

  \brief Behavior component for menus

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "MenuBehaviors.hpp"
#include <Engine/Engine.h>
#include <Messaging/Messaging.hpp>		
#include <Menus/MenuComponent.h>
#include <Audio/Audio.hpp>

/**
 * \brief 
 * \param parent 
 */
MenuBehavior::MenuBehavior(GameObject &parent) : Component(parent) 
{
	Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
	ObjectId key = SetKey(parent.GetKey());
	m_handler->Register(Messaging::ID::MenuUp, key, MenuBehavior::InputMenuUp);
	m_handler->Register(Messaging::ID::MenuDown, key, MenuBehavior::InputMenuDown);
	m_handler->Register(Messaging::ID::MenuSelect, key, MenuBehavior::InputMenuSelect);
}

/**
 * \brief 
 */
MenuBehavior::~MenuBehavior() {
	Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
	ObjectId key = GetParent().GetKey();
	m_handler->Unregister(Messaging::ID::MenuUp, key);
	m_handler->Unregister(Messaging::ID::MenuDown, key);
	m_handler->Unregister(Messaging::ID::MenuSelect, key);
	m_handler->Unregister(Messaging::ID::Attack, key);
}

/**
 * \brief 
 * \param dt 
 */
void MenuBehavior::Update(float dt) {
	
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void MenuBehavior::InputMenuUp(ObjectId obj, const Messaging::Message & message) {
	GameState* state = engine.Find<GameStateManager>()->CurrentState();
	GameObjectManager& manager = state->Manager();
	GameObject& object = manager.Find(obj);
	object.Find<MenuBehavior>()->onInputMenuUp(message);
}

/**
 * \brief 
 * \param message 
 */
void MenuBehavior::onInputMenuUp(const Messaging::Message & message) {

	MenuComponent* menu = GetParent().Find<MenuComponent>();
	Audio::AudioManager *audio_manager = engine.Find<Audio::AudioManager>();

	if (message.message.menu_up.key_state == Messaging::KeyState::Pressed)
	{
		--(*menu);
		audio_manager->PlaySound("menu_move");
	}
	else if (message.message.menu_up.key_state == Messaging::KeyState::Held)
	{
		delay -= 0.0167f;
		if (delay <= 0) 
		{ 
			--(*menu);
			audio_manager->PlaySound("menu_move");
			delay = 0.25f;
		}
	}
	else if (message.message.menu_up.key_state == Messaging::KeyState::Released)
	{
		delay = 0.25f;
	}
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void MenuBehavior::InputMenuDown(ObjectId obj, const Messaging::Message & message) {
	GameState* state = engine.Find<GameStateManager>()->CurrentState();
	GameObjectManager& manager = state->Manager();
	GameObject& object = manager.Find(obj);
	object.Find<MenuBehavior>()->onInputMenuDown(message);
}

/**
 * \brief 
 * \param message 
 */
void MenuBehavior::onInputMenuDown(const Messaging::Message & message) {
	MenuComponent* menu = GetParent().Find<MenuComponent>();
	Audio::AudioManager *audio_manager = engine.Find<Audio::AudioManager>();

	if (message.message.menu_down.key_state == Messaging::KeyState::Pressed)
	{
		++(*menu);
		audio_manager->PlaySound("menu_move");
	}
	else if (message.message.menu_down.key_state == Messaging::KeyState::Held)
	{
		delay -= 0.0167f;
		if (delay <= 0)
		{
			++(*menu);
			audio_manager->PlaySound("menu_move");
			delay = 0.25f;
		}
	}
	else if (message.message.menu_down.key_state == Messaging::KeyState::Released)
	{
		delay = 0.25f;
	}
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void MenuBehavior::InputMenuSelect(ObjectId obj, const Messaging::Message & message) {
	GameState* state = engine.Find<GameStateManager>()->CurrentState();
	GameObjectManager& manager = state->Manager();
	GameObject& object = manager.Find(obj);
	object.Find<MenuBehavior>()->onInputMenuSelect(message);
}

/**
 * \brief 
 * \param message 
 */
void MenuBehavior::onInputMenuSelect(const Messaging::Message & message) {
	MenuComponent* menu = GetParent().Find<MenuComponent>();
	Audio::AudioManager *audio_manager = engine.Find<Audio::AudioManager>();

	if (message.message.menu_select.key_state == Messaging::KeyState::Pressed) {
		audio_manager->PlaySound("Scifi_Forcefield_Impact3");
		menu->PushState();
	}
}