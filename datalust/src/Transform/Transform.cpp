/******************************************************************************/
/*!
  \file   Transform.cpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Transform.hpp"
#include <GameObject/GameObject.h>
#define _USE_MATH_DEFINES
#include <math.h>

//*---------------------------------------------------------------------------
//* Constructors and Destructors
//*---------------------------------------------------------------------------
/*************************************************************************/
/*!
  \brief
  Default constructor for Transform
*/
/*************************************************************************/
Transform::Transform(GameObject& parent) 
: Component(parent), position_(0), oldPosition_(0),
  orientationTransform_(1), scale_(1), transform_(1), glTransform_(1) 
{
  //Everything else can be default identity
  orientation_ = 0;
  isDirty_ = false;
}

/*************************************************************************/
/*!
  \brief
  Virtual destructor for inheritance
*/
/*************************************************************************/
Transform::~Transform(void) {
}

// Update
/**
 * \brief 
 */
void Transform::Update(float) {
  if (!isDirty_)
    return;


  //Degrees to Radians
  float rad = (float)(orientation_ * M_PI / 180.0f);

  //Generate Orientation Transform
  orientationTransform_[0].x = cos(rad);
  orientationTransform_[0].y = sin(rad);
  orientationTransform_[0].z = 0;
  orientationTransform_[1].x = sin(rad) * -1.0f;
  orientationTransform_[1].y = cos(rad);
  orientationTransform_[1].z = 0;
  orientationTransform_[2].x = 0;
  orientationTransform_[2].y = 0;
  orientationTransform_[2].z = 1;

  //Set Transform to Orientation
  transform_ = orientationTransform_;

  //Set translation
  oldPosition_.x = transform_[2].x;
  oldPosition_.y = transform_[2].y;

  transform_[2].x = position_.x;
  transform_[2].y = position_.y;

  //Apply scale
  transform_ *= scale_;

  //Set GL Matrix using transform
  fillGlMatrix();
}


//*---------------------------------------------------------------------------
//* Position Accessors
//*---------------------------------------------------------------------------

/*************************************************************************/
/*!
  \brief
  Sets position, given a vector2
  \param pos
  Position to set to
*/
/*************************************************************************/
void Transform::setPosition(const glm::vec2& pos) {
  if (position_ != pos) {
    oldPosition_ = position_;
    setDirty(true);
    position_ = pos;
  }
}

/*************************************************************************/
/*!
  \brief
  Sets position, given values
  \param x
  x position to set to
  \param y
  y position to set to
*/
/*************************************************************************/
void Transform::setPosition(float x, float y) {
  if (position_.x != x || position_.y != y) 
  {
    oldPosition_.x = position_.x;
    oldPosition_.y = position_.y;
    setDirty(true);
    position_.x = x;
    position_.y = y;
  }
}

/*************************************************************************/
/*!
  \brief
  Gives the position by returning a Vector2 with this position values
  \return
  Copy of body's position vector in Vector2
*/
/*************************************************************************/
glm::vec2 const& Transform::getPosition(void) const {
  return position_;
}

/*************************************************************************/
/*!
  \brief
  Gives the old position by returning a Vector2 with this position values
  \return
  Copy of body's position vector in Vector2
*/
/*************************************************************************/
glm::vec2 Transform::getOldPosition(void) const {
  return oldPosition_;
}

//*---------------------------------------------------------------------------
//* Orientation Accessors
//*---------------------------------------------------------------------------
/**
 * \brief 
 * \param angle 
 */
void Transform::setOrientation(float angle) {
  if (orientation_ != angle) {
    setDirty(true);
    orientation_ = angle;
  }
}

/**
 * \brief 
 * \return 
 */
float Transform::getOrientation(void) const {
  return orientation_;
}

/*************************************************************************/
/*!
  \brief
  Gives body orientation by returning a copy
  \return
  Copy of body orientation
*/
/*************************************************************************/
glm::mat3 Transform::getOrientationTransform(void) const {
  return orientationTransform_;
}

//*---------------------------------------------------------------------------
//* Scale Accessors
//*---------------------------------------------------------------------------
/*************************************************************************/
/*!
  \brief
  Sets x scale
  \param x
  Scale to set x scale to
*/
/*************************************************************************/
void Transform::setXScale(float x) {
  if (scale_[0].x != x) {
    setDirty(true);
    scale_[0].x = x; 
  }
}

/*************************************************************************/
/*!
  \brief
  Sets y scale
  \param x
  Scale to set y scale to
*/
/*************************************************************************/
void Transform::setYScale(float y) {
  if (scale_[1].y != y) {
    setDirty(true);
    scale_[1].y = y;
  }
}

/*************************************************************************/
/*!
  \brief
  Gets x scale;
  \return
  Returns x scale;
*/
/*************************************************************************/
float Transform::getXScale(void) const {
  return scale_[0].x;
}

/*************************************************************************/
/*!
  \brief
  Gets y scale;
  \return
  Returns y scale;
*/
/*************************************************************************/
float Transform::getYScale(void) const {
  return scale_[1].y;
}

/**
 * \brief 
 * \return 
 */
