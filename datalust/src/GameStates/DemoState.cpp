/**
  \file   DemoState.cpp
  \author Taylor Osmond

  \brief 

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "DemoState.h"
#include <GameObject/GameObject.h>
#include <GameObject/GameObjectManager.h>
#include <Engine/Engine.h>
#include <Graphics/Graphics.hpp>
#include <Transform/Transform.hpp>
#include <Physics/Colliders.hpp>
#include <Physics/RigidBody.hpp>
#include <GameObject/Component.h>
#include <Physics/ForceRegistry.hpp>
#include <Physics/ForceEmitter.hpp>
#include <Audio/Audio.hpp>
#include <iostream>
#include <Trace/Trace.hpp>
#include "Graphics\Camera.hpp"

enum facing
{
	right,
	left,
};
//float pitch = 1.0f;
//float frequency = 1.0f;
//float volume = 1.0f;
facing Player1 = right;
static int jumps = 0;
static float Friction = 0.3f;

//Trace::TraceHandler *trace_handler = engine.Find<Trace::TraceHandler>();
//int trace_counter = 0;

namespace InputPlayer {
  bool anchorFlip = false;
  bool isWinnning = false;
	void InputEsc(ObjectId obj, const Messaging::Message& message) {
    if (message.message.menu_select.key_state == Messaging::KeyState::Pressed) {
      //engine.Find<GameStateManager>()->CurrentState()->Reset(true);
    }
	}


	void InputLeft(ObjectId obj, const Messaging::Message& message)
	{
		// Grab the Player using the ObjectId 
		GameState* state = engine.Find<GameStateManager>()->CurrentState();
		GameObjectManager& manager = state->Manager();
		GameObject& player = manager.Find(obj);

		// Grab the Components from the player object
		KPE::RigidBody* body = player.Find<KPE::RigidBody>();
		Transform* transform = player.Find<Transform>();
		Graphics::Sprite* sprite = player.Find<Graphics::Sprite>();
		glm::vec2 velocity = body->getVelocity();
			

		if (message.message.move_left.player == 1 && player.GetName() == "Player1")
		{
			Player1 = left;
			if (transform->getXScale() >= 0)
				transform->setXScale(-player.Find<Transform>()->getXScale());
			if (message.message.move_left.key_state == Messaging::KeyState::Pressed) {
				std::cout << "LEFT \n";
				sprite->play("walk");
				//trace_handler->TraceMessage(INFO, "left pressed");
				//trace_handler->TraceMessage(WARNING, "Left Pressed!");
				//trace_handler->TraceMessage(ERROR, "LEFT PRESSED!!!");
				//trace_counter++;
			}
			else if (message.message.move_left.key_state == Messaging::KeyState::Held) {
				velocity.x = -player.MoveVal();
				body->setVelocity(velocity);
			}
			else if (message.message.move_left.key_state == Messaging::KeyState::Released) {
				std::cout << "UNLEFT \n";
				velocity.x = 0;
				body->setVelocity(velocity);
				sprite->play("idle");
			}
		}
	}

	void InputUp(ObjectId obj, const Messaging::Message& message)
	{
		// Grab the Player using the ObjectId 
		GameState* state = engine.Find<GameStateManager>()->CurrentState();
		GameObjectManager& manager = state->Manager();
		GameObject& player = manager.Find(obj);

		// Grab the Components from the player object
		KPE::RigidBody* body = player.Find<KPE::RigidBody>();
		Transform* transform = player.Find<Transform>();
		Graphics::Sprite* sprite = player.Find<Graphics::Sprite>();
		glm::vec2 velocity = body->getVelocity();

    /*
		if (message.message.jump.player == 1 && player.GetName() == "Player1")
		{
			// check if the velocity of the player is withing a certain range around 0 and if it is rest
			// the number of jumps available to the player
			if ((velocity.y <= 0.1 && velocity.y >= -0.1) || velocity.y == -0.196285218f)
				jumps = 0;
			if (message.message.jump.key_state == Messaging::KeyState::Pressed) {

				if (jumps < 2)
				{
					// play the jump animation
					sprite->play("jump");
					// trace msg to the console
					std::cout << "UPWARDS \n";
					// change the velocity of the player
					velocity.y = player.JumpVal();
					body->setVelocity(velocity);
					//increment the number of jumps
					++jumps;

					//play jump sound
					Audio::AudioManager *audio_manager = engine.Find<Audio::AudioManager>();
					audio_manager->PlaySound("jump_1");
					audio_manager->SetEffectsVolume(4.0f);
				}
			}
		}*/
	}
	
	void InputRight(ObjectId obj, const Messaging::Message& message)
	{
		// Grab the Player using the ObjectId 
		GameState* state = engine.Find<GameStateManager>()->CurrentState();
		GameObjectManager& manager = state->Manager();
		GameObject& player = manager.Find(obj);

		// Grab the Components from the player object
		KPE::RigidBody* body = player.Find<KPE::RigidBody>();
		Transform* transform = player.Find<Transform>();
		Graphics::Sprite* sprite = player.Find<Graphics::Sprite>();
		glm::vec2 velocity = body->getVelocity();


		if (message.message.move_right.player == 1 && player.GetName() == "Player1")
		{
			Player1 = right;
			if (transform->getXScale() <= 0)
				transform->setXScale(-player.Find<Transform>()->getXScale());
			if (message.message.move_left.key_state == Messaging::KeyState::Pressed) {
				std::cout << "Right \n";
				sprite->play("walk");
			}
			else if (message.message.move_left.key_state == Messaging::KeyState::Held) {
				velocity.x = player.MoveVal();
				body->setVelocity(velocity);
			}
			else if (message.message.move_left.key_state == Messaging::KeyState::Released) {
				std::cout << "UNRight \n";
				velocity.x = 0;
				body->setVelocity(velocity);
				sprite->play("idle");
			}
		}
	}

	void InputPunch(ObjectId obj, const Messaging::Message& message) 
	{
		
		// Grab the Player using the ObjectId 
		GameState* state = engine.Find<GameStateManager>()->CurrentState();
		GameObjectManager& manager = state->Manager();
		GameObject& player = manager.Find(obj);

		// Grab the Components from the player object
		KPE::RigidBody* body = player.Find<KPE::RigidBody>();
		Transform* transform = player.Find<Transform>();
		Graphics::Sprite* sprite = player.Find<Graphics::Sprite>();
		glm::vec2 velocity = body->getVelocity();


		if (message.message.attack.Mouse_state == Messaging::MouseState::Pressed)
		{
			if (message.message.attack.player == 1 && player.GetName() == "Player1")
			{
					// create a "Punch Object"
					try { manager.Find("Punch"); }
					catch (int)
					{
						ObjectFactory::Create("data/json/Punch.json", manager);

						// grab the punch object we just created
						GameObject& Punch = manager.Find("Punch");

						//change the transform of the object
						glm::vec2 pos = transform->getPosition();
						
						//TODO MAKE THE PUNCH SPAWN BASED ON THE RIGIDBODY'S HALFSIZE
						// set the pos of the punch based on whether it is facing left or right
						pos.x = (pos.x + (34 * transform->getXScale()));

						// set the pos of the punches transform
						Punch.Find<Transform>()->setPosition(pos);

						glm::vec2 velocity(0, 0);
						velocity.x = Player1 ? -0.7f : 0.7f;
						Punch.Find<KPE::RigidBody>()->addVelocity(velocity);

						manager.Update(0);
						sprite->play("punch");
						manager.DelaydDestruction(Punch.Find<Transform>()->GetKey(), 0.44125f);

						//play punch sound
						Audio::AudioManager *audio_manager = engine.Find<Audio::AudioManager>();
						audio_manager->PlaySound("punch_2");
						audio_manager->SetEffectsVolume(5.0f);
					}
			}
		}
	}

	void RegisterPInput(ObjectId key, Messaging::MessageHandler *m_handler)
	{
		m_handler->Register(Messaging::ID::MoveLeft, key, InputPlayer::InputLeft);
		m_handler->Register(Messaging::ID::MoveRight, key, InputPlayer::InputRight);
		m_handler->Register(Messaging::ID::Jump, key, InputPlayer::InputUp);
		m_handler->Register(Messaging::ID::Attack, key, InputPlayer::InputPunch);
	}

	void UnRegisterPInput(ObjectId key, Messaging::MessageHandler *m_handler)
	{
		m_handler->Unregister(Messaging::ID::MoveLeft, key);
		m_handler->Unregister(Messaging::ID::MoveRight, key);
		m_handler->Unregister(Messaging::ID::Jump, key);
		m_handler->Unregister(Messaging::ID::Attack, key);
	}

  void InputSUp(ObjectId obj, const Messaging::Message& message)
  {
    if (message.message.sjump.player == 1)
    {
      GameState* state =
        engine.Find<GameStateManager>()->CurrentState();
      GameObjectManager& manager = state->Manager();
      KPE::ForceRegistry& freg = engine.GetPhysicsEngine()->forceRegistry_;
      GameObject& box = manager.Find(obj);
      KPE::RigidBody* body = box.Find<KPE::RigidBody>();

      glm::vec2 anchorPos = body->GetParent().Find<Transform>()->getPosition();
      anchorPos.y -= 0.0f;

      float springC = 1.0f;
      float restL = 1.0f;
     
      if (message.message.sjump.key_state == Messaging::KeyState::Pressed) {
        //if (anchorFlip == false) {
        //  std::cout << "Create Anchor\n";
        //  anchorFlip = true;
        //  freg.add(body,
        //    new KPE::ForceAnchoredSpring(anchorPos, springC, restL));
        //}
        //else {
        //  std::cout << "Delete Anchor\n";
        //  anchorFlip = false;
        //  freg.remove(body, KPE::ForceType::FAnchorSpring);
        //}

        if (body->forcesAreActive()) {
          std::cout << "Deactivate Forces\n";
          body->forcesDeactivate();
        }
        else {
          std::cout << "Activate Forces\n";
          body->forcesActivate();
        }
      }
      else if (message.message.sjump.key_state == Messaging::KeyState::Held) {
        
      }
      else if (message.message.sjump.key_state == Messaging::KeyState::Released) {
      }
    }
  }

}


