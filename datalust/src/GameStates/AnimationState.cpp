/**
  \file   AnimationState.cpp
  \author Samuel Cook

  \brief 

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "AnimationState.h"
#include <GameObject/GameObject.h>
#include <GameObject/GameObjectManager.h>
#include <Engine/Engine.h>
#include <Graphics/Graphics.hpp>
#include <Transform/Transform.hpp>
#include <Physics/Colliders.hpp>
#include <Physics/RigidBody.hpp>
#include <GameObject/Component.h>
#include <Physics/ForceRegistry.hpp>
#include <Audio/Audio.hpp>
#include <iostream>
#include <cstdlib>

#define NO_OVERLAY

//float pitch = 1.0f;

namespace AnimationState_ {
  const char* anim_list[] = {
    "idle",
    "walk",
    "punch",
    "jumpsquat"
  };

  int anim_index = 0;
  int anim_count = sizeof( anim_list ) / sizeof( anim_list[0] );

  void InputLeft(ObjectId obj, const Messaging::Message& message)
  {
    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& spr = manager.Find(obj);
    GameObject& other = manager.Find( "Red" );


    if (message.message.move_left.key_state == Messaging::KeyState::Pressed){

    }
    else if (message.message.move_left.key_state == Messaging::KeyState::Held){

    }
    else if (message.message.move_left.key_state == Messaging::KeyState::Released){
      --anim_index;
      if ( anim_index == -1 ) anim_index = anim_count - 1;
      spr.Find<Graphics::UISprite>()->play( anim_list[anim_index] );
      other.Find<Graphics::UISprite>()->play( anim_list[anim_index] );
    }
  }

  void InputEsc(ObjectId obj, const Messaging::Message& message) {
	  if (message.message.menu_select.key_state == Messaging::KeyState::Pressed)
		  engine.Find<GameStateManager>()->CurrentState()->Reset(true);
  }

  void InputRight(ObjectId obj, const Messaging::Message& message)
  {
    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state ->Manager();
    GameObject& spr = manager.Find(obj);
    GameObject& other = manager.Find( "Red" );

    if (message.message.move_right.key_state == Messaging::KeyState::Pressed) {

    }
    else if (message.message.move_right.key_state == Messaging::KeyState::Held) {

    }
    else if (message.message.move_right.key_state == Messaging::KeyState::Released) {
      ++anim_index;
      if ( anim_index == anim_count ) anim_index = 0;      
      spr.Find<Graphics::UISprite>()->play( anim_list[anim_index] );
      other.Find<Graphics::UISprite>()->play( anim_list[anim_index] );
    }
  }

  void onAnimationUpdate( ObjectId obj, const Messaging::Message& message ) {
    Audio::AudioManager *audio = engine.Find<Audio::AudioManager>();
    if ( message.message.animation_update.sprite->GetKey() == obj ) {
      std::cout << "animation_update" << std::endl;
      std::cout << "|- obj:        " << message.message.animation_update.sprite->GetParent().GetName() << std::endl;
      std::cout << "|- animation:  " << message.message.animation_update.animation << std::endl;
      std::cout << "|- frame:      " << message.message.animation_update.frame << std::endl;
      std::cout << "'- is_playing: " << message.message.animation_update.is_playing << std::endl << std::endl;

      message.message.animation_update.sprite->setBlendColor( Graphics::Color{ 0, 0, 0, 0 } );

      if ( message.message.animation_update.frame == 0 ) {
        message.message.animation_update.sprite->setBlendColor( Graphics::Color{ 0, 1, 0, 0.5 } );
      }

      if ( message.message.animation_update.is_playing == false ) {
        message.message.animation_update.sprite->setBlendColor( Graphics::Color{ 1, 0, 0, 0.5 } );
      }

      if ( std::string( message.message.animation_update.animation ) == "punch" ) {
        if ( message.message.animation_update.frame == 4 ) {
          switch ( rand() % 3 ) {
            case 0:
              audio->PlaySound( "punch_1" );
              audio->SetSoundPitch( ((rand() / (float)RAND_MAX) * 0.4f) + 0.8f, "punch_1" );
              break;
            case 1:
              audio->PlaySound( "punch_2" );
              audio->SetSoundPitch( ((rand() / (float)RAND_MAX) * 0.4f) + 0.8f, "punch_2" );
              break;
            case 2:
              audio->PlaySound( "punch_3" );
              audio->SetSoundPitch( ((rand() / (float)RAND_MAX) * 0.4f) + 0.8f, "punch_3" );
              break;
          }
        }
        if ( message.message.animation_update.frame >= 4 && message.message.animation_update.frame <= 7 ) {
          message.message.animation_update.sprite->setBlendColor( Graphics::Color{ 1, 1, 0, 0.5 } );
        }
      } else if ( std::string( message.message.animation_update.animation ) == "jumpsquat" ) {
        if ( message.message.animation_update.is_playing == false ) {
          audio->PlaySound( "jump_1" );
          
          audio->SetSoundPitch( ((rand() / (float)RAND_MAX) * 0.4f) + 0.8f, "jump_1" );
          message.message.animation_update.sprite->play( "jump" );
        }
      }

    #ifdef NO_OVERLAY
      message.message.animation_update.sprite->setBlendColor( Graphics::Color{ 0, 0, 0, 0 } );
    #endif

    }
  }
}


void AnimationState::Init() {

  //UISprite test
  GameObject *ui = new GameObject( "UI", "Blue" );
  ui->Add( new Graphics::UISprite( *ui, "data/img/spr/blue_tank.png", Graphics::Size{ 128, 128 }, -100.0f ) );
  ui->Find<Transform>()->setPosition(-200, 0);
  ui->Find<Transform>()->setXScale( 2);
  ui->Find<Transform>()->setYScale( 2);
  Manager().Add(*ui);

  ui = new GameObject( "UI", "Red" );
  ui->Add( new Graphics::UISprite( *ui, "data/img/spr/red_tank.png", Graphics::Size{ 128, 128 }, -100.0f ) );
  ui->Find<Transform>()->setPosition(200, 0);
  ui->Find<Transform>()->setXScale(-2);
  ui->Find<Transform>()->setYScale( 2);
  Manager().Add(*ui);

  GameObject *obj = new GameObject( "Circle", "Circle1" );
  obj->Add( new Graphics::CircleSprite( *obj, "data/img/spr/test.png", Graphics::Size{ 128, 128 }, 0.0f, 0, 1.0f, Graphics::Color{ 0.0, 1.0, 0.0, 0.5 } ));
  Manager().Add(*obj);

  //Input Testing
  Component *component = Manager().Find( "Blue" ).Find<Transform>();
  Messaging::MessageHandler *m_handler = engine.Find<Messaging::MessageHandler>();
  m_handler->Register(Messaging::ID::MoveLeft, component->GetKey(), AnimationState_::InputLeft);
  m_handler->Register(Messaging::ID::MoveRight, component->GetKey(), AnimationState_::InputRight);
  m_handler->Register(Messaging::ID::MenuSelect, nullobj, AnimationState_::InputEsc);
  m_handler->Register(Messaging::ID::AnimationUpdate, component->GetKey(), AnimationState_::onAnimationUpdate);

  component = Manager().Find( "Red" ).Find<Transform>();
  m_handler->Register(Messaging::ID::AnimationUpdate, component->GetKey(), AnimationState_::onAnimationUpdate);

  Manager().Find( "Blue" ).Find<Graphics::UISprite>()->play( AnimationState_::anim_list[AnimationState_::anim_index] );
  Manager().Find( "Red" ).Find<Graphics::UISprite>()->play( AnimationState_::anim_list[AnimationState_::anim_index] );
}

void AnimationState::Update(float dt) {
  Manager().Update(dt);
  if (Reset())
  {
	  Reset(false);
	  engine.Find<GameStateManager>()->ResetState();
  }
}

void AnimationState::Shutdown() {
  Component *component = Manager().Find( "Blue" ).Find<Transform>();
	Messaging::MessageHandler *m_handler = engine.Find<Messaging::MessageHandler>();
	m_handler->Unregister(Messaging::ID::MoveLeft, component->GetKey());
	m_handler->Unregister(Messaging::ID::MoveRight, component->GetKey());  
	m_handler->Unregister(Messaging::ID::MenuSelect, nullobj);
	m_handler->Unregister(Messaging::ID::AnimationUpdate, component->GetKey());

  component = Manager().Find( "Red" ).Find<Transform>();
  m_handler->Unregister(Messaging::ID::AnimationUpdate, component->GetKey());

	Manager().Shutdown();
}


