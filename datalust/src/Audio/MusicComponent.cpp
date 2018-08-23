/**
  \file   MusicComponent.cpp
  \author Taylor Osmond

  \brief Implementation of audio manager system

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/

#include "MusicComponent.hpp"
#include "Audio.hpp"
#include "Engine/Engine.h"

/**
 * \brief 
 * \return 
 */
MusicController* MusicController::Clone_() const
{
	return new MusicController(*this);
}

/**
 * \brief 
 * \param name 
 * \param delay 
 * \param probability 
 */
void MusicController::AddMusic(const char* name, unsigned int delay, float probability) 
{
	songs_.push_back(std::make_tuple(name, delay ,probability)); //make new pair and push it to vector
}

/**
 * \brief 
 */
void MusicController::PlayMusic()
{
	engine.Find<Audio::AudioManager>()->PlayRandomSound(songs_);
}