glm::mat3 const& Transform::getScale() const
{
  return scale_;
}

//*---------------------------------------------------------------------------
//* Transform Accessors
//*---------------------------------------------------------------------------
/*************************************************************************/
/*!
  \brief
  Gives the body's transform through a given pointer
  \param transform
  Matrix3 pointer to store transform to
*/
/*************************************************************************/
void Transform::getTransform(glm::mat3* transform) const {
  *transform = transform_;
}

/*************************************************************************/
/*!
  \brief
  Gives the body's transform though a given pointer
  \param transform
  Matrix4 pointer to store transform to
*/
/*************************************************************************/
void Transform::getGLTransform(glm::mat4* transform) const {
  *transform = glTransform_;
}

/*************************************************************************/
/*!
  \brief
  Gives transform by returning a copy
  \return
  Copy of body's transform matrix
*/
/*************************************************************************/
glm::mat3 const& Transform::getTransform(void) const {
  return transform_;
}

/*************************************************************************/
/*!
  \brief
  Gives GL transform by returning a copy
  \return
  Copy of body's GL transform matrix
*/
/*************************************************************************/
glm::mat4 const& Transform::getGLTransform(void) const {
  return glTransform_;
}

//*---------------------------------------------------------------------------
//* Dirty Flag
//*---------------------------------------------------------------------------
/**
 * \brief 
 * \return 
 */
bool Transform::isDirty(void) const {
  return isDirty_;
}

/**
 * \brief 
 * \param isDirty 
 */
void Transform::setDirty(bool isDirty) {
  isDirty_ = isDirty;
}

//*---------------------------------------------------------------------------
//* Point and Vector Conversions (World to Local) (Local to World)
//*---------------------------------------------------------------------------
/*************************************************************************/
/*!
  \brief
  Converts given point from world to local space
  \param point
  Point to convert given in Vec2
  \return
  Converted point
*/
/*************************************************************************/
glm::vec2 Transform::getPointWorldToLocal(const glm::vec2& point) const {
  glm::mat3 inverseMat = inverse(transform_);
  //Tag a 1 at end because it is a point
  glm::vec3 point_(point, 1.0f);

  return inverseMat * point_;
}

/*************************************************************************/
/*!
  \brief
  Converts given point from local to world space
  \param point
  Point to convert given in Vec2
  \return
  Converted point
*/
/*************************************************************************/
glm::vec2 Transform::getPointLocalToWorld(const glm::vec2& point) const {
  //Tag a 1 at end because it is a point
  glm::vec3 point_(point, 1.0f);
  
  return transform_ * point_;
}

/*************************************************************************/
/*!
  \brief
  Converts given point from world to local space
  \param point
  Point to convert given in Vec3
  \return
  Converted point
*/
/*************************************************************************/
glm::vec3 Transform::getPointWorldToLocal(const glm::vec3& point) const {
  glm::mat3 inverseMat = inverse(transform_);
  return inverseMat * point;
}

/*************************************************************************/
/*!
  \brief
  Converts given point from local to world space
  \param point
  Point to convert given in Vec3
  \return
  Converted point
*/
/*************************************************************************/
glm::vec3 Transform::getPointLocalToWorld(const glm::vec3& point) const {
  return transform_ * point;
}

/*************************************************************************/
/*!
  \brief
  Converts given point from world to local space
  \param point
  Point to convert given in Vec4
  \return
  Converted point
*/
/*************************************************************************/
glm::vec4 Transform::getPointWorldToLocal(const glm::vec4& point) const {
  glm::mat4 inverseMat = inverse(glTransform_);
  return inverseMat * point;
}

/*************************************************************************/
/*!
  \brief
  Converts given point from local to world space
  \param point
  Point to convert given in Vec4
  \return
  Converted point
*/
/*************************************************************************/
glm::vec4 Transform::getPointLocalToWorld(const glm::vec4& point) const {
  return glTransform_ * point;
}

//*---------------------------------------------------------------------------
//* Methods
//*---------------------------------------------------------------------------
/*************************************************************************/
/*!
  \brief
  Fills the GL matrix using the transform_ matrix
*/
/*************************************************************************/
void Transform::fillGlMatrix(void) {
  glTransform_[0][0] = transform_[0][0];
  glTransform_[1][0] = transform_[1][0];
  glTransform_[3][0] = transform_[2][0];

  glTransform_[0][1] = transform_[0][1];
  glTransform_[1][1] = transform_[1][1];
  glTransform_[3][1] = transform_[2][1];

  glTransform_[0][3] = transform_[0][2];
  glTransform_[1][3] = transform_[1][2];
  glTransform_[3][3] = transform_[2][2];
}

///*************************************************************************/
///*!
//  \brief
//  Returns the type of the component (Transform)
//*/
///*************************************************************************/
//Component_::TypeEnum Transform::GetComponentType() const { 
//  return Component_::TypeEnum::Transform;
//} 

/*************************************************************************/
/*!
  \brief
  Creates a copy of the component
*/
/*************************************************************************/
Component* Transform::Clone_() const {
  return new Transform(*this);
}