/*************************************************************************/
/*!
\file   HealthPickupComponent.h
\author studio 200
\par
\par    DigiPen login:
\par    Course: GAME 200
\par   Copyright � 2017 DigiPen (USA) Corp. and its owners. All Rights Reserved.
\date   9/28/2017
\brief
Functions include:
*/
/*************************************************************************/
#pragma once
#include <queue>
#include <BulletType.h>
#include "GameObject/Component.h"
#include "Graphics/ColorFade.hpp"

/**
 * \brief 
 */
namespace Graphics {
  class Sprite;
}


/**
 * \brief 
 */
enum HealthComponentType { PLAYER, ENEMY, DOOR, HCTNONE };


/**
 * \brief 
 * \param min 
 * \param max 
 * \return 
 */
static float RandomRange(float min, float max)
{
  return min + static_cast<float>(rand()) /
    (static_cast<float>(RAND_MAX / (max - min)));
}

/**
 * \brief 
 */
class HealthComponent :
  public Component
{
public:
  static bool OnePunchMan;
  HealthComponent(GameObject& parent, HealthComponentType type = HealthComponentType::HCTNONE, int health  = 0, BulletType bt = BulletType::NONE);
  ~HealthComponent();
  int GetHealth() const;
  void drop_coins();
  void  dmgFade();
  void checkhealth();
  void AddHealth(int amt);
  void playerdiesactions();
  void DoDamage(int c, BulletType bulletcolor = BulletType::BLUE);
  BulletType GetColor() const;
  //! clone function MUST BE IMPLEMENTED BY ALL CLASSES (recomended: implement using a copy ctor)
  virtual Component* Clone_() const;
  //! virtual update function SHOULD be implemented by all derived classes
  virtual void Update(float dt);

  virtual void Start();
  //! virtual draw function should be implemented by classes that need it
  virtual void Draw() const;
  HealthComponentType getType() const { return _type; };

  void activateGodMode_();
  void deactivateGodMode_();
  bool godMode_;
private:
  
  int _health;
  int coin;
  const int _MAXHEALTH;
  HealthComponentType _type; 
  std::queue<BulletType> colors;
  BulletType currcolor;
  BulletType nextcolor;
  bool gainHealthWrongHit;
  float healthhittimer = 0.0f;
  float delay = 5.13f;

  float flashDelay = 1.0f / 5.0f;
  float flashTimer = 0.0f;
  bool  redFlash = true;


  ColorFade colorFade;

  /**
   * \brief 
   */
  enum class State
  {
    INVALID = -1,
    Idle = 0,
    ChangeColors = 1,
    COUNT
  };

  State currentState = State::INVALID;
  State nextState = State::Idle;

  void HintNextColor();
  void AddColorsToList(int count);

  void IdleStart();
  void IdleUpdate(float dt);
  
  void ChangeColorsStart();
  void ChangeColorsUpdate(float dt);

  void PlayerUpdate(float dt);

  void StateStart(void);
  void StateUpdate(float dt);


  void sendDMGEvent();
};

