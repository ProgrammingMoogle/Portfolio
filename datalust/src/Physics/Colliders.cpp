/******************************************************************************/
/*!
  \file   Colliders.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Colliders.hpp"
#include <GameObject/GameObject.h>
#include <Transform/Transform.hpp>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

namespace KPE {
  //----------------------------------------------------------------------------
  // Colliders Definition
  //----------------------------------------------------------------------------
  /**
   * \brief 
   */
  std::vector<Colliders*> Colliders::registry_;
  //std::vector<BoundingBox*> Colliders::bounds_;

  /**
   * \brief 
   * \param parent 
   */
  Colliders::Colliders(GameObject& parent) 
  : Component(parent),
    colSprite(*this)
  {
    registry_.push_back(this);
    right_ = 0;
    left_ = 0;
    top_ = 0;
    bottom_ = 0;
  }
  
  /**
   * \brief 
   */
  Colliders::~Colliders(void) {
    //Remove this collider from the registry when it is deleted.
    registry_.erase(
      std::remove(registry_.begin(), registry_.end(), this),
      registry_.end());

      for ( auto i : bounds_ ) {
      delete i;
    }

    if (bounds_.empty() != true)
    {
      for (int i = bounds_.size(); i > 0; --i)
      {
        bounds_.pop_back();
      }
    }
  }




  /**
   * \brief 
   * \param dt 
   */
  void Colliders::Update(float dt) {
#if 1
    if (bounds_.empty() == false)
    {
      auto iter = bounds_.begin();
      Transform* body = GetParent().Find<Transform>();

      for (; iter != bounds_.end(); ++iter) {
        (*iter)->update(body);
        if ((*iter)->getType() == BoundBox) {
          float temp = 0;
          float degree = (float)(body->getOrientation() / 180 * M_PI);
          float vDegree = (float)((body->getOrientation() + 90) / 180 * M_PI);

          temp = (*iter)->getOffset().x + (*iter)->getHalfsize().x;
          temp *= cosf(degree);
          if (right_ < temp)
            right_ = temp;

          temp = (*iter)->getOffset().x - (*iter)->getHalfsize().x;
          temp *= cosf(degree);
          if (left_ > temp)
            left_ = temp;

          temp = (*iter)->getOffset().y + (*iter)->getHalfsize().y;
          temp *= sinf(vDegree);
          if (top_ < temp)
            top_ = temp;

          temp = (*iter)->getOffset().y - (*iter)->getHalfsize().y;
          temp *= sinf(vDegree);
          if (bottom_ > temp)
            bottom_ = temp;
        }

      }
    }
#endif
  }


  /**
   * \brief 
   * \param type 
   * \param body 
   * \param xHalfsize 
   * \param yHalfsize 
   * \param xOffset 
   * \param yOffset 
   */
  void Colliders::add(BoundType type, Transform* body, float xHalfsize, float yHalfsize, 
           float xOffset, float yOffset) {

    switch(type) {
    case BoundType::BoundBox:
      {
        bounds_.push_back(new BoundingBox(body, xHalfsize, yHalfsize, xOffset, yOffset));
        
      }
      break;
    case BoundType::BoundHitBox:
      {
        bounds_.push_back(new BoundingHitBox(body, xHalfsize, yHalfsize, xOffset, yOffset));
      }
      break;
    case BoundType::BoundHurtBox:
      {
        bounds_.push_back(new BoundingHurtBox(body, xHalfsize, yHalfsize, xOffset, yOffset));
        break;
      }
    }
  }

  /**
   * \brief 
   * \param type 
   */
  void Colliders::removeType(BoundType type) {
    bounds_.erase(
      std::remove_if(bounds_.begin(), bounds_.end(), 
                     [type] (BoundingBox* bound) {
                        if (bound->getType() == type)
                          return true;
                        return false;
                     }),
      bounds_.end()
    );
  }

  //Raycasting
  //Check if colliding and return first object hit
  //bool Colliders::raycastForBody(RigidBody* body, glm::vec2 direction, float length) {
  //  auto iter = registry_.begin();
  //  for (; iter != registry_.end(); ++iter) {
  //    auto iterBounds = (*iter)->bounds_.begin();
  //    for (; iterBounds != (*iter)->bounds_.end(); ++iterBounds) {
  //      if ((*iterBounds)->getType == BoundBox) {
  //        //Create Temporary BoundingBox
  //        BoundingBox tempBox(GetParent().Find<Transform>(), )
  //      }
  //    }
  //  }
  //}
