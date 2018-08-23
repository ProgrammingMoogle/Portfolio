/**
  \file   MessagingTest.hpp
  \author Taylor Osmond

  \brief Interface for input test.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _MESSAGING_TEST_H
#define _MESSAGING_TEST_H
#include <Util/Util.hpp>
#include <GameStates\GameState.h>

//create new messaging test state
/**
 * \brief 
 */
class MessagingTest : public GameState
{
  public:
	MessagingTest(std::string levels);         ///< constructor
	~MessagingTest() {}      ///< destructor
    void Init();             ///< init function
	void Update(float dt) {} ///< update function
	void Shutdown() {}       ///< shutdown function

private:
	/*************************************************************************/
};

#endif //DEMOSTATE_H
