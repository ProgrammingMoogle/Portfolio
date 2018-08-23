#pragma once
#include "TutorialWallBehavior.hpp"
#include <Engine/Engine.h>
#include "BulletBehavior.h"


TutorialWallBehavior::TutorialWallBehavior(GameObject &parent, bool isRed):
Component(parent), isRed_(isRed){

}
TutorialWallBehavior::~TutorialWallBehavior() {

}
void TutorialWallBehavior::Update(float dt) {

}

void TutorialWallBehavior::CollisionHandle(
  ObjectId obj, const Messaging::Message& message) {

  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);
  TutorialWallBehavior* pb = object.Find<TutorialWallBehavior>();
  pb->onCollisionHandle(message);
}

void TutorialWallBehavior::onCollisionHandle(const Messaging::Message& message) {
  KPE::RigidBody* otherBody;
  
  //Find body of the object that this wall collided with
  if (message.message.collision.body_ == GetParent().Find<KPE::RigidBody>()) {
    otherBody = message.message.collision.otherBody_;
  }
  else {
    otherBody = message.message.collision.body_;
  }

  //Check if it was a bullet
  BulletBehavior* check = otherBody->GetParent().Find<BulletBehavior>();

  if (check) {
    //If it collided with a bullet
    //Check color
    if (isRed_ == false && check->GetBullet() == BLUE) {
      GetParent().Destroy();
    }
    else if (isRed_ == true && check->GetBullet() == RED) {
      GetParent().Destroy();
    }
  }
  else {
    return;
  }
}