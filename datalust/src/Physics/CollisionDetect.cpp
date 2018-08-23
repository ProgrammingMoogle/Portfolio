#pragma once
/******************************************************************************/
  /*!
  \file   stub.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "CollisionDetect.hpp"
#include "ContactResolver.hpp"

#include <Transform/Transform.hpp>
#include <GameObject/GameObject.h>

/**
 * \brief 
 */
namespace KPE {

  /**
   * \brief 
   */
  Contact::~Contact() {

  }

  /**
   * \brief 
   */
  ContactList::~ContactList() {
    //Delete allocated data for contacts
    auto iter = contacts_.begin();
    for (; iter != contacts_.end(); ++iter) {
      delete *iter;
    }
  }

  /**
   * \brief 
   * \param registry 
   */
  void CollisionDetector::setRegistry(std::vector<Colliders*>* registry) {
    registry_ = registry;
  }
  
  /**
   * \brief 
   * \param resolver 
   * \return 
   */
  int CollisionDetector::generateContactLists(ContactResolver& resolver) {
    int numContacts = 0;
    auto colliderHolder = registry_->begin();     //Start
    auto colliderWalker = registry_->begin();     //Start +1

    //Check if there is more than 1 collider
    if (registry_->size() <= 1)
      return numContacts;
  
    //Find every contact and add it to the resolver
    //Iterate through the collider registry
    for (; colliderHolder != registry_->end(); ++colliderHolder) {
      if (!(*colliderHolder)->GetParent().IsActive())
        continue;

      //Must compare every registration
      colliderWalker = colliderHolder + 1;
      for (; colliderWalker != registry_->end(); ++colliderWalker) {

        if (!(*colliderWalker)->GetParent().IsActive())
          continue;
        //Above iterates through the collider registry
        //Below iterates through the bounds vector in the colider
        std::vector<BoundingBox*>* bounds = (*colliderHolder)->getBounds();
        std::vector<BoundingBox*>* otherBounds = (*colliderWalker)->getBounds();

        //Find RigidBody and Transform
        RigidBody* body = (*colliderHolder)->GetParent().Find<RigidBody>();
        RigidBody* otherBody = (*colliderWalker)->GetParent().Find<RigidBody>();

        //Broad Detect
        if (!broadCheck(body, otherBody, bounds, otherBounds)) {
          continue;
        }

        //Create a new List
       ContactList* tempList = new ContactList;

       //Check if two objects of infinite mass
       if (!(body->hasFiniteMass()) && !(otherBody->hasFiniteMass())) {
         delete tempList;
         continue;
       }

        //The first RigidBody MUST have a finite mass
        if (body->hasFiniteMass()) {
          tempList->body = body;
          tempList->otherBody = otherBody;
          fillContactList(numContacts, tempList, bounds, otherBounds);
        }
        else {
          tempList->body = otherBody;
          tempList->otherBody = body;
          fillContactList(numContacts, tempList, otherBounds, bounds);
        }

        //Add to the resolver only if contacts were made
        if (tempList->contacts_.size() > 0) {
          resolver.addContactList(tempList);
        }
        else {
          delete tempList;
        }

      }// Loop registrations
    }
  
    return numContacts;
  
  }

  /**
   * \brief 
   * \param numContacts 
   * \param list 
   * \param bounds 
   * \param otherBounds 
   */
  void CollisionDetector::fillContactList(int& numContacts, ContactList* list,
    std::vector<BoundingBox*>* bounds, 
    std::vector<BoundingBox*>* otherBounds) {

    //These variables hold pointers to the BoundArea Vectors
    auto boundsHolder = bounds->begin();

    for (; boundsHolder != bounds->end(); ++boundsHolder) {
      //Reset second vector
      auto boundsWalker = otherBounds->begin();

      for (; boundsWalker != otherBounds->end(); ++boundsWalker) {
        //Check if these bounds can collide
        if (canCollide((*boundsHolder), (*boundsWalker)))
        {
          //Check if those boxes are colliding
          if ((*boundsHolder)->overlaps((*boundsWalker))) {
            ++numContacts;
            //Create a contact
            generateContact(list, (*boundsHolder), (*boundsWalker));
          }
        }
      }
    }

  }

