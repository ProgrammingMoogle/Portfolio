/**
  \file   Audio.cpp
  \author Taylor Osmond

  \brief Implementation of audio manager system

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Audio.hpp"
#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Engine/Engine.h"

/**
 * \brief 
 */
namespace Audio {
	void ERRCHECK(FMOD_RESULT result)
	{
	  //check fmod result and throw if not ok
		if (result != FMOD_OK && result != FMOD_ERR_CHANNEL_STOLEN)
			//throw std::runtime_error(FMOD_ErrorString(result));
			engine.Find<Trace::TraceHandler>()->TraceMessage(ERROR, FMOD_ErrorString(result));
	}

  /**
	 * \brief 
	 * \return 
	 */
	bool AudioManager::Init()
	{
		//seed the random number generator
		srand(static_cast<unsigned int>(time(NULL)));

		//create new system
		ERRCHECK(FMOD::System_Create(&system_));

		//get the fmod version
		ERRCHECK(system_->getVersion(&version_));

		//verify fmod version
		if (version_ < FMOD_VERSION)
			throw std::runtime_error("FMOD version doesn't match header version");

		//initialize the system
		ERRCHECK(system_->init(MAX_CHANNELS, FMOD_INIT_NORMAL, 0));

		//initialize the channelgroups
		ERRCHECK(system_->createChannelGroup("Effects", &effects_));
		ERRCHECK(system_->createChannelGroup("Music", &music_));

		//get the sample output rate
		ERRCHECK(system_->getSoftwareFormat(&output_rate_,0,0));

		//load sounds
		Load();

		return true;
	}

  /**
	 * \brief 
	 * \param dt 
	 */
	void AudioManager::Update(float dt)
	{
		//call fmod update on system
		ERRCHECK(system_->update());
	}

  /**
	 * \brief 
	 */
	void AudioManager::Shutdown()
	{
		Unload();                     //unload all sounds
		ERRCHECK(system_->close());   //close fmod system
		ERRCHECK(system_->release()); //release fmod system
	}

  /**
	 * \brief 
	 */
	void AudioManager::Load()
	{
		//open effect manifest
		std::ifstream effect_file("data/sounds/effects/manifest.txt");

		//if the file is open...
		if (effect_file.is_open())
		{
			//loop through the file
			while(!effect_file.eof())
			{
				std::string filename; //create string to hold filename
				std::string loop;	  //string to hold loop status
				std::string line;	  //string to get the entire line

				std::getline(effect_file, line); //get a line

				std::stringstream line_stream(line); //open a line stream

				//if the line is a comment, ignore this line
				if (line_stream.str()[0] == '#')
					continue;

				//read in the filename and loop
				line_stream >> filename >> loop;

				//if filename is empty, move to next line
				if (filename == "")
					continue;
				
				//create a temp bool to hold the loop status
				bool temp_loop;

				//set the temp depending on token read in
				if (loop == "loop")
					temp_loop = true;
				else if (loop == "noloop")
					temp_loop = false;
				//if token was not recognized, give warning message and assume false
				else
				{
					engine.Find<Trace::TraceHandler>()->TraceMessage(INFO, "Loop status not specified for file %s, noloop assumed\n", filename);
					temp_loop = false;
				}

				//create new sound
				Sound *effect = new Sound(filename.c_str(), EFFECT, temp_loop);

				//insert into map
				sounds_.insert(std::make_pair(filename, effect));
			}

			//close the file
			effect_file.close();
		}

		//open music manifest
		std::ifstream music_file("data/sounds/music/manifest.txt");

		//if the file is open...
		if (music_file.is_open())
		{
			//loop through the file
			while (!music_file.eof())
			{
				std::string filename; //create string to hold filename
				std::string loop;	  //string to hold loop status
				std::string line;	  //string to get the entire line

				std::getline(music_file, line); //get a line

				std::stringstream line_stream(line); //open a line stream

													 //if the line is a comment, ignore this line
				if (line_stream.str()[0] == '#')
					continue;

				//read in the filename and loop
				line_stream >> filename >> loop;

				//if filename is empty, move to next line
				if (filename == "")
					continue;

				//create a temp bool to hold the loop status
				bool temp_loop;

				//set the temp depending on token read in
				if (loop == "loop")
					temp_loop = true;
				else if (loop == "noloop")
					temp_loop = false;
				//if token was not recognized, give warning message and assume false
				else
				{
					std::cerr << "WARNING: Loop status not specified for file " << filename << ", noloop assumed" << std::endl;
					temp_loop = false;
				}

				//create new sound
				Sound *music = new Sound(filename.c_str(), MUSIC, temp_loop);

				//insert into map
				sounds_.insert(std::make_pair(filename, music));
			}

			//close the file
			music_file.close();
		}
	}

