/**
  \file   HUDBehavior.cpp
  \author Samuel Cook

  \brief Behavior component for HUDs

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "HUDBehavior.hpp"
#include <Engine/Engine.h>
#include <Behaviors/Player1Behavior.hpp>
#include <Graphics/Sprite.hpp>
#include <Transform/Transform.hpp>
#include "Graphics/TextSprite.hpp"

/**
 * \brief 
 * \param parent 
 * \param type 
 */
HUDBehavior::HUDBehavior(GameObject & parent, HUDTYPE type) : Component(parent), manager(engine.Find<GameStateManager>()->CurrentState()->Manager()), _type(type)
{
  setPlayerBehavior(manager.Find("Player1"));
}

/**
 * \brief 
 * \param life 
 */
void HUDBehavior::calculateLife(GameObject* life)
{
  //get health count
  int health = player1_b_behavior_->GetHealth();
  //display
  life->Find<Graphics::TextSprite>()->setDisplayNumber(health);
}

/**
 * \brief 
 * \param coins 
 */
void HUDBehavior::calulateCoins(GameObject* coins)
{
  //get coins
  int coincount = player1_b_behavior_->GetCoins();
  //display
  coins->Find<Graphics::TextSprite>()->setDisplayNumber(coincount);
  // Graphics::TextSprite cointext(*coins, coincount);
}

/**
 * \brief 
 * \param dt 
 */
void HUDBehavior::Update(float dt)
{
  /*wtf? is this stuff*/
#if 0
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();

  int dmg = 0;
  if (player_ != nullobj) {
    try {
      GameObject& player = manager.Find(player_);

      Player1Behavior* p1b = player.Find<Player1Behavior>();

      if (p1b) {
        dmg = p1b->DMG();
      }

    }
    catch (...) {}
  }
  dmg = (dmg > 1) ? 1 : dmg;
  GetParent().Find<Graphics::UICircleSprite>()->setBlendColor(Graphics::Color{ 1.0f * dmg, 1.0f * (1.0f - dmg), 0.0f, 1.0f });
#endif

  if (!GetParent().IsDestroyed())
  {
    if (_type == HUDHealth) {
      GameObject* life = &manager.Find("LifeCount");
      if (life != nullptr)
      {
        calculateLife(life);
      }
    }
    else if (_type == HUDCoin) {
      GameObject* count = &manager.Find("CoinCount");
      if (count != nullptr)
      {
        calulateCoins(count);
      }
    }
  }
}