#if 1
  //Check if colliding with "something"
  /**
   * \brief 
   * \param direction 
   * \param length 
   * \return 
   */
  bool Colliders::raycastFromTrans(glm::vec2 direction, float length) {

    auto iter = registry_.begin();
    for (; iter != registry_.end(); ++iter) {
      if ((*iter) != this) {
        auto iterBounds = (*iter)->bounds_.begin();
        for (; iterBounds != (*iter)->bounds_.end(); ++iterBounds) {
          if ((*iterBounds)->getType() == BoundBox) {
            Segment line;
            line.point = GetParent().Find<Transform>()->getPosition();
            line.vector = direction * length;
            if (lineBoxCollision(*iterBounds, line)) {
              return true;
            }
          }
        }
      }
    }
    return false;

    return false;
  }
#endif
  //Component_::TypeEnum Colliders::GetComponentType() const {
  //  return Component_::TypeEnum::Colliders;
  //}

  /**
   * \brief 
   * \return 
   */
  std::vector<BoundingBox*>* Colliders::getBounds(void) {
    return &bounds_;
  }

  /**
   * \brief 
   * \return 
   */
  std::vector<Colliders*>* Colliders::getCollidersRegistry(void) {
    return &registry_;
  }
  

  /**
   * \brief 
   * \return 
   */
  float Colliders::getTop(void) const {
    return top_;
  }
  /**
   * \brief 
   * \return 
   */
  float Colliders::getBottom(void) const {
    return abs(bottom_);
  }
  /**
   * \brief 
   * \return 
   */
  float Colliders::getRight(void) const {
    return right_;
  }
  /**
   * \brief 
   * \return 
   */
  float Colliders::getLeft(void) const {
    return abs(left_);
  }


  /**
   * \brief 
   * \return 
   */
  Component* Colliders::Clone_() const {
    //WARNING: YOU SHOULDNT CLONE THIS CLASS
    //SHALLOW COPY
    return new Colliders(*this);
  }
  
  /**
   * \brief 
   * \param box 
   * \param line 
   * \return 
   */
  bool Colliders::lineBoxCollision(BoundingBox* box, Segment& line) {
    glm::vec2 pos = box->getPosition();
    //Create lines from the Box
    Segment upper;
    Segment right;
    Segment lower;
    Segment left;

    upper.point.x = pos.x - box->getHalfsize().x;
    upper.point.y = pos.y + box->getHalfsize().y;
    right.point.x = pos.x + box->getHalfsize().x;
    right.point.y = pos.y + box->getHalfsize().y;
    lower.point.x = pos.x + box->getHalfsize().x;
    lower.point.y = pos.y - box->getHalfsize().y;
    left.point.x = pos.x - box->getHalfsize().x;
    left.point.y = pos.y - box->getHalfsize().y;

    //Set Vectors
    upper.vector = right.point - upper.point;
    right.vector = lower.point - right.point;
    lower.vector = left.point - lower.point;
    left.vector = upper.point - left.point;


    if (lineCollision(line, upper) == true) {
      return true;
    }
    else if (lineCollision(line, right) == true) {
      return true;
    }
    else if (lineCollision(line, lower) == true) {
      return true;
    }
    else if (lineCollision(line, left) == true) {
      return true;
    }

    return false;
  }

  /**
   * \brief 
   * \param q 
   * \param p 
   * \return 
   */
  bool Colliders::lineCollision(Segment& q, Segment& p) {
    float t =
      cross(q.point - p.point, q.vector) /
      cross(p.vector, q.vector);
    float u = 
      cross(q.point - p.point, p.vector) /
      cross(p.vector, q.vector);

    float check1 = cross(p.vector, q.vector);
    float check2 = cross(q.point - p.point, p.vector);

    if (check1 == 0 && check2 == 0) {
      return true;
    }
    else if (check1 == 0 && check2 != 0) {
      return false;
    }
    else if (check1 != 0 && 
        (t >= 0 && t <= 1) &&
        (u >= 0 && u <= 1)) {
      return true;
    }
    return false;

  }

  /**
   * \brief 
   * \param vec 
   * \param otherVec 
   * \return 
   */
  float Colliders::cross(glm::vec2& vec, glm::vec2& otherVec) {
    return vec.x * otherVec.y - vec.y * otherVec.x;
  }
}