  /**
	 * \brief 
	 */
	void AudioManager::Unload()
	{
		//remove all sounds
		for (auto i : sounds_) {
			delete i.second;
		}
		sounds_.clear();
	}

  /**
	 * \brief 
	 * \param name 
	 * \param delay 
	 */
	void AudioManager::PlaySound(const char* name, unsigned int delay)
	{
		//find sound with given name and play it
		auto sound = sounds_.find(name);

		if (sound != sounds_.end())
			sound->second->Play(delay);
		else {
			engine.Find<Trace::TraceHandler>()->TraceMessage(ERROR, "file %s%s not found", name, ".mp3");
			sounds_.find("default")->second->Play(delay);
		}
	}

  /**
	 * \brief 
	 * \param name 
	 */
	void AudioManager::PauseSound(const char* name)
	{
		//find sound with given name and pause it
		sounds_.find(name)->second->Pause();
	}

  /**
	 * \brief 
	 * \param name 
	 */
	void AudioManager::UnpauseSound(const char* name) 
	{
		//find sound with given name and unpause it
		sounds_.find(name)->second->Unpause();
	}

  /**
	 * \brief 
	 * \param name 
	 * \param type 
	 * \param looping 
	 */
	Sound::Sound(const char* name, int type, bool looping) :
		name_(name),
		type_(type),													     
		looping_(looping),													     
		length_(0),
		pitch_(1.0f),
		frequency_(0.0f),
		sound_(NULL),													     
		channel_(NULL)
	{
   	//get the audio manager
		AudioManager *manager = engine.Find<AudioManager>();

		//if sound is an effect, create a sound
		if (type_ == EFFECT)
		{
			//set directory
			directory_ = "data/sounds/effects/";
			directory_ += name;
			directory_ += ".mp3";

			//if looping is true...
			if (looping_) {
				//create the sound with fmod loop
				FMOD_RESULT result = manager->system_->createSound(directory_.c_str(), FMOD_LOOP_NORMAL, 0, &sound_);

				//if file not found, send error message
				if (result == FMOD_ERR_FILE_NOTFOUND) {
					sound_->release();
					engine.Find<Trace::TraceHandler>()->TraceMessage(ERROR, "file %s%s not found", name, ".mp3");
				}
			}
			//otherwise...
			else {
				//create the sound without loop
				FMOD_RESULT result = manager->system_->createSound(directory_.c_str(), FMOD_DEFAULT, 0, &sound_);

				//if file not found, send error message
				if (result == FMOD_ERR_FILE_NOTFOUND) {
					sound_->release();
					engine.Find<Trace::TraceHandler>()->TraceMessage(ERROR, "file %s%s not found", name, ".mp3");
				}
			}
		}
		//otherwise if sound is music, create a stream
		else if (type_ == MUSIC) 
		{
			//set directory
			directory_ = "data/sounds/music/";
			directory_ += name;
			directory_ += ".mp3";

			//if looping is true...
			if (looping_) {
				//create the sound with fmod loop
				FMOD_RESULT result = manager->system_->createStream(directory_.c_str(), FMOD_LOOP_NORMAL, 0, &sound_);

				//if file not found, send error message
				if (result == FMOD_ERR_FILE_NOTFOUND) {
					sound_->release();
					engine.Find<Trace::TraceHandler>()->TraceMessage(ERROR, "file %s%s not found", name, ".mp3");
				}
			}
			//otherwise...
			else {
				//create the sound without loop
				FMOD_RESULT result = manager->system_->createStream(directory_.c_str(), FMOD_DEFAULT, 0, &sound_);

				//if file not found, send error message
				if (result == FMOD_ERR_FILE_NOTFOUND) {
					sound_->release();
					engine.Find<Trace::TraceHandler>()->TraceMessage(ERROR, "file %s%s not found", name, ".mp3");
				}
			}
		}

		//get the length of the sound in miliseconds
		ERRCHECK(sound_->getLength(&length_, FMOD_TIMEUNIT_MS));
	}

