/**
  \file   Player2Behavior.hpp
  \author Zayd Gaudet

  \brief Behavior component for player 2

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <GameObject\Component.h>
#include <Messaging\Messaging.hpp>

class Player2Behavior : public Component {
public:
  Player2Behavior( GameObject &parent );
  ~Player2Behavior();
  virtual Component* Clone_() const { return new Player2Behavior(*this); }
  virtual void Update(float dt);
  void AddToDMG(float NewDMG) { dmg += NewDMG; }
  float DMG() { return dmg; }
  static void InputLeft( ObjectId, const Messaging::Message& );
  static void InputRight( ObjectId, const Messaging::Message& );
  static void InputJump( ObjectId, const Messaging::Message& );
  static void InputPunch( ObjectId, const Messaging::Message& );
  static void AnimationUpdate( ObjectId, const Messaging::Message& );
private:
  void onInputLeft( const Messaging::Message& );
  void onInputRight( const Messaging::Message& );
  void onInputJump( const Messaging::Message& );
  void onInputPunch( const Messaging::Message& );
  void onAnimationUpdate( const Messaging::Message& );
  float dmg = 0;
  enum state {
    idle,
    walking,
    jumping
  };
  enum facing {
    left,
    right
  };
  bool isPunching_ = false;
  state state_ = idle;
  facing facing_ = left;

  int jumps_ = 0;
};