/**
  \file   BoundryBehavior.cpp
  \author Samuel Cook

  \brief Behavior component for camera boundries

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BoundryBehavior.hpp"
#include <Engine/Engine.h>
#include <GameStateManager/GameStateManager.h>
#include <Physics/RigidBody.hpp>
#include <Transform/Transform.hpp>
#include <Behaviors/CameraBehavior.hpp>

/**
 * \brief 
 */
float BoundryBehavior::break_speed_sqr_ = 20.0f;

/**
 * \brief 
 * \param parent 
 * \param player 
 */
BoundryBehavior::BoundryBehavior(GameObject& parent, ObjectId player) : Component(parent), player_(player) {
	GetParent().Find<Graphics::Sprite>()->setBlendColor({ 1, 1, 0, 0.5 });
	Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
	ObjectId key = SetKey(GetParent().GetKey());
	m_handler->Register(Messaging::ID::Collision, key, BoundryBehavior::CollisionHandler);
}

/**
 * \brief 
 */
BoundryBehavior::~BoundryBehavior()
{
	Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
	m_handler->Unregister(Messaging::ID::Collision, GetKey());
}


/**
 * \brief 
 * \param dt 
 */
void BoundryBehavior::Update(float dt) {
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void BoundryBehavior::CollisionHandler(ObjectId obj, const Messaging::Message& message) {
	GameState* state = engine.Find<GameStateManager>()->CurrentState();
	GameObjectManager& manager = state->Manager();
	try {
		GameObject& object = manager.Find(obj);
		if (object.GetType() == "Boundry") {
			object.Find<BoundryBehavior>()->onCollisionHandle(message);
		}
	}
	catch (...) {}
}


/**
 * \brief 
 * \param message 
 */
void BoundryBehavior::onCollisionHandle(const Messaging::Message& message)
{
	if (message.message.collision.otherBody_ == GetParent().Find<KPE::RigidBody>()) 
	{
		if (message.message.collision.body_->GetParent().GetKey() == player_) {
			GameObjectManager &Manager = engine.Find<GameStateManager>()->CurrentState()->Manager();
			GameObject& player = message.message.collision.body_->GetParent();
			GameObject& boundry = GetParent();
			glm::vec2 velocity = player.Find<KPE::RigidBody>()->getVelocity();
			float speed_sqr = velocity.x * velocity.x + velocity.y * velocity.y;
			if (speed_sqr > break_speed_sqr_) {
				*(message.message.collision.pass_) = false;
			}
			GameObject& cam = Manager.Find("cam");
			CameraBehavior* cBehav = cam.Find<CameraBehavior>();
			bool scroll = cBehav->scrollLeft() || cBehav->scrollRight();

			if (scroll) {
				*(message.message.collision.pass_) = false;
			}
		}
	}
}