	Sound::~Sound()	{ ERRCHECK(sound_->release()); }

  /**
	 * \brief 
	 * \param delay 
	 */
	void Sound::Play(unsigned int delay)
	{
		//get the audio manager
		AudioManager *manager = engine.Find<AudioManager>();

		//play effect
		if (type_ == EFFECT)
		{
			//start sound paused
			ERRCHECK(manager->system_->playSound(sound_, manager->effects_, PAUSE, &channel_));
		}
		//play music
		else if (type_ == MUSIC)
		{
			//start sound paused
			ERRCHECK(manager->system_->playSound(sound_, manager->music_, PAUSE, &channel_));
		}

		if(delay)
		{
			//get current DSP clock time
			unsigned long long parent_clock;
			channel_->getDSPClock(0, &parent_clock);

			//get the given delay in samples
			unsigned int sample_delay = static_cast<unsigned int>(engine.Find<AudioManager>()->GetOutputRate());
			sample_delay *= delay;
			sample_delay /= 1000ULL;

			//get the total delay (delay + start time)
			unsigned long long total_delay = 0ULL;
			total_delay = (static_cast<unsigned long long>(sample_delay) + parent_clock);

			//set the delay on the channel
			channel_->setDelay(total_delay, 0, 0);
		}

		//Get initial frequency
		ERRCHECK(channel_->getFrequency(&frequency_));

		//Set volume, pitch, and frequency
		manager->SetMasterVolume(manager->master_volume_);
		manager->SetMasterPitch(manager->master_pitch_);

		//unpause sound
		ERRCHECK(channel_->setPaused(false));
	}

  /**
	 * \brief 
	 */
	void Sound::Pause()
	{
		//set channel paused
		ERRCHECK(channel_->setPaused(true));
	}

  /**
	 * \brief 
	 */
	void Sound::Unpause()
	{
		//set channel unpaused
		ERRCHECK(channel_->setPaused(false));
	}

  /**
	 * \brief 
	 * \param pitch 
	 */
	void Sound::SetPitch(float pitch)
	{
		//get the audio manager
		AudioManager *manager = engine.Find<AudioManager>();

		//validate the channel
		if (!channel_)
			return;

		//update the pitch
		pitch_ = pitch;

		//set the pitch
		ERRCHECK(channel_->setPitch(pitch_));
	}

  /**
	 * \brief 
	 * \param volume 
	 */
	void Sound::SetVolume(float volume)
	{
		//get the audio manager
		AudioManager *manager = engine.Find<AudioManager>();

		//validate the channel
		if (!channel_)
			return;

		//update the volume
		volume_ = volume;

		//if sound is an effect...
		if (type_ == EFFECT)
			//update effect volume
			ERRCHECK(channel_->setVolume(volume_));
		//otherwise
		else
			//update music volume
			ERRCHECK(channel_->setVolume(volume_));
	}

  /**
	 * \brief 
	 */
	void Sound::SetFrequency()
	{
		//get the audio manager
		AudioManager *manager = engine.Find<AudioManager>();

		//validate channel
		if (!channel_)
			return;

		//if sound is an effect...
		if (type_ == EFFECT)
			//update effect frequency
			ERRCHECK(channel_->setFrequency(frequency_ * manager->effects_frequency_ * manager->master_frequency_));
		//otherwise
		else
			//update music frequency
			ERRCHECK(channel_->setFrequency(frequency_ * manager->music_frequency_ * manager->master_frequency_));
	}

  /**
	 * \brief 
	 * \return 
	 */
	int Sound::GetType()
	{
		//get the type
		return type_;
	}

  /**
   * \brief 
   * \return 
   */
  const char* Sound::GetName()
  {
    //get the name
    return name_.c_str();
  }

  /**
	 * \brief 
	 */
	void AudioManager::StopAllSound()
	{
		ERRCHECK(effects_->stop()); //stop effects group
		ERRCHECK(music_->stop()); //stop music group
	}

  /**
	 * \brief 
	 */
	void AudioManager::StopEffects()
	{
		ERRCHECK(effects_->stop()); //stop effects sound group
	}

  /**
	 * \brief 
	 */
	void AudioManager::StopMusic()
	{
		ERRCHECK(music_->stop()); //stop music sound group
	}