void DemoState::Init() {

	ObjectFactory::Create("data/json/DemoState.json", Manager());

	Manager().Find("Player1").Find<Graphics::Sprite>()->play("");

  //Physics
  KPE::RigidBody* body = Manager().Find("Player1").Find<KPE::RigidBody>();
  engine.GetPhysicsEngine()->forceRegistry_.add(
    body, new KPE::ForceHorizontalDrag(.001f, .99f));
  
  body = Manager().Find("Player2").Find<KPE::RigidBody>();
  engine.GetPhysicsEngine()->forceRegistry_.add(
    body, new KPE::ForceHorizontalDrag(.001f, .99f));

  //ForceEmitter
  GameObject fEmitter("Force Emitter", "PushLeft");
  KPE::ForceEmitterCircle* emitter = new KPE::ForceEmitterCircle(fEmitter);
  emitter->setRadius(250);
  
  KPE::ForceDirectionalPush* force =
    new KPE::ForceDirectionalPush(glm::vec2(0, 1), 100);
  emitter->setForce(KPE::ForceType::FDirection, force);
  
  fEmitter.Add(emitter);
  fEmitter.Find<Transform>()->setPosition(0, 1500);
  
  Graphics::CircleSprite* sprite = 
    new Graphics::CircleSprite(fEmitter, NULL, {500, 500}, 0.0f, 0, .7f, 
    Graphics::Color{1, 0, 0, 1});
  fEmitter.Add(sprite);
  
  Manager().Add(fEmitter);

	//audio test
	Audio::AudioManager *manager = engine.Find<Audio::AudioManager>(); //get manager
	manager->PlaySound("bigfedora");
	manager->SetMusicVolume(0.3f);

	//camera test
	GameObject cam("Camera", "cam");
	cam.Add(new Graphics::Camera(cam));
	cam.Find<Graphics::Camera>()->activate();
	cam.Find<Transform>()->setXScale(2);
	cam.Find<Transform>()->setYScale(2);
	Manager().Add(cam);

	//background test
	GameObject *bg1 = new GameObject("BackgroundImage", "bg1");
	bg1->Add(new Graphics::BackgroundImage(*bg1, "data/img/bg/bg1.png", 1.0f, 1.0f));
	bg1->Find<Transform>()->setXScale(4.0);
	bg1->Find<Transform>()->setYScale(4.0);
	Manager().Add(*bg1);
	GameObject *bg2 = new GameObject("BackgroundImage", "bg2");
	bg2->Add(new Graphics::BackgroundImage(*bg2, "data/img/bg/bg2.png", 2.0f, 0.5f));
	bg2->Find<Transform>()->setXScale(4.0);
	bg2->Find<Transform>()->setYScale(4.0);
	Manager().Add(*bg2);
	GameObject *bg3 = new GameObject("BackgroundImage", "bg3");
	bg3->Add(new Graphics::BackgroundImage(*bg3, "data/img/bg/bg3.png", 3.0f, 0.25f));
	bg3->Find<Transform>()->setXScale(4.0);
	bg3->Find<Transform>()->setYScale(4.0);
	Manager().Add(*bg3);

	//UISprite test
	GameObject ui("UI", "UISprite");
	ui.Add(new Graphics::UISprite(ui, "data/img/spr/test.png", Graphics::Size{ 64, 64 }, -100.0f));
	ui.Find<Transform>()->setPosition(-608, 328);
	Manager().Add(ui);

	
	Component* component = Manager().Find("Player1").Find<Transform>();
	Messaging::MessageHandler *m_handler = engine.Find<Messaging::MessageHandler>();
	m_handler->Register(Messaging::ID::MoveLeft, component->GetKey(), InputPlayer::InputLeft);
	m_handler->Register(Messaging::ID::MoveRight, component->GetKey(), InputPlayer::InputRight);
	m_handler->Register(Messaging::ID::Jump, component->GetKey(), InputPlayer::InputUp);
  m_handler->Register(Messaging::ID::SJump, component->GetKey(), InputPlayer::InputSUp);
	m_handler->Register(Messaging::ID::MenuSelect, nullobj, InputPlayer::InputEsc);
	m_handler->Register(Messaging::ID::Attack, component->GetKey(), InputPlayer::InputPunch);

  jumps = 0;
}

