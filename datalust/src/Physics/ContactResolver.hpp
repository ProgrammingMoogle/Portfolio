/******************************************************************************/
/*!
  \file   ContactResolver.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.  
*/
/******************************************************************************/
#ifndef _CONTACTRESOLVER_HPP_
#define _CONTACTRESOLVER_HPP_

#include "CollisionDetect.hpp"
#include <vector>

/**
 * \brief 
 */
namespace KPE {
  /**
   * \brief 
   */
  class ForceRegistry;
  /**
   * \brief 
   */
  class ContactResolver {
    std::vector<ContactList*> contactLists_;
  public:
    void addContactList(ContactList* contactList);
    int resolveContacts(ForceRegistry& registry, float dt);

  private:
    void resolveVelocity(ContactBB* contact, 
      RigidBody* body, RigidBody* otherBody, float dt);
    void resolvePenetration(ContactBB* contact, 
      RigidBody* body, RigidBody* otherBody, float dt);

    float calculateSeparatingVelocity(ContactBB* contact, 
      RigidBody* body, RigidBody* otherBody) const;
    void positionalCorrection(
      Contact* contact, RigidBody* body, RigidBody* otherBody);

    void clear(void);
  };
}

#endif