/******************************************************************************/
/*!
  \file   stub.h
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef _COLLISIONNARROW_HPP_
#define _COLLISIONNARROW_HPP_

#include "Colliders.hpp"
#include <Messaging/Messaging.hpp>


/**
 * \brief 
 */
namespace KPE {
  /**
   * \brief 
   */
  class ContactResolver;
  //TASK: Create struct for potential collisions in CollisionBroad
  //struct PotentialCollision {
  //  BoundingBox* box;
  //  BoundingBox* other;
  //};
  
  //!This is a forward declaration of a contact
  struct Contact {
    //The two colliding bounds
    BoundingBox* bound_;
    BoundingBox* otherBound_;
    float penetration_;
    glm::vec2 contactNormal_;
    virtual ~Contact();
  };

  //Regular Contact between two bounding boxes
  struct ContactBB : public Contact {
    float restitution_;
  };

  //Contact between a hitbox and hurtbox;
  struct ContactHB : public Contact {
  };

  /**
   * \brief 
   */
  class ContactList {
  public:
    RigidBody* body;
    RigidBody* otherBody;
    std::vector<Contact*> contacts_;

    ~ContactList();
  };

  /**
   * \brief 
   */
  class CollisionDetector {
    std::vector<Colliders*>* registry_;
  public:
    void setRegistry(std::vector<Colliders*>* registry);
  
    //Generates contacts and returns number of contacts generated 
    int generateContactLists(ContactResolver& resolver);

  private:
    void fillContactList(int& numContacts, ContactList* list,
      std::vector<BoundingBox*>* bounds, std::vector<BoundingBox*>* otherBounds);
    void generateContact(ContactList* contactList, 
       BoundingBox* bound, BoundingBox* otherBound);
    bool contactCheck(ContactList* contactList, Contact* contact);

    float CollisionDetector::findPenetration(
      Transform* body, Transform* otherBody,
      BoundingBox* bound, BoundingBox* otherBound, glm::vec2 contactNormal) const;
    glm::vec2 findContactNormal(Transform* body, Transform* other) const;
    bool canCollide(BoundingBox* box, BoundingBox* other) const;
    void calculateData(Contact* contact) const;
    bool broadCheck(RigidBody* body, RigidBody* otherbody, 
      std::vector<BoundingBox*>* bounds, std::vector<BoundingBox*>* otherBounds);
  
  };

}


#endif