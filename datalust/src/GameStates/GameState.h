/******************************************************************************/
/*!
  \file    GameState.h
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/07/2017

  \brief  
    Deffinition of the GameState class
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <Util/Util.hpp>
#include <string>
//#include <GameObject/GameObjectManager.h>
#include <GameObject/ObjectFactory.h>
#include <Messaging/Messaging.hpp>



/******************************************************************************/
/*!
    the game_state is the base class to be used as an interface for all other 
    gamestates
*/
/******************************************************************************/
class GameState
{
  public:
	  GameState(std::string filename) : manager_(), filename_(filename) {}

    /**************************************************************************/
    /*!
        virtual destructor for the game_state class, must be implemented by each
        derived class
    */
    /**************************************************************************/
    virtual ~GameState() {}
    /**************************************************************************/
    /*!
        init function for the game_state class, must be implemented by each
        derived class
    */
    /**************************************************************************/
    virtual void Init();
    /**************************************************************************/
    /*!
        update funcntion for the game_state class, must be implemented by each
        derived class

        \param float
            takes the time elapsed since the last update
    */
    /**************************************************************************/
	virtual void Update(float dt);
    /**************************************************************************/
    /*!
        shutdown function for the game_state class, must be implemented by each
        derived class
    */
    /**************************************************************************/
    virtual void Shutdown();

	GameObjectManager& Manager() { return manager_; }
	
	bool Reset() { return reset_; }
	void Reset(bool reset) { reset_ = reset; }
	std::pair<bool, int> NewState() { return NewState_; }
	void NewState(std::pair<bool, int> NewState) { NewState_ = NewState; }
  std::string getFilename_() { return filename_; }
private:
	GameObjectManager manager_;
	std::string filename_;
	bool reset_ = false;
	std::pair<bool, int> NewState_ = { false, 0 };
};

#endif //GAMESTATE_H
