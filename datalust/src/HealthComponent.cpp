/*************************************************************************/
/*!
\file   HealthComponent.cpp
\author studio 200
\par
\par    DigiPen login:
\par    Course: GAME 200
\par   Copyright � 2017 DigiPen (USA) Corp. and its owners. All Rights Reserved.
\date   9/28/2017
\brief
Functions include:
*/
/*************************************************************************/
#pragma once

#include "GameObject/GameObject.h"
#include "Physics/ParticleSystem.hpp"
#include "Physics/ParticleEmitter.hpp"
#include "Graphics/Sprite.hpp"
#include "BulletType.h"
#include "HealthComponent.h"
#include <SFML/Graphics/Transform.hpp>
#include "Transform/Transform.hpp"
#include "Physics/RigidBody.hpp"
#include "GameObject/ObjectFactory.h"
#include "Behaviors/ParticleFadeout.hpp"
#include "Behaviors/FadeOut.hpp"
#include "CoinBehavior.h"
#include "GameStateManager/GameStateManager.h"
#include "Engine/Engine.h"
#include "Graphics/Camera.hpp"

/**
 * \brief 
 * \param parent 
 * \param type 
 * \param health 
 * \param bt 
 */
HealthComponent::HealthComponent(GameObject& parent, HealthComponentType type, int health, BulletType bt) : Component(parent),
_type(type),
_health(health),
_MAXHEALTH(health),
gainHealthWrongHit(false),
currcolor((type == ENEMY) ? ((RandomRange(0, 1) >= 0.5f) ? RED : BLUE) : bt),
nextcolor(currcolor),
colorFade(BulletColors[NONE], BulletColors[currcolor], 0.33f)
{
  godMode_ = false;

  if (_type != PLAYER)
  {
    AddColorsToList(_health - 1);
    HintNextColor();
  }
}


/**
 * \brief 
 */
HealthComponent::~HealthComponent()
{
}

/**
 * \brief 
 * \return 
 */
int HealthComponent::GetHealth() const
{
  return _health;
}

/**
 * \brief 
 */
void HealthComponent::drop_coins()
{
  int coint_amount = 2;

  glm::vec2 pos = this->GetParent().Find<Transform>()->getPosition();

  std::vector<std::reference_wrapper<GameObject>> coins;

  GameObjectManager& manager = engine.Find<GameStateManager>()->CurrentState()->Manager();

  for (int i = 0; i < coint_amount; ++i)
  {
    std::vector<std::reference_wrapper<GameObject>> coins = ObjectFactory::Create("data/json/Coin.json", manager);
    GameObject& dropcoin = coins[0].get();
  
    CoinBehavior* c = dropcoin.Find<CoinBehavior>();

    if (c == nullptr)
      continue;

    c->SetBullet(BulletType::COINS);

    pos +=  ((rand() % 100) * 3.14f);
    dropcoin.Find<Transform>()->setPosition(pos);

    c->scatter();
  }

#if 0
  for (auto && coin : coins)
  {
    auto && Coin = coin.get();

    CoinBehavior * c = nullptr;

    c = Coin.;

    if (c == nullptr)
      continue;

    c->scatter();

    Coin.Find<Transform>()->setPosition(pos);

    Graphics::CircleSprite*_sprite = Coin.Find<Graphics::CircleSprite>();
    if (_sprite == nullptr) continue;

    KPE::ParticleSystem* _particleSys = Coin.Find<KPE::ParticleSystem>();
    if (_particleSys == nullptr) continue;

    c->SetBullet(BulletType::COINS);
  }
#endif
}

/**
 * \brief 
 */
void HealthComponent::dmgFade()
{
  //set visibility flag to false;
}

/**
 * \brief 
 */