void DemoState::Update(float dt) {
	Manager().Update(dt);
	//float s = ((sin(elapsedTime) + 1.0f) / 2.0f) + 0.5f;
	//float s = 2.0f;
	if (Reset())
	{
		Reset(false);
		engine.Find<GameStateManager>()->ResetState();
	}

  glm::vec2 v1 = Manager().Find( "Player1" ).Find<Transform>()->getPosition();

	//Manager().Find("cam").Find<Transform>()->setPosition(Manager().Find("box").Find<Transform>()->getPosition());
  Manager().Find("cam").Find<Transform>()->setPosition(v1.x, v1.y);

}

void DemoState::Shutdown() {
	//trace_handler->TraceMessage(INFO, "Left pressed %i times", trace_counter);

	engine.GetPhysicsEngine()->forceRegistry_.clear();

  Component* component = Manager().Find("Player1").Find<Transform>();
	Messaging::MessageHandler *m_handler = engine.Find<Messaging::MessageHandler>();
	m_handler->Unregister(Messaging::ID::MoveLeft, component->GetKey());
	m_handler->Unregister(Messaging::ID::MoveRight, component->GetKey());
	m_handler->Unregister(Messaging::ID::Jump, component->GetKey());
	m_handler->Unregister(Messaging::ID::MenuSelect, nullobj);
	m_handler->Unregister(Messaging::ID::Attack, component->GetKey());

	Audio::AudioManager *manager = engine.Find<Audio::AudioManager>(); //get manage
	manager->StopAllSound();

	Manager().Serialize("data/json/jsoniguess.json");
	Manager().Shutdown();
}