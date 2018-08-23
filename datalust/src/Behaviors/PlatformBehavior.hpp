/******************************************************************************/
/*!
  \file   PlatformBehavior.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief  Behavior component for semi-solid platforms
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include <GameObject/Component.h>
#include <Messaging/Messaging.hpp>

/**
 * \brief 
 */
class PlatformBehavior : public Component {
public:
  PlatformBehavior( GameObject& parent );
  ~PlatformBehavior();

  virtual Component* Clone_() const { return new PlatformBehavior( *this ); }

  static void CollisionHandler( ObjectId, const Messaging::Message& );

  void Update( float dt );

private:
  void onCollisionHandler( const Messaging::Message& );
};