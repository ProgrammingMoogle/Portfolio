/******************************************************************************/
/*!
  \file   BoundArea.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef _BOUNDAREA_HPP_
#define _BOUNDAREA_HPP_

#include "RigidBody.hpp"

/**
 * \brief 
 */
class Transform;

/**
 * \brief 
 */
namespace KPE {

  //----------------------------------------------------------------------------
  // BoundingBox Definition
  //----------------------------------------------------------------------------
  /**
   * \brief 
   */
  class BoundingBox {
  protected:
    //World Position
    glm::vec2 position_;
    //Halfsize vector (Original)
    glm::vec2 originalHalfsize_;
    //Halfsize vector (Current)
    glm::vec2 halfsize_;
    //Offset from attached body
    glm::vec2 offset_;
    //Rotation
    float rotation_;
  public:
    //Constructors
    BoundingBox(Transform* body, float xHalfsize, float yHalfsize, 
      float xOffset, float yOffset);

    //Destructor
    virtual ~BoundingBox(void) {};

    //Methods

      //Modifies the halfsize to match associated body's orientation
      //Updates the boundingbox using the transform it is tied to
    virtual void update(Transform* body);
      //Overlap
    bool overlaps(BoundingBox* other) const;
      //Return Type
    virtual BoundType getType(void) const;

    void setPosition(float xPos, float yPos);
    glm::vec2 getPosition(void) const;
    void setHalfsize(float xHalf, float yHalf);
    glm::vec2 getHalfsize(void) const;
    //Offset Setting
    void setOffset(float xOffset, float yOffset);
    void setOffset(const glm::vec2& offset);
    glm::vec2 getOffset() const;

  private:
  };

  //----------------------------------------------------------------------------
  // BoundingHitBox Definition
  //----------------------------------------------------------------------------
  /**
   * \brief 
   */
  class BoundingHitBox : public BoundingBox {
  public:
    //Constructors
    BoundingHitBox(Transform* body, float xHalfsize, float yHalfsize, 
      float xOffset = 0, float yOffset = 0);
    //Update
    void update(Transform* body);
    //Offset Setting
    void setOffset(float xOffset, float yOffset);
    void setOffset(const glm::vec2& offset);
    glm::vec2 getOffset() const;
    //Type
    BoundType getType(void) const;
  };

  //----------------------------------------------------------------------------
  // BoundingHurtBox Definition
  //----------------------------------------------------------------------------
  /**
   * \brief 
   */
  class BoundingHurtBox : public BoundingBox {
  public:
    //Constructors
    BoundingHurtBox(Transform* body, float xHalfsize, float yHalfsize, 
      float xOffset = 0, float yOffset = 0);    
    //Update
    void update(Transform* body);
    //Offset Setting
    void setOffset(float xOffset, float yOffset);
    void setOffset(const glm::vec2& offset);
    glm::vec2 getOffset() const;
    //Type
    BoundType getType(void) const;
  };


  //TASK: LATER IMPLEMENTATION
  /**
   * \brief 
   */
  class BoundingCircle {

  };

  /**
   * \brief 
   */
  class BoundingHitCircle : public BoundingCircle {

  };

  /**
   * \brief 
   */
  class BoundingHurtCircle : public BoundingCircle {

  };

}

#endif