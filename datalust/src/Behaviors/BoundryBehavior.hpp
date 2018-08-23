/**
  \file   BoundryBehavior.cpp
  \author Samuel Cook

  \brief Behavior component for camera boundries

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <GameObject/Component.h>
#include <Messaging/Messaging.hpp>

/**
 * \brief 
 */
class BoundryBehavior : public Component {
public:
  BoundryBehavior( GameObject& parent, ObjectId player );

  virtual ~BoundryBehavior();

  virtual Component* Clone_() const { return new BoundryBehavior( *this ); }

  void Update( float dt );

  static void CollisionHandler( ObjectId, const Messaging::Message& );

private:
  
  void onCollisionHandle(const Messaging::Message&);

  ObjectId player_;

  static float break_speed_sqr_;
};