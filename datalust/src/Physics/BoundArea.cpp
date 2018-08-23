/******************************************************************************/
/*!
  \file   BoundArea.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "BoundArea.hpp"
#include <Transform/Transform.hpp>
#include <vector>

namespace KPE {
  //----------------------------------------------------------------------------
  // BoundingBox Definition
  //----------------------------------------------------------------------------
  /**
   * \brief 
   * \param body 
   * \param xHalfsize 
   * \param yHalfsize 
   * \param xOffset 
   * \param yOffset 
   */
  BoundingBox::BoundingBox(Transform* body, float xHalfsize, float yHalfsize,
    float xOffset, float yOffset)
  :originalHalfsize_(xHalfsize, yHalfsize){

    halfsize_ = originalHalfsize_;
    offset_.x = xOffset;
    offset_.y = yOffset;
    position_ = body->getPosition() + offset_;
  }

  /**
   * \brief 
   * \param body 
   */
  void BoundingBox::update(Transform* body) {
#if 1
    //Update the box
    glm::mat2 orientationTrans = body->getOrientationTransform();
    glm::mat2 scaleTrans(0);
    scaleTrans[0].x = abs(body->getXScale());
    scaleTrans[1].y = abs(body->getYScale());

    //Set Position
    position_ = body->getPosition() + scaleTrans * offset_;

    //Transform original halfsize
    glm::vec2 tempHalf = originalHalfsize_;

    tempHalf = orientationTrans * tempHalf;
    tempHalf = scaleTrans * tempHalf;

    if (tempHalf == halfsize_) 
      return;
    halfsize_ = tempHalf;
#endif
  }

  /**
   * \brief 
   * \param other 
   * \return 
   */
  bool BoundingBox::overlaps(BoundingBox* other) const {
#if 1
    float lengthX = 0.0f; //Distance between boxes in x axis
    float lengthY = 0.0f; //Distance between boxes in y axis

    lengthX = abs(position_.x - other->position_.x);


    lengthY = abs(position_.y - other->position_.y);


    //Check halfsizes to check if overlapping in x or y axis.
    //DANGER: If halfsize is negative, it'll not work
    if (lengthX < halfsize_.x + other->halfsize_.x)
      if (lengthY < halfsize_.y + other->halfsize_.y)
        return true; 
#endif
    return false;

  }

  
  /**
   * \brief 
   * \return 
   */
  BoundType BoundingBox::getType(void) const {
    return BoundType::BoundBox;
  }

  /**
   * \brief 
   * \param xPos 
   * \param yPos 
   */
  void BoundingBox::setPosition(float xPos, float yPos) {
    position_.x = xPos;
    position_.y = yPos;
  }

  /**
   * \brief 
   * \return 
   */
  glm::vec2 BoundingBox::getPosition(void) const{
    return position_;
  }

  /**
   * \brief 
   * \param xHalf 
   * \param yHalf 
   */
  void BoundingBox::setHalfsize(float xHalf, float yHalf) {
    halfsize_.x = xHalf;
    halfsize_.y = yHalf;
  }

  /**
   * \brief 
   * \return 
   */
  glm::vec2 BoundingBox::getHalfsize(void) const{
    return halfsize_;
  }

  /**
   * \brief 
   * \param xOffset 
   * \param yOffset 
   */
  void BoundingBox::setOffset(float xOffset, float yOffset) {
    offset_.x = xOffset;
    offset_.y = yOffset;
  }

  /**
   * \brief 
   * \param offset 
   */
  void BoundingBox::setOffset(const glm::vec2& offset) {
    offset_ = offset;
  }

  /**
   * \brief 
   * \return 
   */
  glm::vec2 BoundingBox::getOffset() const {
    return offset_;
  }

  //----------------------------------------------------------------------------
  // BoundingHitBox Definition
  //----------------------------------------------------------------------------
  /**
   * \brief 
   * \param body 
   * \param xHalfsize 
   * \param yHalfsize 
   * \param xOffset 
   * \param yOffset 
   */
  BoundingHitBox::BoundingHitBox(Transform* body, float xHalfsize, float yHalfsize, 
    float xOffset, float yOffset)
    : BoundingBox(body, xHalfsize, yHalfsize, xOffset, yOffset) {
  }

  //Update
  void BoundingHitBox::update(Transform* body) {
    //Update position based on offset
    position_ = body->getPosition() + offset_;
  }

  /**
   * \brief 
   * \return 
   */
  BoundType BoundingHitBox::getType(void) const {
    return BoundType::BoundHitBox;
  }

  //----------------------------------------------------------------------------
  // BoundingHurtBox Definition
  //----------------------------------------------------------------------------
  /**
   * \brief 
   * \param body 
   * \param xHalfsize 
   * \param yHalfsize 
   * \param xOffset 
   * \param yOffset 
   */
  BoundingHurtBox::BoundingHurtBox(Transform* body, float xHalfsize, float yHalfsize, 
    float xOffset, float yOffset)
    : BoundingBox(body, xHalfsize, yHalfsize, xOffset, yOffset){
  }

  //Update
  /**
   * \brief 
   * \param body 
   */
  void BoundingHurtBox::update(Transform* body) {
    //Update position based on offset
    position_ = body->getPosition() + offset_;
  }

  /**
   * \brief 
   * \return 
   */
  BoundType BoundingHurtBox::getType(void) const {
    return BoundType::BoundHurtBox;
  }

  
}