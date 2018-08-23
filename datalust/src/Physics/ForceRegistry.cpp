#pragma once
/******************************************************************************/
/*!
  \file   ForceRegistry.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ForceRegistry.hpp"

#include <algorithm>

/**
 * \brief 
 */
namespace KPE {
  /**
   * \brief 
   * \param body 
   * \param forceGen 
   */
  void ForceRegistry::add(RigidBody* body, ForceGenerator* forceGen) {
    
    ForceRegistration reg;

    reg.body_ = body;
    reg.fGen_ = forceGen->clone();
    
    registry_.push_back(reg);
  }

  /**
   * \brief 
   * \param body 
   * \param forceGen 
   */
  void ForceRegistry::remove(RigidBody* body, ForceGenerator* forceGen) {
    auto iter = registry_.begin();

    for (; iter != registry_.end(); ++iter) {
      //Check if the body is equal
      if (iter->body_ == body) {
        if (*(iter->fGen_) == forceGen) {
          delete iter->fGen_;
          //Delete allocated data first
          registry_.erase(iter);
          break;
        }
      }
    }
  }

  /**
   * \brief 
   * \param body 
   * \param type 
   * \param emitter 
   */
  void ForceRegistry::removeByType(RigidBody* body, ForceType type, bool emitter) {
    auto iter = registry_.begin();

    for (; iter != registry_.end(); ++iter) {
      //Check if the body is equal
      if (iter->body_ == body) {
        //Check if the types are equal
        //WARNING::MULTIPLE OF THE SAME TYPE OF GENERATOR ATTACHED TO 
        //ONE OBJECT MAY CAUSE WRONG BEHAVIOR
        if (iter->fGen_->getType() == type) {
          if (emitter == iter->fGen_->isEmitterForce()) {
            delete iter->fGen_;
            //Delete allocated data first
            registry_.erase(iter);
          }
          break;
        }
      }
    }
  }

  /**
   * \brief 
   */
  void ForceRegistry::clear(void) {
    auto iter = registry_.begin();

    //Delete all allocated data
    for (; iter != registry_.end(); ++iter) {
      delete iter->fGen_;
    }

    registry_.clear();
  }

  /**
   * \brief 
   * \param dt 
   */
  void ForceRegistry::updateForces(float dt) {
    auto iter = registry_.begin();
    //Update every force in list
    for (; iter != registry_.end(); ++iter) {
      iter->fGen_->updateForce(iter->body_, dt);
    }
  }
}