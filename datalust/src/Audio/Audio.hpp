/**
  \file   Audio.hpp
  \author Taylor Osmond

  \brief Interface for audio manager system.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _AUDIO_HPP_
#define _AUDIO_HPP_

#include <fmod.hpp>
#include <fmod_errors.h>
#include <Engine/System.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <Trace/Trace.hpp>

#define MAX_CHANNELS 32
#define PAUSE true
#define PLAY false
#define MUSIC 1
#define EFFECT 2

/**
 * \brief 
 */
namespace Audio {
	/// Error function
	void ERRCHECK(FMOD_RESULT result);
	
	/// Sound
	class Sound {
	public:
		Sound(const char* name, int type, bool looping); ///< Constructor
		~Sound();					   ///< Destructor
		void Play(unsigned int delay); ///< Play the sound (with delay)
		void Pause();				   ///< Pause the sound
		void Unpause();				   ///< Unpause the sound
		void SetPitch(float pitch);	   ///< Set pitch of the sound
		void SetVolume(float volume);  ///< Set volume of the sound
		void SetFrequency();		   ///< Set frequency of the sound
		int GetType();				   ///< Gets the type of the sound
		const char* GetName();		   ///< Gets the name of a sound
	private:
	  std::string name_;       ///< Name of the sound
	  std::string directory_;  ///< Directory of sound
	  int type_;               ///< Type of sound (effect/music)
	  bool looping_;           ///< Loop state of the sound
	  unsigned int length_;    ///< Length of the sound (miliseconds)
	  float volume_;
	  float pitch_;            ///< Pitch
	  float frequency_;        ///< Frequency (Hz)
	  FMOD::Sound *sound_;     ///< FMOD sound
	  FMOD::Channel *channel_; ///< FMOD channel
	};

	/// Audio Manager
  /**
  * \brief
  */
	class AudioManager : public System {
	public:
	  bool Init();												 ///< Init function
	  void Update(float dt);									 ///< Update function
	  void Shutdown();											 ///< Shutdown function
	  void Load();												 ///< Loads sounds into memory
	  void Unload();											 ///< Unloads sounds from memory
	  void PlaySound(const char* name, unsigned int delay = 0U); ///< Play a sound by name with delay in ms (defaulted to 0 if not specified)
	  void PauseSound(const char* name);						 ///< Pause a sound by name
	  void UnpauseSound(const char* name);						 ///< Unpause a sound by name
	  void StopAllSound();										 ///< Stops all currently playing sounds
	  void StopEffects();										 ///< Stops all currently playing effects
	  void StopMusic();											 ///< Stops all currently playing music
	  void SetEffectsVolume(float volume);						 ///< Set the volume for effects channel (0 to 1)
	  void SetMusicVolume(float volume);						 ///< Set the volume for music channel (0 to 1)
	  void SetMasterVolume(float volume);						 ///< Volume multiplyer for both effects and music (0 to 1)
	  float GetEffectsVolume();									 ///< Get the current volume of the effects channel
	  float GetMusicVolume();									 ///< Get the current volume of the music channel
	  float GetMasterVolume();									 ///< Get the current master volume multiplyer
	  void SetEffectsPitch(float pitch);						 ///< Set the pitch of the effects channel
	  void SetMusicPitch(float pitch);							 ///< Set the pitch of the music channel
	  void SetMasterPitch(float pitch);							 ///< Set the pitch of the master channel
	  float GetEffectsPitch();									 ///< Set the pitch of the effects channel
	  float GetMusicPitch();									 ///< Set the pitch of the music channel
	  float GetMasterPitch();									 ///< Set the pitch of the master channel
	  void SetEffectsFrequency(float frequency);				 ///< Set the frequency of the effects channel
	  void SetMusicFrequency(float frequency);					 ///< Set the frequency of the music channel
	  void SetMasterFrequency(float frequency);					 ///< Set the frequency of the master channel
	  float GetEffectsFrequency();								 ///< Set the frequency of the effects channel
	  float GetMusicFrequency();								 ///< Set the frequency of the music channel
	  float GetMasterFrequency();								 ///< Set the frequency of the master channel
	  int GetOutputRate();										 ///< Get the system sample output rate
	  
	  void SetSoundPitch(float pitch, const char* name);																     ///< Set the frequency of a specific sound
	  void SetSoundVolume(float volume, const char* name);																	 ///< Set the volume of a specific sound
	  int PlayRandomSound(std::vector<std::tuple<std::string, unsigned int, float>> sounds);					             ///< Play a random sound from a list of sounds, delays (ms), and probabilities (0.0 - 1.0), returns index of played sound
	  void PlaySequence(std::vector<std::pair<std::string, unsigned int>> sequence, bool relative);						     ///< Play a sequence of sounds from a list of sounds and delays (ms), bool: delay relative to last sound = true | absolute time = false
	  void PlayRandomSequence(std::list<std::vector<std::tuple<std::string, unsigned int, float>>> sequence, bool relative); ///< Play a random sequence from a list of sounds, delays (ms), and probabilities (0.0 - 1.0), bool: delay relative to last sound = true | absolute time = false
	private:
	  unsigned int version_;		   ///< FMOD version
	  FMOD::System *system_;		   ///< FMOD system
	  FMOD::ChannelGroup *effects_;	   ///< Effect channelgroup
	  FMOD::ChannelGroup *music_;	   ///< Music channelgroup
	  int output_rate_;		           ///< System output rate in samples
	  float effects_volume_ = 1.0f;	   ///< Effects volume
	  float music_volume_ = 1.0f;	   ///< Music volume
	  float master_volume_ = 1.0f;	   ///< Master volume
	  float effects_pitch_ = 1.0f;	   ///< Effects pitch
	  float music_pitch_ = 1.0f;	   ///< Music pitch
	  float master_pitch_ = 1.0f;	   ///< Master pitch
	  float effects_frequency_ = 1.0f; ///< Effects frequency
	  float music_frequency_ = 1.0f;   ///< Music frequency
	  float master_frequency_ = 1.0f;  ///< Master frequency

      /// Map of all sounds
	  std::unordered_map<std::string, Sound*> sounds_; 
	  friend class Sound;
	};
}

#endif