  /**
	 * \brief 
	 * \param volume 
	 */
	void AudioManager::SetEffectsVolume(float volume)
	{
		//update effects volume
		if(volume >= 0)
			effects_volume_ = volume;

		//set volume to given volume, scaled by master volume
		ERRCHECK(effects_->setVolume(effects_volume_ * master_volume_));
	}

  /**
	 * \brief 
	 * \param volume 
	 */
	void AudioManager::SetMusicVolume(float volume)
	{
		//update music volume
		if(volume >= 0)
			music_volume_ = volume;

		//set volume to given volume, scaled by master volume
		ERRCHECK(music_->setVolume(music_volume_ * master_volume_));
	}

  /**
	 * \brief 
	 * \param volume 
	 */
	void AudioManager::SetMasterVolume(float volume)
	{
		//update master volume
		if(volume >= 0)
			master_volume_ = volume;

		//update effects and music volumes with new scalar
		ERRCHECK(effects_->setVolume(effects_volume_ * master_volume_));
		ERRCHECK(music_->setVolume(music_volume_ * master_volume_));
	}

  /**
	 * \brief 
	 * \return 
	 */
	float AudioManager::GetEffectsVolume()
	{
	    //get effects volume
		return effects_volume_;
	}

  /**
	 * \brief 
	 * \return 
	 */
	float AudioManager::GetMusicVolume()
	{
		//get music volume
		return music_volume_;
	}

  /**
	 * \brief 
	 * \return 
	 */
	float AudioManager::GetMasterVolume()
	{
		//get master volume
		return master_volume_;
	}

  /**
	 * \brief 
	 * \param pitch 
	 */
	void AudioManager::SetEffectsPitch(float pitch) 
	{
		//update master pitch
		if (pitch > 0)
			effects_pitch_ = pitch;

		ERRCHECK(effects_->setPitch(effects_pitch_ * master_pitch_));
	}

  /**
	 * \brief 
	 * \param pitch 
	 */
	void AudioManager::SetMusicPitch(float pitch)
	{
		//update master pitch
		if (pitch > 0)
			music_pitch_ = pitch;

		ERRCHECK(music_->setPitch(music_pitch_ * master_pitch_));
	}

  /**
	 * \brief 
	 * \param pitch 
	 */
	void AudioManager::SetMasterPitch(float pitch)
	{   
		//update master pitch
		if (pitch > 0)
			master_pitch_ = pitch;

		ERRCHECK(effects_->setPitch(effects_pitch_ * master_pitch_));
		ERRCHECK(music_->setPitch(music_pitch_ * master_pitch_));
	}

  /**
	 * \brief 
	 * \return 
	 */
	float AudioManager::GetEffectsPitch()
	{
		//get effects pitch
		return effects_pitch_;
	}

  /**
	 * \brief 
	 * \return 
	 */
	float AudioManager::GetMusicPitch()
	{
		//get music pitch
		return music_pitch_;
	}

  /**
	 * \brief 
	 * \return 
	 */
	float AudioManager::GetMasterPitch() 
	{
		//get master pitch
		return master_pitch_;
	}

  /**
	 * \brief 
	 * \param frequency 
	 */
	void AudioManager::SetEffectsFrequency(float frequency)
	{
		//update effects frequency
		effects_frequency_ = frequency;

		//update frequency all effects
		for (auto it = sounds_.begin(); it != sounds_.end(); ++it)
		{
			if(it->second->GetType() == EFFECT)
				it->second->SetFrequency();
		}
	}

  /**
	 * \brief 
	 * \param frequency 
	 */
	void AudioManager::SetMusicFrequency(float frequency) 
	{
		//update music frequency
		music_frequency_ = frequency;

		//update frequency all music
		for (auto it = sounds_.begin(); it != sounds_.end(); ++it)
		{
			if (it->second->GetType() == MUSIC)
				it->second->SetFrequency();
		}
	}

  /**
	 * \brief 
	 * \param frequency 
	 */
	void AudioManager::SetMasterFrequency(float frequency) 
	{
		//update master frequency
		master_frequency_ = frequency;

		//update frequency for all sounds
		for (auto it = sounds_.begin(); it != sounds_.end(); ++it)
		{
			it->second->SetFrequency();
		}
	}

