#pragma once
/******************************************************************************/
/*!
  \file   ContactResolver.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ContactResolver.hpp"

#include "ForceRegistry.hpp"

#include <GameObject/Component.h>
#include <GameObject/GameObject.h>
#include <Transform/Transform.hpp>

#include <Messaging/Messaging.hpp>
#include <Engine/Engine.h>

/**
 * \brief 
 */
namespace KPE {
  /**
   * \brief 
   * \param contactList 
   */
  void ContactResolver::addContactList(ContactList* contactList) {
    contactLists_.push_back(contactList);
  }

  /**
   * \brief 
   * \param registry 
   * \param dt 
   * \return 
   */
  int ContactResolver::resolveContacts(ForceRegistry& registry, float dt) {
    int numContacts = 0;
    Messaging::MessageHandler* mHandler = engine.Find<Messaging::MessageHandler>();

    //Loop through the vector of ContactLists
    auto iterList = contactLists_.begin();
    for (; iterList != contactLists_.end(); ++iterList) {
      RigidBody* body = (*iterList)->body;
      RigidBody* otherBody = (*iterList)->otherBody;

      //Loop through the list of contacts
      auto iterContact = (*iterList)->contacts_.begin();
      for (; iterContact != (*iterList)->contacts_.end(); ++iterContact) {
        ++numContacts;

        //Check what type of contact it is
        if ((*iterContact)->bound_->getType() == BoundType::BoundBox) {
          ContactBB* tempContact = static_cast<ContactBB*>((*iterContact));
          bool pass = true;

         
            //Possible semisolid interaction
          Messaging::Message msg;
          msg.id = Messaging::ID::Collision;
          msg.message.collision.hit_ = false;
          msg.message.collision.pass_ = &pass;
          msg.message.collision.body_ = body;
          msg.message.collision.otherBody_ = otherBody;
          msg.message.collision.normal_ = tempContact->contactNormal_;
          msg.message.collision.penetration_ = tempContact->penetration_;
          mHandler->Post(msg);
          

          if (pass) {
            //Resolve Velocity
            resolveVelocity(tempContact, body, otherBody, dt);

            //Resolve Interpenetration
            if (tempContact->penetration_ > 0.0f)
              resolvePenetration(tempContact, body, otherBody, dt);
            //positionalCorrection(tempContact, body, otherBody);
          }
        }
        else {
          ContactHB* tempContact = static_cast<ContactHB*>((*iterContact));
          //Send to messaging
          Messaging::Message msg;
          msg.id = Messaging::ID::Collision;
          msg.message.collision.hit_ = true;
          //Order RigidBodies (Hitbox, Hurtbox)
          if (tempContact->bound_->getType() == BoundHitBox) {
            msg.message.collision.body_ = body;
            msg.message.collision.otherBody_ = otherBody;
            msg.message.collision.normal_ = tempContact->contactNormal_;
            msg.message.collision.penetration_ = tempContact->penetration_;
          }
          else if (tempContact->bound_->getType() == BoundHurtBox) {
            msg.message.collision.body_ = otherBody;
            msg.message.collision.otherBody_ = body;
            msg.message.collision.normal_ = -(tempContact->contactNormal_);
            msg.message.collision.penetration_ = tempContact->penetration_;
          }
          mHandler->Post(msg);
        }
      }
      //Delete ContactList
    }


    //Clear the list vector
    clear();

    return numContacts;
  }

