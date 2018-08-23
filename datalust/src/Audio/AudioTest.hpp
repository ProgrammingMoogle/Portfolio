/**
  \file   AudioTest.hpp
  \author Taylor Osmond

  \brief Interface for audio test.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _AUDIO_TEST_H
#define _AUDIO_TEST_H
#include <Util/Util.hpp>
#include <string>
#include <GameStates/GameState.h>

//create new input test state
/**
 * \brief 
 */
class AudioTest : public GameState
{
  public:
	AudioTest(std::string levels);           ///< constructor
	~AudioTest() {}        ///< destructor
    void Init();           ///< init function
	void Update(float dt); ///< update function
	void Shutdown() {}     ///< shutdown function

private:
	/*************************************************************************/
};

#endif //DEMOSTATE_H
