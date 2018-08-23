/******************************************************************************/
/*!
  \file   CheatOptions.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include <GameObject/Component.h>
#include <Messaging/Messaging.hpp>
#include <Physics/RigidBody.hpp>


/**
 * \brief 
 */
class CheatOptions: public Component {
  bool cheatActive_;
  bool isFullscreen_;
  bool godMode_;
  bool musicMuted_;
  float prevMusicVol_;
  bool HasWon = false;
  float delay = 5.0f;
  float timer = delay;
public:
  CheatOptions(GameObject& parent);
  ~CheatOptions();
  virtual Component* Clone_() const { return new CheatOptions(*this); }
  void Update(float dt);

  static void InputF1(ObjectId, const Messaging::Message&);
  static void InputF2(ObjectId, const Messaging::Message&);
  static void InputF3(ObjectId, const Messaging::Message&);
  static void InputF4(ObjectId, const Messaging::Message&);
  static void InputF5(ObjectId, const Messaging::Message&);
  static void InputF6(ObjectId, const Messaging::Message&);
  static void InputF7(ObjectId, const Messaging::Message&);

private:
  void showCredits();
  void toggleFullscreen();
  void muteMusic();
  void skipLevel();
  void winGame();
  void godMode();
  void loseGame();

  void placeInCorner();
};