  /**
   * \brief 
   * \param contact 
   * \param body 
   * \param otherBody 
   * \param dt 
   */
  void ContactResolver::resolveVelocity(ContactBB* contact,
    RigidBody* body, RigidBody* otherBody, float dt) {
    glm::vec2 tempVel(0); //Temporary velocity vector


    //Get Total Separating Velocity
    float separatingVelocity = calculateSeparatingVelocity(contact, body, otherBody);

    //If no separating velocity, no need to resolve velocity
    //Separating velocity must be negative. Positive means they are separating, not colliding
    if (separatingVelocity > 0)
      return;

    //Apply restitution
    float newSepVel = -separatingVelocity * contact->restitution_;

    //Find velocity caused by acceleration
    glm::vec2 accCausedVel = body->getLastAcceleration();
    if (otherBody)
      accCausedVel -= otherBody->getLastAcceleration();
    float accCausedSepVel = dot(accCausedVel, contact->contactNormal_) * dt;

    //Apply acceleration velocity to separating velocity
    if (accCausedSepVel < 0) {
      newSepVel += -accCausedSepVel * contact->restitution_;
      if (newSepVel < 0)
        newSepVel = 0;
    }

    //Find the delta velocity
    float deltaVel = newSepVel - separatingVelocity;

    //Find total inverse mass of both bodies
    float totalInverseMass = body->getInverseMass();
    if (otherBody)
      if (otherBody->hasFiniteMass())
        totalInverseMass += otherBody->getInverseMass();


    if (totalInverseMass <= 0)
      return;

    //Find impulse magnitude per inverse mass
    float impulse = deltaVel / totalInverseMass;

    //Find impulse per inverse mass
    glm::vec2 impulsePerInverseMass = contact->contactNormal_ * impulse;

    //Apply velocities
    tempVel = body->getVelocity();
    tempVel += impulsePerInverseMass * body->getInverseMass();
    body->setVelocity(tempVel);
    if (otherBody) {
      tempVel = otherBody->getVelocity();
      tempVel -= impulsePerInverseMass * otherBody->getInverseMass();
      otherBody->setVelocity(tempVel);
    }
  }

  /**
   * \brief 
   * \param contact 
   * \param body 
   * \param otherBody 
   * \param dt 
   */
  void ContactResolver::resolvePenetration(ContactBB* contact,
    RigidBody* body, RigidBody* otherBody, float dt) {

    Transform* trans = body->GetParent().Find<Transform>();
    Transform* otherTrans = otherBody->GetParent().Find<Transform>();

    if (!(otherBody->hasFiniteMass())) {
      trans->setPosition(
        trans->getPosition() + contact->penetration_ * contact->contactNormal_);
      return;
    }

    glm::vec2 tempVel(0);

    if (contact->penetration_ <= 0)
      return;

    if ((body->GetParent().GetName() == "Player1" &&
      otherBody->GetParent().GetName() == "Player2") ||
      (body->GetParent().GetName() == "Player2" &&
        otherBody->GetParent().GetName() == "Player1"))
    {
      tempVel.x = 0;
    }

    //Neither are immovable.
    float totalInverseMass = body->getInverseMass();
    if (otherBody)
      if (otherBody->hasFiniteMass())
        totalInverseMass += otherBody->getInverseMass();


    if (totalInverseMass <= 0)
      return;

    glm::vec2 pulsePerInverseMass =
      contact->contactNormal_ * (contact->penetration_ / totalInverseMass);

    glm::vec2 tempShift(0);

    tempShift = pulsePerInverseMass * body->getInverseMass();
    tempShift += trans->getPosition();
    trans->setPosition(tempShift);

    if (otherBody) {
      tempShift = -pulsePerInverseMass * otherBody->getInverseMass();
      tempShift += otherTrans->getPosition();
      otherTrans->setPosition(tempShift);
    }

  }


  /**
   * \brief 
   * \param contact 
   * \param body 
   * \param otherBody 
   * \return 
   */
  float ContactResolver::calculateSeparatingVelocity(ContactBB* contact,
    RigidBody* body, RigidBody* otherBody) const {

    glm::vec2 relativeVel = body->getVelocity();
    if (otherBody)
      relativeVel -= otherBody->getVelocity();


    return dot(relativeVel, contact->contactNormal_);
  }


  /**
   * \brief 
   * \param contact 
   * \param body 
   * \param otherBody 
   */
  void ContactResolver::positionalCorrection(
    Contact* contact, RigidBody* body, RigidBody* otherBody) {
    Transform* trans = body->GetParent().Find<Transform>();
    Transform* otherTrans = otherBody->GetParent().Find<Transform>();
    float percentage = 0.2f;
    glm::vec2 correction = contact->penetration_ /
      (body->getInverseMass() + otherBody->getInverseMass()) * percentage *
      contact->contactNormal_;

    trans->setPosition(
      trans->getPosition() + correction * body->getInverseMass());
    otherTrans->setPosition(
      otherTrans->getPosition() + correction * otherBody->getInverseMass());

  }


  /**
   * \brief 
   */
  void ContactResolver::clear(void) {
    auto iter = contactLists_.begin();
    for (; iter != contactLists_.end(); ++iter) {
      delete *iter;
    }
    contactLists_.clear();
  }
}
