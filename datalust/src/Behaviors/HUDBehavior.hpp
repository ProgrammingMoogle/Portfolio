/**
  \file   HUDBehavior.hpp
  \author Samuel Cook

  \brief Behavior component for HUDs

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <GameObject/Component.h>
#include <Messaging/Messaging.hpp>
#include "GameObject/GameObjectManager.h"
#include "Player1Behavior.hpp"

/**
 * \brief 
 */
enum HUDTYPE  {HUDCoin = 0, HUDHealth = 1};

/**
 * \brief 
 */
class HUDBehavior : public Component {
public:
	HUDBehavior(GameObject& parent,HUDTYPE type );

  void setPlayerBehavior(GameObject& player) { player1_b_behavior_ = player.Find<Player1Behavior>();}
	virtual Component* Clone_() const { return new HUDBehavior(*this); }
	void Update(float dt);

private:
  HUDTYPE _type;
  GameObjectManager& manager;
  void calculateLife(GameObject* life);
  void calulateCoins(GameObject*coins);
  Player1Behavior* player1_b_behavior_ = nullptr;
};