/******************************************************************************/
/*!
  \file   Physics.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Physics.hpp"
#include "json/json.h"
#include <fstream>
#include <time.h>

/**
 * \brief 
 */
namespace KPE {
  glm::vec2 GRAV_VEC(0.0f,0.0f);
  float LINEAR_DAMP_BASE = 0;
  float ANGULAR_DAMP_BASE = 0;
  float DYNAMIC_MULT = 0;

  /**
   * \brief 
   */
  void PhysicsEngine::init() {
    std::srand((unsigned)time(0));
	// Open the json file so we can serialize it
	std::ifstream inFile("data/physics/PhysicsConstants.json");
    Json::Value root;
    if (inFile.is_open()) {
	  // create a Json Char Reader to read the filebuffer into a json value
	  Json::Reader parser;
	  // read the file into the jason value
	  parser.parse(inFile, root);

      LINEAR_DAMP_BASE = root["Linear Dampening"].asFloat();
      ANGULAR_DAMP_BASE = root["Angular Dampening"].asFloat();
      DYNAMIC_MULT = root["Dynamics Multiplier"].asFloat();

      //GRAV_VEC.y = root["Gravity"].asFloat();
    }
  }


  /**
   * \brief 
   * \param dt 
   */
  void PhysicsEngine::update(float dt) {

    //Pause
    if (paused_) {
      return;
    }

    //Rigidbody and Colliders updated by engine
    
    for (unsigned i = 0; i < 1; ++i) {
      //Debug Info
      int contactsGenerated = 0;
      int contactsResolved = 0;

      //Grab registry of colliders and pass into collision detection
      narrowDetection_.setRegistry(Colliders::getCollidersRegistry());
      contactsGenerated = narrowDetection_.generateContactLists(contactResolver_);

      if (contactsGenerated > 0) {
        //Resolve Collisions
        contactsResolved = contactResolver_.resolveContacts(forceRegistry_, dt);
      }

      
    }
    //Apply Forces
    forceRegistry_.updateForces(dt);
    
  }

  /**
   * \brief 
   */
  void PhysicsEngine::togglePause() {
    if (!paused_) {
      paused_ = true;
    }
    else {
      paused_ = false;
    }
  }
}