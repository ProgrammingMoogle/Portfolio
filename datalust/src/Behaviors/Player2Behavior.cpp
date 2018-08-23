/**
  \file   Player2Behavior.cpp
  \author Zayd Gaudet

  \brief Behavior component for player 2

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Player2Behavior.hpp"
#include "PunchBehaviors.hpp"
#include <Engine/Engine.h>
#include <Messaging/Messaging.hpp>
#include <Transform/Transform.hpp>
#include <Graphics/Sprite.hpp>
#include <Behaviors/HUDBehavior.hpp>
#include <Audio/Audio.hpp>

Player2Behavior::Player2Behavior(GameObject &parent) : Component(parent) {
	Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
	ObjectId key = GetParent().GetKey();
	m_handler->Register(Messaging::ID::MoveLeft, key, Player2Behavior::InputLeft);
	m_handler->Register(Messaging::ID::MoveRight, key, Player2Behavior::InputRight);
	m_handler->Register(Messaging::ID::Jump, key, Player2Behavior::InputJump);
	m_handler->Register(Messaging::ID::Attack, key, Player2Behavior::InputPunch);
	m_handler->Register(Messaging::ID::AnimationUpdate, key, Player2Behavior::AnimationUpdate);
}

Player2Behavior::~Player2Behavior() {
	Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
	ObjectId key = GetParent().GetKey();
	m_handler->Unregister(Messaging::ID::MoveLeft, key);
	m_handler->Unregister(Messaging::ID::MoveRight, key);
	m_handler->Unregister(Messaging::ID::Jump, key);
	m_handler->Unregister(Messaging::ID::Attack, key);
	m_handler->Unregister(Messaging::ID::AnimationUpdate, key);
}

void Player2Behavior::Update(float dt) {
  try {
    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find( "P2Hud" );
    HUDBehavior* b = object.Find<HUDBehavior>();
    if ( b )
      b->setPlayer( GetParent().GetKey() );
  }
	catch (...) {}
}

void Player2Behavior::InputLeft(ObjectId obj, const Messaging::Message & message) {
	GameState* state = engine.Find<GameStateManager>()->CurrentState();
	GameObjectManager& manager = state->Manager();
	GameObject& object = manager.Find(obj);
	if (object.GetName() == "Player2" && message.message.move_left.player == 2) {
		object.Find<Player2Behavior>()->onInputLeft(message);
	}
}

void Player2Behavior::onInputLeft(const Messaging::Message & message) {
	if (!isPunching_) {

		KPE::RigidBody* body = GetParent().Find<KPE::RigidBody>();
		Transform* transform = GetParent().Find<Transform>();
		Graphics::Sprite* sprite = GetParent().Find<Graphics::Sprite>();
		glm::vec2 velocity = body->getVelocity();

		if (message.message.move_left.key_state == Messaging::KeyState::Pressed ||
			message.message.move_left.key_state == Messaging::KeyState::Held) {
			if (facing_ == right) {
				facing_ = left;
				transform->setXScale(-GetParent().Find<Transform>()->getXScale());
			}

			if (state_ != walking) {
				if (state_ != jumping) {
					sprite->play("walk");
					state_ = walking;
				}
			}

			velocity.x = -GetParent().MoveVal();
			body->setVelocity(velocity);

		}
		if (message.message.move_left.key_state == Messaging::KeyState::Released) {
			if (state_ = walking) {
				velocity.x = 0;
				body->setVelocity(velocity);
				sprite->play("idle");
				state_ = idle;
			}
		}
	}
}

void Player2Behavior::InputRight(ObjectId obj, const Messaging::Message & message) {
	GameState* state = engine.Find<GameStateManager>()->CurrentState();
	GameObjectManager& manager = state->Manager();
	GameObject& object = manager.Find(obj);
	if (object.GetName() == "Player2" && message.message.move_right.player == 2) {
		object.Find<Player2Behavior>()->onInputRight(message);
	}
}

void Player2Behavior::onInputRight(const Messaging::Message & message) {
	if (!isPunching_) {

		KPE::RigidBody* body = GetParent().Find<KPE::RigidBody>();
		Transform* transform = GetParent().Find<Transform>();
		Graphics::Sprite* sprite = GetParent().Find<Graphics::Sprite>();
		glm::vec2 velocity = body->getVelocity();

		if (message.message.move_right.key_state == Messaging::KeyState::Pressed ||
			message.message.move_right.key_state == Messaging::KeyState::Held) {
			if (facing_ == left) {
				facing_ = right;
				transform->setXScale(-GetParent().Find<Transform>()->getXScale());
			}

			if (state_ != walking) {
				if (state_ != jumping) {
					sprite->play("walk");
					state_ = walking;
				}
			}

			velocity.x = GetParent().MoveVal();
			body->setVelocity(velocity);

		}
		if (message.message.move_right.key_state == Messaging::KeyState::Released) {
			if (state_ = walking) {
				velocity.x = 0;
				body->setVelocity(velocity);
				sprite->play("idle");
				state_ = idle;
			}
		}
	}
}

void Player2Behavior::InputJump(ObjectId obj, const Messaging::Message & message) {
	GameState* state = engine.Find<GameStateManager>()->CurrentState();
	GameObjectManager& manager = state->Manager();
	GameObject& object = manager.Find(obj);
	if (object.GetName() == "Player2" && message.message.jump.player == 2) {
		object.Find<Player2Behavior>()->onInputJump(message);
	}
}

void Player2Behavior::onInputJump(const Messaging::Message & message) {
	if (!isPunching_) {

		KPE::RigidBody* body = GetParent().Find<KPE::RigidBody>();
		Transform* transform = GetParent().Find<Transform>();
		Graphics::Sprite* sprite = GetParent().Find<Graphics::Sprite>();
		glm::vec2 velocity = body->getVelocity();
		Audio::AudioManager *audio_manager = engine.Find<Audio::AudioManager>();

		if (message.message.jump.key_state == Messaging::KeyState::Pressed) {
			bool onGround = GetParent().Find<KPE::Colliders>()->raycastFromTrans(glm::vec2(0, -1), 136);
			if (onGround) {
				jumps_ = 1;

				sprite->play("jump");

				audio_manager->PlaySequence({
					{ "Lift_SmallPlatformStart01", 0 },
					{ "jump_1", 0 } },
					false);
				audio_manager->SetSoundVolume(0.3f, "Lift_SmallPlatformStart01");
				audio_manager->SetSoundVolume(2.0f, "jump_1");
				audio_manager->SetSoundPitch(((rand() / (float)RAND_MAX) * 0.4f) + 0.8f, "jump_1");

				velocity.y = GetParent().JumpVal();
				body->setVelocity(velocity);
			}
			else if (jumps_ > 0) {
				sprite->play("jump");

				audio_manager->PlaySequence({
					{ "Lift_SmallPlatformStart01", 0 },
					{ "jump_1", 0 } },
					false);
				audio_manager->SetSoundVolume(0.3f, "Lift_SmallPlatformStart01");
				audio_manager->SetSoundVolume(2.0f, "jump_1");
				audio_manager->SetSoundPitch(((rand() / (float)RAND_MAX) * 0.4f) + 0.8f, "jump_1");

				velocity.y = GetParent().JumpVal();
				body->setVelocity(velocity);

				--jumps_;
			}
		}
	}
}

void Player2Behavior::InputPunch(ObjectId obj, const Messaging::Message & message) {
	GameState* state = engine.Find<GameStateManager>()->CurrentState();
	GameObjectManager& manager = state->Manager();
	GameObject& object = manager.Find(obj);
	if (object.GetName() == "Player2" && message.message.attack.player == 2) {
		object.Find<Player2Behavior>()->onInputPunch(message);
	}
}

void Player2Behavior::onInputPunch(const Messaging::Message & message) {
	if (!isPunching_) {

		GameObject& obj = GetParent();
		Graphics::Sprite* sprite = obj.Find<Graphics::Sprite>();
		Audio::AudioManager *audio_manager = engine.Find<Audio::AudioManager>();

		if (message.message.attack.Mouse_state == Messaging::MouseState::Pressed) {
			GameObjectManager & manager = engine.Find<GameStateManager>()->CurrentState()->Manager();
			ObjectFactory::Create("data/json/Punch1.json", manager);
			// grab the punch object we just created
			GameObject& Punch = manager.Find("Punch1");
			Punch.Find<PunchBehavior>()->AtachPlayer(obj.GetKey());
			Punch.Find<PunchBehavior>()->Update(0);
			Punch.Update(0);
			isPunching_ = true;
			state_ = idle;
			sprite->play("punch");

			audio_manager->PlaySound("Magic_SpiritBlast_Ricochet4");
			audio_manager->SetSoundPitch(2.0f, "Magic_SpiritBlast_Ricochet4");
			audio_manager->SetSoundPitch(((rand() / (float)RAND_MAX)) + 1.0f, "Magic_SpiritBlast_Ricochet4");
		}
	}
}

void Player2Behavior::AnimationUpdate(ObjectId obj, const Messaging::Message & message) {
	GameState* state = engine.Find<GameStateManager>()->CurrentState();
	GameObjectManager& manager = state->Manager();
	GameObject& object = manager.Find(obj);
	if (object.GetName() == "Player2") {
		object.Find<Player2Behavior>()->onAnimationUpdate(message);
	}
}

void Player2Behavior::onAnimationUpdate(const Messaging::Message & message) {
	if (std::string(message.message.animation_update.animation) == "punch") {
		if (message.message.animation_update.is_playing == false)
			isPunching_ = false;
	}
}
