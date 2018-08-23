/**
  \file   PunchBehaviors.cpp
  \author Zayd Gaudet

  \brief Behavior component for punches

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include <Engine/Engine.h>
#include <Transform/Transform.hpp>
#include "Player1Behavior.hpp"
#include "Player2Behavior.hpp"
#include "PunchBehaviors.hpp"
#include <Audio/Audio.hpp>

/**
 * \brief 
 * \param parent 
 * \param dmg 
 * \param scalar 
 */
PunchBehavior::PunchBehavior(GameObject &parent, float dmg, float scalar) : Component(parent), dmg_(dmg), scalar_(scalar)
{
	Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
	m_handler->Register(Messaging::ID::AnimationUpdate, SetKey(parent.GetKey()), PunchBehavior::Punch);
	m_handler->Register(Messaging::ID::Collision, GetKey(), PunchBehavior::Collision);
}

/**
 * \brief 
 */
PunchBehavior::~PunchBehavior()
{
	Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
	ObjectId key = GetKey();
	m_handler->Unregister(Messaging::ID::AnimationUpdate, key);
	m_handler->Unregister(Messaging::ID::Collision, key);
}

/**
 * \brief 
 * \param player 
 */
void PunchBehavior::AttachPlayer(ObjectId player)
{
	player_ = player;
	ptransform = engine.Find<GameStateManager>()->CurrentState()->Manager().Find(player).Find<Transform>();
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void PunchBehavior::Collision(ObjectId obj, const Messaging::Message& message)
{
	GameState* state = engine.Find<GameStateManager>()->CurrentState();
	GameObjectManager& manager = state->Manager();
	GameObject& object = manager.Find(obj);
	object.Find<PunchBehavior>()->onCollision(message);
}

/**
 * \brief 
 * \param message 
 */
void PunchBehavior::onCollision(const Messaging::Message& message)
{
#if 0
	if (message.message.collision.hit_
		&& message.message.collision.body_->GetParent() == GetParent()
		&& message.message.collision.otherBody_->GetKey() != player_)
	{
		if (collided == false) 
		{
		int dmg = 0;
		//Messaging::MessageHandler* mHandler = engine.Find<Messaging::MessageHandler>();
		//Messaging::Message msg;
		//msg.id = Messaging::ID::Damage;
		//msg.message.damage.Player = &message.message.collision.otherBody_->GetKey();
		Player1Behavior * p1 = message.message.collision.otherBody_->GetParent().Find<Player1Behavior>();
		if (p1)
		{	
			p1->AddToDMG(dmg_);
			dmg = scalar_ * p1->DMG();
		}
		else
		{
			Player2Behavior * p2 = message.message.collision.otherBody_->GetParent().Find<Player2Behavior>();
			if (p2)
			{
				p2->AddToDMG(dmg_);
				dmg = scalar_ * p2->DMG();
			}
		}

		message.message.collision.otherBody_->addForce(glm::vec2{ dmg*(200 * -message.message.collision.normal_.x), dmg*100 });
		//mHandler->Post(msg);


			Audio::AudioManager *audio_manager = engine.Find<Audio::AudioManager>();

			int sound = audio_manager->PlayRandomSound({
        std::make_tuple( "Hit_DeepMetalKick", 0, 0.3f ),
        std::make_tuple( "Hit_BodyImpactWall", 0, 0.03f ),
        std::make_tuple( "Crash_BodyHitMetalGate", 0, 0.3f ) });

			switch (sound)
			{
			case 0:
				audio_manager->SetSoundPitch(2.5f, "Hit_DeepMetalKick");
				break;
			case 1:
				audio_manager->SetSoundPitch(2.5f, "Hit_BodyImpactWall");
				break;
			case 2:
 				audio_manager->SetSoundPitch(1.5f, "Crash_BodyHitMetalGate");
				break;
			}

			collided = true;
		}
	}
#endif
}
 
/**
 * \brief 
 * \param obj 
 * \param message 
 */
void PunchBehavior::Punch(ObjectId obj, const Messaging::Message& message)
{
	GameState* state = engine.Find<GameStateManager>()->CurrentState();
	GameObjectManager& manager = state->Manager();
	GameObject& object = manager.Find(obj);
	object.Find<PunchBehavior>()->onPunch(message);
}

/**
 * \brief 
 * \param message 
 */
void PunchBehavior::onPunch(const Messaging::Message& message)
{
	std::string name = message.message.animation_update.animation;
	  
	if (name == "punch" && 
		engine.Find<GameStateManager>()->CurrentState()->Manager().Find(player_).Find<Graphics::Sprite>()
		== message.message.animation_update.sprite)
	{
		frame = message.message.animation_update.frame;
	}
}

/**
 * \brief 
 * \param dt 
 */
void PunchBehavior::Update(float dt)
{
	if (player_ != nullobj)
	{
		// player stuff
		glm::vec2 pos = ptransform->getPosition();
		float xscale = ptransform->getXScale();		
		// punch stuff

		
		switch (frame)
		{
		case 0:
 			GetParent().Find<Transform>()->setPosition(glm::vec2{ pos.x + 15 * xscale, pos.y });
			break;
		case 1:
			GetParent().Find<Transform>()->setPosition(glm::vec2{ pos.x + 15 * xscale, pos.y });
			break;
		case 2:
			GetParent().Find<Transform>()->setPosition(glm::vec2{ pos.x + 15 * xscale, pos.y });
			break;
		case 3:
			GetParent().Find<Transform>()->setPosition(glm::vec2{ pos.x + 30 * xscale, pos.y });
			break;
		case 4:
			GetParent().Find<Transform>()->setPosition(glm::vec2{ pos.x + 28 * xscale, pos.y });
			break;
		case 5:
			GetParent().Find<Transform>()->setPosition(glm::vec2{ pos.x + 28* xscale, pos.y });
			break;
		case 6:
			GetParent().Find<Transform>()->setPosition(glm::vec2{ pos.x + 50 * xscale, pos.y });
			break;
		case 7:
			GetParent().Find<Transform>()->setPosition(glm::vec2{ pos.x + 46 * xscale, pos.y });
			break;
		case 8:
			GetParent().Destroy();
			break;
		}
	}
}