void HealthComponent::checkhealth()
{
  if (_health <= 0)
  {

    if (_type == ENEMY)
    {
      Transform * enemyTransform = GetParent().Find<Transform>();
      GameObject EnemySplosion = ObjectFactory::ArchetypeGameObject("data/json/EnemySplosion.json", enemyTransform->getPosition());
      KPE::ParticleSystem * particle = EnemySplosion.Find<KPE::ParticleSystem>();
      particle->setBlendColor(BulletColors[currcolor]);
      KPE::ParticleEmitter * emitter = EnemySplosion.Find<KPE::ParticleEmitter>();
      glm::vec2  power = emitter->getPowerVariance();
      auto transform = GetParent().Find<Transform>();
      glm::vec2 scale{ transform->getXScale(), transform->getYScale() };
      emitter->setPowerVariance(power * scale);
      emitter->setMaxCount(static_cast<unsigned>(GetParent().Find<Transform>()->getXScale() * emitter->getMaxCount()));
      EnemySplosion.Find<ParticleFadeOut>()->setColor(BulletColors[currcolor]);
      engine.Find<GameStateManager>()->CurrentState()->Manager().Add(std::move(EnemySplosion));

      Messaging::Message m;
      m.id = Messaging::ID::EnemyDeath;
      engine.Find<Messaging::MessageHandler>()->Post(m);

      drop_coins();
    }
    else if(_type == DOOR)
    {
      Transform * doorTransform = GetParent().Find<Transform>();

      GameObject fade = ObjectFactory::ArchetypeGameObject("data/json/WallFade.json", doorTransform->getPosition());
      
      Transform * fTransfrom = fade.Find<Transform>();
      fTransfrom->setXScale(doorTransform->getXScale());
      fTransfrom->setYScale(doorTransform->getYScale());

      FadeOut * fadeOut = fade.Find<FadeOut>();
      fadeOut->setColor(BulletColors[currcolor]);

      engine.Find<GameStateManager>()->CurrentState()->Manager().Add(std::move(fade));
    }

    GetParent().Destroy();
    return;
  }
}

/**
 * \brief 
 * \param amt 
 */
void HealthComponent::AddHealth(int amt)
{
  { 
    _health += amt;
    sendDMGEvent();
  };
}

/**
 * \brief 
 */
void HealthComponent::playerdiesactions()
{
  ObjectFactory::Create("data/json/PlayerDies.json", engine.Find<GameStateManager>()->CurrentState()->Manager());
  GetParent().DeActivate();
  engine.Find<GameStateManager>()->DelayedPopState();
  engine.Find<GameStateManager>()->DelayedPushState(1);
  return;
}

/**
 * \brief 
 * \param dmg 
 * \param bulletcolor 
 */
void HealthComponent::DoDamage(int dmg, BulletType bulletcolor)
{
  if (_type == HealthComponentType::PLAYER)
  {
    if (godMode_) {
      return;
    }

    if (_health > 0)
    {
      dmgFade();
      if (healthhittimer <= 0) {
        _health -= dmg;
        sendDMGEvent();
        healthhittimer = delay;

        //set visbility fade off
      }
    }
    // instead of destroying the player, 
    // trigger a "OnPlayerDeath" event - PEC

    else
    {
      playerdiesactions();
      return;
    }
  }
  else if (currcolor == bulletcolor)
  {
    _health -= dmg;
    sendDMGEvent();

    checkhealth();
    if (!(_type == PLAYER))
      nextState = State::ChangeColors;
  }
  else if (gainHealthWrongHit)
  {
    if (_health < _MAXHEALTH)
    {
      AddColorsToList(1);
      _health += dmg;
      sendDMGEvent();

      // if there is a color to now hint
      if (colors.size() == 1)
      {
        HintNextColor();
      }
    }
  }

}

/**
 * \brief 
 * \return 
 */
BulletType HealthComponent::GetColor() const
{
  return currcolor;
}

/**
 * \brief 
 */
void HealthComponent::HintNextColor()
{
  KPE::ParticleEmitter * emitter = GetParent().Find <KPE::ParticleEmitter>();
  if (emitter != nullptr)
  {
    if (!colors.empty())
    {
      emitter->setMaxCount(0); // infinite
      nextcolor = colors.front();
    }
    else // there is no color to hint
    {
      emitter->setMaxCount(1); // essentially deactivates emitter
      nextcolor = NONE;
    }
  }
}

/**
 * \brief 
 * \return 
 */
Component* HealthComponent::Clone_() const
{
  return new HealthComponent(*this);
}

/**
 * \brief 
 * \param dt 
 */
void HealthComponent::Update(float dt)
{
  if (_type == PLAYER)
  {
    PlayerUpdate(dt);
    return;
  }

  if (currentState != nextState)
  {
    currentState = nextState;
    StateStart();
  }

  StateUpdate(dt);

  Graphics::Sprite *sprite = GetParent().Find<Graphics::Sprite>();
  if (sprite == nullptr)
    return;

  sprite->setBlendColor(colorFade.interp());
}

/**
 * \brief 
 */
void HealthComponent::Start()
{
  sendDMGEvent();
}

/**
 * \brief 
 */
