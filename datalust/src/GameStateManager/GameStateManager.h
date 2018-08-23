/******************************************************************************/
/*!
  \file    GameStateManager.h
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/07/2017

  \brief
    GameStateManager.h contains the declerations for the game state manager
    
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <queue>
#include <stack>
#include <memory>
#include <Engine/System.h>
#include <GameStates/GameState.h>
#include <GameObject/ObjectFactory.h>

/******************************************************************************/
/*!
    the GameStateManager class maintains the GameStates in a stack and makes 
    sure that they are properly initialized, updated, and quit. 
*/
/******************************************************************************/
class GameStateManager : public System
{
  bool paused_ = false;
  bool playerPaused = false;
public:




  /****************************************************************************/
  /*!
      constructor for the GameStateManager, puts a nullptr as the first member
      of the stack for safety/sanity checking. It also sets the isUpdating flag
      to be false
  */
  /****************************************************************************/
  GameStateManager();

  /****************************************************************************/
  /*!
      destructor for the GameStateManager, calls the Quit function, and then 
      dealocates the data used for the stack
  */
  /****************************************************************************/
  ~GameStateManager();

  /****************************************************************************/
  /*!
      init function pushes the initial GameState onto the stack and initializes
      it, this is seperate from the constructor so that the manager can be 
      created without data

      \param GameState
        the initial game state
  */
  /****************************************************************************/
  bool Init();

  /****************************************************************************/
  /*!
      update funcntion for the GameStateManager class, only updates the 
      GameState on the top of the stack 
      (Stack manipulation is gaurenteed not to occur inside the update function)

      \param float
          takes the time elapsed since the last update
  */
  /****************************************************************************/
  void Update(float dt);
  
  /****************************************************************************/
  /*!
      loops through the stack while the and calls the pop_stack member function 
      untill it hits the nullptr at begining of the stack
  */
  /****************************************************************************/
  void Quit();
  
  /****************************************************************************/
  /*!
      restarts the GameState by calling the Quit on the state, and then
      re-initializing it
  */
  /****************************************************************************/
  void ResetState();
  
  /****************************************************************************/
  /*!
      changes the state by poping the current state and pushing a new state

      \param GameState
        the state that it is being changed to
  */
  /****************************************************************************/
  void ChangeState(const int level);

  /****************************************************************************/
  /*!
	changes the state to the next state
  */
  /****************************************************************************/
  void ChangeState();
  
  /****************************************************************************/
  /*!
      pushes the game state onto the stack and then inits the state

      \param GameState
        the state that it is being changed to
  */
  /****************************************************************************/
  void PushState(const int new_state);
  
  /****************************************************************************/
  /*!
      shutsdown the state and pops it from the stack
  */
  /****************************************************************************/
  void PopState();

  void DelayedPopState();
  void DelayedPushState(int new_state);
  void DelayedResetState();

  /****************************************************************************/
  /*!
      returns the GameState at the top of the stack

      \return GameState
        the GameState on top the stack
  */
  /****************************************************************************/
  GameState* CurrentState();

  /****************************************************************************/
  /*!
      determines whether the gameStateManager is running

      \return bool
        returns true if the gameStateManager is runnin and false otherwise
  */
  /****************************************************************************/
  bool IsRunning();

  GameObject* pauseObj;

  void PauseState(const int Level);
  void UnpauseState(const int level);

  //void Unpause(const int Level);

  //void Unpause();
private:
  enum StateOperation
  {
    EnumPopState,
    EnumPushState,
    EnumResetState
  };
  struct Operation
  {
    Operation(StateOperation _op, int _newState = 0) : op(_op), newState(_newState) {}
    StateOperation op;
    int newState;
  };
  std::queue<Operation> MachineUpdateQueue;
  //! private variable used to determine if the manager is updating, that what 
  //! it won't be able to manipulate the stack if it is
  bool isUpdating_ = false;
  
  //! the stack used to store all of the game states
  std::stack<std::shared_ptr<GameState> > gameStateManager_;

  //! class used to store and order the levels in the game read in from a manifest file
  LevelSerializer levels_;
  
  //! private function that implements Quit(), used as saftey feature so it 
  //! won't manipulate the stack while the manager is updating
  void quit_();
  
  //! private function that implements reset_state(), used as saftey feature so 
  //! it won't manipulate the stack while the manager is updating
  void resetState_();
  
  //! private function that implements change_state(), used as saftey feature so 
  //! it won't manipulate the stack while the manager is updating
  void changeState_(const int new_state);
  
  void changeState_();

  //! private function that implements push_state(), used as saftey feature so 
  //! it won't manipulate the stack while the manager is updating
  void pushState_(const int new_state);
  void pushState_();


  //! private function that implements pop_state(), used as saftey feature so 
  //! it won't manipulate the stack while the manager is updating
  void popState_();
  
  /****************************************************************************/
  /*!
    class used to catch throws from the quit() function
  */
  /****************************************************************************/
  class stateQuit_
  {};

  /****************************************************************************/
  /*!
    class used to catch throws from the reset_state() function
  */
  /****************************************************************************/
  class stateReset_
  {};

  /****************************************************************************/
  /*!
    class used to catch throws from the changeState_() function
  */
  /****************************************************************************/
  class stateChange_
  {
    const int newState_;
  public:
    stateChange_(const int newState);
    const int get_state() const;      
  };

  /****************************************************************************/
  /*!
    class used to catch throws from the push_state() function
  */
  /****************************************************************************/
  class statePush_
  {
    const int newState_;
  public:
    statePush_(const int newState);    
    const int get_state() const;
  };
  /****************************************************************************/
  /*!
    class used to catch throws from the pop_state() function
  */
  /****************************************************************************/
  class statePop_
  {};
};




#endif //GAMESTATEMANAGER_H