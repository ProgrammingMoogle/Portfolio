#pragma once

#include <GameObject\Component.h>
#include <Messaging\Messaging.hpp>

class TutorialWallBehavior : public Component {
  bool isRed_;
public:
  TutorialWallBehavior(GameObject &parent, bool isRed);
  ~TutorialWallBehavior();
  virtual Component* Clone_() const { return new TutorialWallBehavior(*this); }
  virtual void Update(float dt);
private:
  void CollisionHandle(ObjectId, const Messaging::Message&);
  void onCollisionHandle(const Messaging::Message& message);
};