  /**
   * \brief 
   * \param contactList 
   * \param bound 
   * \param otherBound 
   */
  void CollisionDetector::generateContact(ContactList* contactList,
    BoundingBox* bound, BoundingBox* otherBound) {

    Transform* trans = contactList->body->GetParent().Find<Transform>();
    Transform* otherTrans = contactList->otherBody->GetParent().Find<Transform>();

    Contact* contact;

    //Check what kind of collisions it was
    //If it was a regular bounding box collision
    if (bound->getType() == BoundType::BoundBox) {
      ContactBB* tempContact = new ContactBB;

      //Find if the first body is immovable
      //The first body in the contact must be moveable

      //First object has infinite mass
      if (!(contactList->body->hasFiniteMass())) {
        //Set Bounds
        tempContact->bound_ = otherBound;
        tempContact->otherBound_ = bound;
        //Set Restitution
        tempContact->restitution_ =
          (contactList->body->getRestitution() + 
           contactList->otherBody->getRestitution()) / 2.0f;

        //Set Contact Normal and Penetration
        calculateData(tempContact);
      }
      //The first object does not have infinite mass
      else {
        //Set Bounds
        tempContact->bound_ = bound;
        tempContact->otherBound_ = otherBound;
        //Set Restitution
        tempContact->restitution_ =
          (contactList->body->getRestitution() + 
           contactList->otherBody->getRestitution()) / 2.0f;

        //Set Contact Normal and Penetration
        calculateData(tempContact);
      }

      contact = tempContact;
    }
    //This is a hitbox/hurtbox collision
    else {
      ContactHB* tempContact  = new ContactHB;
      tempContact->bound_ = bound;
      tempContact->otherBound_ = otherBound;
      //Set Contact Normal and Penetration
      calculateData(tempContact);
      contact = tempContact;
    }

    //Contact Check
    if (contactCheck(contactList, contact)) {
      //Add contact to list
      contactList->contacts_.push_back(contact);
    }
    else {
      delete contact;
    }
  }

  //Determines if the given contact should be applied over an existing contact
  //for the object
  /**
   * \brief 
   * \param contactList 
   * \param contact 
   * \return 
   */
  bool CollisionDetector::contactCheck(ContactList* contactList, Contact* contact) {

    //Check type of Contact
    if (contact->bound_->getType() == BoundType::BoundBox) {
      ContactBB* tempContact = static_cast<ContactBB*>(contact);

      //Walk through the list of contacts
      auto iter = contactList->contacts_.begin();
      for (; iter != contactList->contacts_.end(); ++iter) {
        //Check contact type is the same
        if ((*iter)->bound_->getType() == BoundType::BoundBox) {
          ContactBB* compare = static_cast<ContactBB*>((*iter));
          //Check if the normals are the same
          if (tempContact->contactNormal_ == compare->contactNormal_) {
            //The contact with the greater pentration should stay
            if (tempContact->penetration_ > compare->penetration_) {
              delete compare;
              contactList->contacts_.erase(iter);
              return true;
            }
            else {
              return false;
            }
          }
        }
      }

    }
    else if (contact->bound_->getType() == BoundType::BoundHitBox ||
             contact->bound_->getType() == BoundType::BoundHurtBox) {
      ContactHB* tempContact = static_cast<ContactHB*>(contact);

    }

    //There is no contact with the same normal
    return true;
  }
  
  float CollisionDetector::findPenetration(
    Transform* body, Transform* otherBody, 
    BoundingBox* bound, BoundingBox* otherBound, glm::vec2 contactNormal) const {
   
    glm::vec2 pos = body->getPosition();
    glm::vec2 otherPos = otherBody->getPosition();
    glm::vec2 penVec(0);

    pos *= contactNormal;
    otherPos *= contactNormal;

    penVec = pos - otherPos;

    return length(penVec);
  }

  /**
   * \brief 
   * \param body 
   * \param other 
   * \return 
   */
  glm::vec2 CollisionDetector::findContactNormal(Transform* body, Transform* other) const {
    glm::vec2 compare(0);
    //The vector of penetration of Body to Other
    if ((body->getOldPosition() - body->getPosition()) !=
      glm::vec2(0)) {
      compare = body->getOldPosition() - other->getPosition();
    }
    else {
      compare = other->getOldPosition() - body->getPosition();
    }

    if (compare.x > 0)
      compare.x = 1;
    else if (compare.x < 0)
      compare.x = -1;
    else
      compare.x = 0;

    if (compare.y > 0)
      compare.y = 1;
    else if (compare.y < 0)
      compare.y = -1;
    else
      compare.y = 0;

    if (compare.x != 0 && compare.y != 0)
    {
      compare.x = 0;
    }
    
    return compare;
  }
  
  /**
   * \brief 
   * \param box 
   * \param other 
   * \return 
   */
  bool CollisionDetector::canCollide(BoundingBox* box, BoundingBox* other) const {
    //BoundBoxes can collide with BoundBoxes
    if (box->getType() == BoundType::BoundBox) {
      if (other->getType() == BoundType::BoundBox) {
        return true;
      }
    }
    else {
      if (box->getType() == BoundType::BoundHitBox &&
          other->getType() == BoundType::BoundHurtBox) {
        return true;
      }
      else if (box->getType() == BoundType::BoundHurtBox &&
               other->getType() == BoundType::BoundHitBox) {
        return true;
      }
    }
    return false;
  }

