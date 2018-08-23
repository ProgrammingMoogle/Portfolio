/**
  \file   InputTest.hpp
  \author Taylor Osmond

  \brief Interface for input test.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _INPUT_TEST_H
#define _INPUT_TEST_H
#include <Util/Util.hpp>
#include <GameStates/GameState.h>

//create new input test state
class InputTest : public GameState
{
  public:
	InputTest(std::string levels);             ///< constructor
	~InputTest() {}          ///< destructor
    void Init();             ///< init function
	void Update(float dt) {} ///< update function
	void Shutdown() {}       ///< shutdown function

private:
	/*************************************************************************/
};

#endif //DEMOSTATE_H
