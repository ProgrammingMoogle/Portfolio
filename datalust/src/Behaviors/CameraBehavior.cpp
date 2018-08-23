/**
  \file   CameraBehavior.cpp
  \author Samuel Cook

  \brief Behavior component for camera

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "CameraBehavior.hpp"
#include "Engine/Engine.h"
#include "Graphics\Camera.hpp"
#include "GameObject/GameObjectManager.h"
#include "Transform/Transform.hpp"
#include <Behaviors/BoundryBehavior.hpp>
#include <Physics/RigidBody.hpp>
#include <Physics/Colliders.hpp>
#include <Graphics/Sprite.hpp>
#include <Behaviors/Player1Behavior.hpp>
#include <Audio/Audio.hpp>
//#include <GameObject/ObjectFactory.h>

/**
 * \brief 
 * \param parent 
 */
CameraBehavior::CameraBehavior(GameObject& parent) : Component(parent) {
	boundry1 = CreateBoundry("Boundry1", "Player1");
}

/**
 * \brief 
 * \param dt 
 */
void CameraBehavior::Update(float dt) {

  Graphics::Camera * cam = GetParent().Find<Graphics::Camera>();

  if (cam == nullptr)
    return;

  GameObjectManager& manager = engine.Find<GameStateManager>()->CurrentState()->Manager();
  Transform * player = manager.Find("Player1").Find<Transform>();
  cam->setCenter(player->getPosition());
}

/**
 * \brief 
 * \param name 
 * \param player 
 * \return 
 */
ObjectId CameraBehavior::CreateBoundry(const std::string & name, const std::string & player) {
	GameObjectManager& manager = engine.Find<GameStateManager>()->CurrentState()->Manager();
  //auto Created = ObjectFactory::Create("data/json/Boundry.json", manager);
	//transform->setPosition( -1280, 720 );

  return /*(Created.empty()) ?*/ nullobj; //: Created.front().get().GetKey();
}