  //Contact Normal is found with relation to the first body

  /**
   * \brief 
   * \param contact 
   */
  void CollisionDetector::calculateData(Contact* contact) const {
    glm::vec2 normal(0);
    float lengthX = 0;
    float lengthY = 0;
    float halfsizeTotal = 0;
    float distanceX = 0;
    float distanceY = 0;

    //x-axis check
    lengthX = contact->bound_->getPosition().x - contact->otherBound_->getPosition().x;
    halfsizeTotal = 
      contact->bound_->getHalfsize().x + contact->otherBound_->getHalfsize().x;
    distanceX = halfsizeTotal - abs(lengthX);


    //y-axis check
    lengthY = contact->bound_->getPosition().y - contact->otherBound_->getPosition().y;
    halfsizeTotal = 
      contact->bound_->getHalfsize().y + contact->otherBound_->getHalfsize().y;
    distanceY = halfsizeTotal - abs(lengthY);

    //Find the lowest penetration
    //Check if both axises are penetrating
    if (distanceX > 0 && distanceY > 0) {
      if (distanceX < distanceY) {
        //The x-axis has the lower penetration
        if (lengthX > 0)
          normal.x = 1.0f;
        else
          normal.x = -1.0f;
        contact->contactNormal_ = normal;
        contact->penetration_ = distanceX;
      }
      else {
        //The y-axis has the lower penetration
        if (lengthY > 0)
          normal.y = 1.0f;
        else
          normal.y = -1.0f;
        contact->contactNormal_ = normal;
        contact->penetration_ = distanceY;
      }
    }
    //only x-axis is penetrating
    else if (distanceX > 0) {
      if (lengthX > 0)
        normal.x = 1.0f;
      else
        normal.x = -1.0f;
      contact->contactNormal_ = normal;
      contact->penetration_ = distanceX;
    }
    //only y-axis is penetrating
    else if (distanceY > 0) {
      if (lengthY > 0)
        normal.y = 1.0f;
      else
        normal.y = -1.0f;
      contact->contactNormal_ = normal;
      contact->penetration_ = distanceY;
    }
  }


  /**
   * \brief 
   * \param n 
   * \return 
   */
  static constexpr float Square(float n)
  {
    return n * n;
  }

  /**
   * \brief 
   * \param body 
   * \param otherbody 
   * \param bounds 
   * \param otherBounds 
   * \return 
   */
  bool CollisionDetector::broadCheck(RigidBody* body, RigidBody* otherbody,
    std::vector<BoundingBox*>* bounds, std::vector<BoundingBox*>* otherBounds) {
    float width1, width2, height1, height2 = 0;
    float temp = 0;
    
    const glm::vec2 position = body->GetParent().Find<Transform>()->getPosition() + (*bounds).front()->getOffset();
    const glm::vec2 otherPosition = otherbody->GetParent().Find<Transform>()->getPosition() + (*otherBounds).front()->getOffset();
    const float distanceBW = Square(otherPosition.y - position.y) + Square(otherPosition.x - position.x);

    //Calculate the width and height of both objects
    width1 = (*bounds).at(0)->getOffset().x + (*bounds).at(0)->getHalfsize().x;
    height1 = (*bounds).at(0)->getOffset().y + (*bounds).at(0)->getHalfsize().y;
    width2 = (*otherBounds).at(0)->getOffset().x + (*otherBounds).at(0)->getHalfsize().x;
    height2 = (*otherBounds).at(0)->getOffset().y + (*otherBounds).at(0)->getHalfsize().y;

    for (auto&& box : *bounds) {
      temp = box->getOffset().x + box->getHalfsize().x;
      if (width1 < temp) {
        width1 = temp;
      }
      temp = box->getOffset().y + box->getHalfsize().y;
      if (height1 < temp) {
        height1 = temp;
      }
    }

    for (auto&& box : *otherBounds) {
      temp = box->getOffset().x + box->getHalfsize().x;
      if (width2 < temp) {
        width2 = temp;
      }
      temp = box->getOffset().y + box->getHalfsize().y;
      if (height2 < temp) {
        height2 = temp;
      }
    }

    float sqRad1 = 2 * (Square(width1) + Square(height1));
    float sqRad2 = 2 * (Square(width2) + Square(height2));

    if (distanceBW < sqRad1 + sqRad2) {
      return true;
    }
    return false;

  }
}