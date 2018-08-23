#pragma once
/**
  \file   Player1Behavior.hpp
  \author Zayd Gaudet

  \brief Behavior component for player 1

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <GameObject\Component.h>
#include <Messaging\Messaging.hpp>
#include "HealthComponent.h"

/**
 * \brief 
 */
class Player1Behavior : public Component {
public:
  Player1Behavior( GameObject &parent, bool canshoot = false);
  ~Player1Behavior();
  void RemoveInput(ObjectId ID);
  void ReturnInput(ObjectId ID);
  virtual Component* Clone_() const { return new Player1Behavior(*this); }
  virtual void Update(float dt);
  void AddToDMG(int NewDMG) { dmg += NewDMG; }
  int DMG() { return dmg; }
  void AddCoin() { coins++; };
  void RemoveCoin() { coins--; };
  void EnableBlueGun() { CanShootBlue = true; }
  void EnableRedGun() { CanShootRed = true; }
  const int GetCoins() { return coins; };
  const int GetHealth() { return GetParent().Find<HealthComponent>()->GetHealth();}
  void AddHealthToPlayer(int amt) { GetParent().Find<HealthComponent>()->AddHealth(amt);}
  static void InputLeft( ObjectId, const Messaging::Message& );
  static void InputRight( ObjectId, const Messaging::Message& );
  static void InputUp(ObjectId, const Messaging::Message&);
  static void InputDown(ObjectId, const Messaging::Message&);
  static void InputShoot(ObjectId, const Messaging::Message&);
  static void AnimationUpdate( ObjectId, const Messaging::Message& );
  static void CollisionHandle(ObjectId, const Messaging::Message&);
  static void InputF1(ObjectId obj, const Messaging::Message& message);
  static void Pause(ObjectId obj, const Messaging::Message& message);

  static void FocusLost(ObjectId obj, const Messaging::Message& message);
  void onFocusLost(const Messaging::Message& message);

  static void PMenu(ObjectId obj, const Messaging::Message& message);
  static void PQuit(ObjectId obj, const Messaging::Message& message);
  static void PReset(ObjectId obj, const Messaging::Message& message);
  static void PConfirm(ObjectId obj, const Messaging::Message& message);
  static void PDeny(ObjectId obj, const Messaging::Message& message);


  /**
   * \brief 
   * \return 
   */
  bool checktimer() {
    if (_timer <= 0.0f){
    return true;
  }
    return false;
  };
  void activateGodMode();
  void deactivateGodMode();
  void togglePause();
  bool godMode_ = false;
  private:
  void onInputF1(const Messaging::Message& message);
  void onInputLeft( const Messaging::Message& );
  void onInputRight( const Messaging::Message& );
  void onInputUp(const Messaging::Message&);
  void onInputDown(const Messaging::Message&);
  void onAnimationUpdate( const Messaging::Message& );
  void onCollisionHandle(const Messaging::Message&);
  void onInputShoot(const Messaging::Message& message);
  float _timer = 0.0f;
  float delay = 0.0935f;
  int dmg = 0;
  int coins = 0;
  bool CanShootRed = false;
  bool CanShootBlue = false;
  
  bool paused_ = false;


  ObjectId confirmKey_ = nullobj;
  /*new helpers*/
  void Shoot(const Messaging::Message& message);
  void PMenuHandle(const Messaging::Message& message);
  void PResetHandle(const Messaging::Message& message);
  void PQuitHandle(const Messaging::Message& message);
  void PConfirmHandle(const Messaging::Message& message);
  void PDenyHandle(const Messaging::Message& message);

  void backToMenu();
  void quit();
  void reset();

  /**
   * \brief 
   */
  enum ConfirmStatus
  {
    None,
    QuitMenu,
    QuitGame,
    ResetLevel
  };
  int confirm_status_ = None;

  bool backToMenu_ = false;
  bool quitting_ = false;
  bool resetting_ = false;

  /**
   * \brief 
   */
  enum state {
    idle,
    walking,
    jumping
  };
  /**
   * \brief 
   */
  enum facing {
    left,
    right
  };

  bool isWinnning_ = false;
  bool isShooting = false;
  state state_ = idle;
  facing facing_ = right;
};