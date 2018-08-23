#pragma once
#include <GameObject\Component.h>
#include <Messaging\Messaging.hpp>
class BaseAI_Behavior: public Component
{
public:  
  BaseAI_Behavior(GameObject &parent);
  virtual Component* Clone_() const { return new BaseAI_Behavior(*this); }
  virtual ~BaseAI_Behavior() {}
  //! clone function MUST BE IMPLEMENTED BY ALL CLASSES (recomended: implement using a copy ctor)
  void MoveAI(float DT);
  //! virtual update function SHOULD be implemented by all derived classes
  virtual void Update(float dt) { UNREFERENCED_PARAMETER(dt); }
  //! virtual draw function should be implemented by classes that need it
  virtual void Draw() const {}
  float DMG() { return dmg; }
  private:
    float dmg;
    float _timer;
    float _timer2;
    float _angle;
    float  _randValue;
    float RandomFloatRange(float begin, float end);
};
