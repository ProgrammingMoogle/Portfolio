/**
  \file   MusicComponent.hpp
  \author Taylor Osmond

  \brief Interface for audio manager system.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _MUSIC_COMPONENT_HPP_
#define _MUSIC_COMPONENT_HPP_

#include <Util/Util.hpp>
#include <GameObject/Component.h>
#include <string>
#include <vector>
#include <tuple>

/**
 * \brief 
 */
class GameObject;

/**
 * \brief 
 */
class MusicController : public Component {
    friend class GameObject;
public:
	MusicController(GameObject& parent) : Component(parent) {}			   ///< Constructor
    virtual ~MusicController() {}										   ///< Destructor
    virtual MusicController* Clone_() const;							   ///< Clone function
	void AddMusic(const char* name, unsigned int delay, float probability); ///< Add music to vector
	void PlayMusic();											           ///< Play random song
private:
	std::vector<std::tuple<std::string, unsigned int, float>> songs_; ///< Vector of sound names, delays (ms), and probabilities (0.0 - 1.0)
};

#endif