  /**
	 * \brief 
	 * \return 
	 */
	float AudioManager::GetEffectsFrequency()
	{
		//get effects frequency
		return effects_frequency_;
	}

  /**
	 * \brief 
	 * \return 
	 */
	float AudioManager::GetMusicFrequency()
	{
		//get music frequency
		return music_frequency_;
	}

  /**
	 * \brief 
	 * \return 
	 */
	float AudioManager::GetMasterFrequency()
	{
		//get master frequency
		return master_frequency_;
	}

  /**
	 * \brief 
	 * \return 
	 */
	int AudioManager::GetOutputRate()
	{
		//get output rate
		return output_rate_;
	}

  /**
	 * \brief 
	 * \param pitch 
	 * \param name 
	 */
	void AudioManager::SetSoundPitch(float pitch, const char* name)
	{
		//loop through sounds
		for (auto it = sounds_.begin(); it != sounds_.end(); ++it)
		{
			//if the sound is the desired sound, set its pitch
			if (std::string(it->second->GetName()) == name)
				it->second->SetPitch(pitch);
		}
	}

  /**
	 * \brief 
	 * \param volume 
	 * \param name 
	 */
	void AudioManager::SetSoundVolume(float volume, const char* name)
	{
		//loop through sounds
		for (auto it = sounds_.begin(); it != sounds_.end(); ++it)
		{
			//if the sound is the desired sound, set its volume
			if (std::string(it->second->GetName()) == name)
				it->second->SetVolume(volume);
		}
	}

  /**
	 * \brief 
	 * \param sounds 
	 * \return 
	 */
	int AudioManager::PlayRandomSound(std::vector<std::tuple<std::string, unsigned int, float>> sounds)
	{
		//get a random number between 0.0 and 1.0
		float r = static_cast<float>(rand());
			r /= static_cast<float>(RAND_MAX);

		float cur_sum = 0.0f; //counter
		
		//normalize probabilities
		float sum = 0.0f; //counter 2
		for (auto &it : sounds)
			sum += std::get<2>(it);
		for (auto &it : sounds)
			std::get<2>(it) /= sum;

		//loop through given vector
		for (unsigned int i = 0; i < sounds.size(); ++i)
		{
			//if the random number is less than the current sum + the next...
			if (r < (cur_sum + std::get<2>(sounds[i])))
			{
				PlaySound(std::get<0>(sounds[i]).c_str(), std::get<1>(sounds[i])); //play the sound
				return i; //return the index
			}
			else
				cur_sum += std::get<2>(sounds[i]); //otherwise add the next to the sum
		}

		return -1; //ERROR
	}

  /**
	 * \brief 
	 * \param sequence 
	 * \param relative 
	 */
	void AudioManager::PlaySequence(std::vector<std::pair<std::string, unsigned int>> sequence, bool relative)
	{
		//if delay is relative to the last sound
		if (relative)
		{
			unsigned int total_delay = 0U; //create delay counter

			//loop through sounds vector
			for(auto &sound : sequence)
			{
				total_delay += sound.second; //add current delay to total
				PlaySound(sound.first.c_str(), total_delay); //play the sound with the total delay
			}
		}
		//if delay time is absolute
		else
			//loop through sounds vector
			for (auto &sound : sequence)
				PlaySound(sound.first.c_str(), sound.second); //play the sound with its delay
	}

  /**
	 * \brief 
	 * \param sequence 
	 * \param relative 
	 */
	void AudioManager::PlayRandomSequence(std::list<std::vector<std::tuple<std::string, unsigned int, float>>> sequence, bool relative)
	{
		//if delay is relative to the last sound
		if (relative) 
		{
			int index; //index of randomly chosen sound
			unsigned int total_delay = 0U; //total delay in miliseconds

			//loop through list
			for(auto &sounds_vec : sequence)
			{
				//loop through vector
				for(auto &sound : sounds_vec)
					std::get<1>(sound) += total_delay; //add the total delay to all sounds in the vector

				index = PlayRandomSound(sounds_vec); //play a random sound from a vector
				total_delay = std::get<1>(sounds_vec[index]); //get the new total delay from the previously played sound
			}
		}
		//if delay time is absolute
		else
			for(auto &sounds_vec : sequence) //loop through list
				PlayRandomSound(sounds_vec); //play a random sound from each node
	}
}