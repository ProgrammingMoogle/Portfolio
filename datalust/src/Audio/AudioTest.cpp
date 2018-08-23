/**
  \file   AudioTest.cpp
  \author Taylor Osmond

  \brief Implementation of audio test

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Audio.hpp"
#include <GameObject/SlotMap.h>
#include <Engine/Engine.h>
#include "AudioTest.hpp"
#include <iostream>

#include <GameObject/GameObject.h>
#include <Transform/Transform.hpp>
#include <Physics/Colliders.hpp>
#include <Physics/RigidBody.hpp>
#include <GameObject/Component.h>
#include <Physics/ForceRegistry.hpp>

/**
 * \brief 
 * \param levels 
 */
AudioTest::AudioTest(std::string levels) : GameState(levels){}

/**
 * \brief 
 */
void AudioTest::Init()
{
	//TEMP STUFF UNTIL COLLIDERS DONT BREAK EVERYTHING

	// create a object
	GameObject* box = new GameObject("Player", "Player1");

	// add components to the Object
	box->Find<Transform>()->setOrientation(0);
	auto body_ = new KPE::RigidBody(*box);
	auto colliders_ = new KPE::Colliders(*box);
	box->Add(colliders_);
	box->Add(body_);
	colliders_->add(KPE::BoundType::BoundBox, box->Find<Transform>(), 64, 64);

	// add the object to the manager
	Manager().Add(*box);



	Audio::AudioManager *manager = engine.Find<Audio::AudioManager>(); //get manager
	manager->PlaySound("AFIS");
}

/**
 * \brief 
 * \param dt 
 */
void AudioTest::Update(float dt) 
{
	Manager().Update(dt);
}