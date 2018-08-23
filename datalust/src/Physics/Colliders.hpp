/******************************************************************************/
/*!
  \file   Colliders.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef _COLLIDERS_HPP_
#define _COLLIDERS_HPP_

#include <vector>
#include <array>
#include "Graphics\ColliderSprite.hpp"

#include "BoundArea.hpp"

namespace KPE {

  /**
   * \brief 
   */
  struct Segment {
    glm::vec2 point;
    glm::vec2 vector;
  };

  /**
   * \brief 
   */
  class Colliders : public Component {
    static std::vector<Colliders*> registry_;
    std::vector<BoundingBox*> bounds_;
 public:

   private:

    float right_;
    float left_;
    float top_;
    float bottom_;

  public:
    Colliders(GameObject& parent);
    ~Colliders(void);

    void Update(float dt);

    void add(BoundType type, Transform* body, float xHalfsize, float yHalfsize, 
      float xOffset = 0, float yOffset = 0);

    void removeType(BoundType type);

    //Returns the first rigidbody that it collides with
    //bool raycastForBody(RigidBody* body, glm::vec2 direction, float length);
    bool raycastFromTrans(glm::vec2 direction, float length);

    //Component_::TypeEnum GetComponentType() const;

    std::vector<BoundingBox*>* getBounds(void);

    static std::vector<Colliders*>* getCollidersRegistry(void);

    float getTop(void) const;
    float getBottom(void) const;
    float getRight(void) const;
    float getLeft(void) const;

  private:
    Component* Clone_() const;
    bool lineBoxCollision(BoundingBox* box, Segment& line);
    bool lineCollision(Segment& line, Segment& otherLine);
    float cross(glm::vec2& q, glm::vec2& p);

      // discreetly sends render information to Graphics
    Graphics::ColliderSprite colSprite;
  };
}

#endif