void HealthComponent::Draw() const
{
}

/**
 * \brief 
 * \param count 
 */
void HealthComponent::AddColorsToList(int count)
{
  //std::cout << "Adding " << count << " Colors\n";

  // Loop as many times as given.
  for (int i = 0; i < count; ++i)
  {
    // generate a number from 0 to 1
    const float random = RandomRange(0.0f, 1.0f);
    //std::cout << random << ' ';

    // coin flip whether to set the color red or blue
    const BulletType type = ((random >= 0.5f) ? RED : BLUE);
    //std::cout << ((type == RED) ? "RED" : "BLUE") << ((count - i > 1) ? ", " : "");

    colors.emplace(type);
  }
  //std::cout << '\n';

}

/**
 * \brief 
 */
void HealthComponent::IdleStart()
{
  colorFade = ColorFade(BulletColors[NONE], BulletColors[currcolor], 1.0f / 16.0f);
  HintNextColor();
}

/**
 * \brief 
 * \param dt 
 */
void HealthComponent::IdleUpdate(float dt)
{
  colorFade.fade(dt);

  KPE::ParticleSystem * system = GetParent().Find<KPE::ParticleSystem>();
  if (system == nullptr) return;
  system->setBlendColor(colorFade.interp(BulletColors[NONE], BulletColors[nextcolor]));
}

/**
 * \brief 
 */
void HealthComponent::ChangeColorsStart()
{
  colorFade = ColorFade(BulletColors[currcolor], BulletColors[NONE], 1.0f / 16.0f);

  if (_type == DOOR)
    return;

  if (colors.empty())
    return;

  currcolor = colors.front();

  colors.pop();
}

/**
 * \brief 
 * \param dt 
 */
void HealthComponent::ChangeColorsUpdate(float dt)
{
  if (colorFade.fade(dt))
    nextState = State::Idle;

  KPE::ParticleSystem * system = GetParent().Find<KPE::ParticleSystem>();
  if (system == nullptr) return;
  system->setBlendColor(colorFade.interp(BulletColors[nextcolor], BulletColors[NONE]));
}

/**
 * \brief 
 * \param dt 
 */
void HealthComponent::PlayerUpdate(float dt)
{
  // while in invuln frames
  if (healthhittimer > 0)
  {
    // update timers
    healthhittimer -= dt;
    flashTimer -= dt;

    // if time to flash
    if (flashTimer <= 0)
    {
      // interpolate flash delay w.r.t. remaining invuln duration
      const float t = healthhittimer / delay;
      flashTimer = (0.33f + 0.67f * t) * flashDelay;
      // flip-flop colors
      redFlash = !redFlash;
      KPE::ParticleSystem * particles = GetParent().Find<KPE::ParticleSystem>();
      if (particles != nullptr)
      {
        if (redFlash)
        {
          particles->setBlendColor(Graphics::Color{});
        }
        else
        {
          particles->setBlendColor(Graphics::Color{ 0.175f, 0, 0.35f, 1 });
        }
      }
    }

  }
  else
  {
    // TODO, use flag and set once
    KPE::ParticleSystem * particles = GetParent().Find<KPE::ParticleSystem>();
    if (particles != nullptr)
    {
      particles->setBlendColor(Graphics::Color{ 0.175f, 0, 0.35f, 1 });
    }
  }

}


/**
 * \brief 
 */
void HealthComponent::StateStart(void)
{
  switch (currentState)
  {
  case State::Idle:
    IdleStart();
    break;

  case State::ChangeColors:
    ChangeColorsStart();
    break;

  default:
    break;
  }
}

/**
 * \brief 
 * \param dt 
 */
void HealthComponent::StateUpdate(float dt)
{
  switch (currentState)
  {
  case State::Idle:
    IdleUpdate(dt);
    break;

  case State::ChangeColors:
    ChangeColorsUpdate(dt);
    break;

  default:
    break;
  }
}

/**
 * \brief 
 */
void HealthComponent::sendDMGEvent()
{
  Messaging::Message m;
  m.id = Messaging::ID::Damage;
  ObjectId Key = GetParent().GetKey();
  m.message.damage.Player = &Key;
  engine.Find<Messaging::MessageHandler>()->Post(m);
}

/**
 * \brief 
 */
void HealthComponent::activateGodMode_() {
  godMode_ = true;
  
}

/**
 * \brief 
 */
void HealthComponent::deactivateGodMode_() {
  godMode_ = false;
}

