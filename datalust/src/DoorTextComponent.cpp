/*************************************************************************/
/*!
\file   DoorTextComponent.cpp
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
#include "DoorTextComponent.h"
#include "Engine/Engine.h"
#include "HealthComponent.h"
#include "Graphics/TextSprite.hpp"

/**
 * \brief 
 * \param parent 
 */
DoorTextComponent::DoorTextComponent(GameObject& parent): Component(parent)
{ 
}


/**
 * \brief 
 */
void DoorTextComponent::Start()
{
  Messaging::MessageHandler * m = engine.Find < Messaging::MessageHandler>();
  ObjectId const& key = GetParent().GetKey();
  m->Register(Messaging::ID::Damage, key, DoorTextComponent::HandleDamage);

  UpdateHealth();
}

/**
 * \brief 
 * \param id 
 * \param damage 
 */
void DoorTextComponent::HandleDamage(ObjectId id, Messaging::Message const& damage)
{
  if (id == *damage.message.damage.Player)
  {
    engine.Find<GameStateManager>()->CurrentState()->Manager()
      .Find(id).Find<DoorTextComponent>()->UpdateHealth();
  }
}

/**
 * \brief 
 */
void DoorTextComponent::UpdateHealth()
{
  auto health = GetParent().Find<HealthComponent>();
  if (health == nullptr) return;
  GetParent().Find<Graphics::WorldTextSprite>()->setDisplayNumber(health->GetHealth());
}


/**
 * \brief 
 */
DoorTextComponent::~DoorTextComponent()
{
  Messaging::MessageHandler * m = engine.Find <Messaging::MessageHandler>();
  ObjectId const& key = GetParent().GetKey();
  m->Unregister(Messaging::ID::Damage, key);
}