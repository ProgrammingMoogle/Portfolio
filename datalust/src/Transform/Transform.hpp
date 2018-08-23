/******************************************************************************/
/*!
  \file   Transform.hpp
  \author Keonwoo Ryoo
  \par    email: keonwoo.ryoo\@digipen.edu
  \par    DigiPen login: keonwoo.ryoo
  \par    Last Update: 9/8/2017
  \date   9/8/2017
  \brief
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef _TRANSFORM_HPP_
#define _TRANSFORM_HPP_
#include <glm/glm.hpp>
#include <GameObject/Component.h>

//----------------------------------------------------------------------------
// Transform Declaration
//----------------------------------------------------------------------------
/**
 * \brief 
 */
class Transform : public Component {
  //!Position in World Coordinates
  glm::vec2 position_;

  //!Position of the Transform in the previous iteration
  glm::vec2 oldPosition_;

  //!Current angle of the transform
  float orientation_;

  /*!This matrix contains a rotation transformation matrix, which is the 
     current orientation of the matrix*/
  // cosx -sinx 0
  // sinx cosx  0
  //  0     0   1
  glm::mat3 orientationTransform_;

  /*!This matrix contains a scale transformation matrix, which is the current
     scale of matrix*/
  // x-scale    0         0
  //    0    y-scale      0
  //    0       0    global-scale
  glm::mat3 scale_;

  //!This matrix contains the total transformation in 3x3
  glm::mat3 transform_;

  //!This matrix contains the total transformation in 4x4
  glm::mat4 glTransform_;

  //!Set to true if the transform needs to be recalculated
  bool isDirty_;

public:
  Transform(GameObject& parent);
  virtual ~Transform(void);
  // Update
  void Update(float dt);

  //Accessors
    //Position
  void setPosition(const glm::vec2& pos);
  void setPosition(float x, float y);
  glm::vec2 const& getPosition(void) const;
  glm::vec2 getOldPosition(void) const;

    //Orientation
  void setOrientation(float angle);
  float getOrientation(void) const;
  glm::mat3 getOrientationTransform(void) const;

    //Scale
  
  void setXScale(float x);
  void setYScale(float y);
  float getXScale(void) const;
  float getYScale(void) const;
  glm::mat3 const& getScale() const;

    //Transform
  void getTransform(glm::mat3* transform) const;
  void getGLTransform(glm::mat4* transform) const;
  glm::mat3 const& getTransform(void) const;
  glm::mat4 const& getGLTransform(void) const;

    //Dirty
  bool isDirty(void) const;
  void setDirty(bool isDirty);
  
    //Point and Vector Conversions (World to Local) (Local to World)
  glm::vec2 getPointWorldToLocal(const glm::vec2& point) const;
  glm::vec2 getPointLocalToWorld(const glm::vec2& point) const;
  glm::vec3 getPointWorldToLocal(const glm::vec3& point) const;
  glm::vec3 getPointLocalToWorld(const glm::vec3& point) const;
  glm::vec4 getPointWorldToLocal(const glm::vec4& point) const;
  glm::vec4 getPointLocalToWorld(const glm::vec4& point) const;

  glm::vec2 getDirVecWorldToLocal(const glm::vec2& vec) const;
  glm::vec2 getDirVecLocalToWorld(const glm::vec2& vec) const;
  glm::vec3 getDirVecWorldToLocal(const glm::vec3& vec) const;
  glm::vec3 getDirVecLocalToWorld(const glm::vec3& vec) const;
  glm::vec4 getDirVecWorldToLocal(const glm::vec4& vec) const;
  glm::vec4 getDirVecLocalToWorld(const glm::vec4& vec) const;
  
    //Methods
  void fillGlMatrix(void);

 // virtual Component_::TypeEnum GetComponentType() const; 

private:
  virtual Component* Clone_() const;
};

#endif
