/**
  \file   Camera.hpp
  \author Samuel Cook

  \brief Component that represents a viewport.



  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <set>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "GameObject/Component.h"
#include "GameObject/GameObject.h"

namespace Graphics {

  enum class AspectMode
  {
    Stretch = 0,
    Fit
  };

  /**
    \brief Class that represents a camera.
  */
  class Camera : public Component {
    public:
      /// Constructor
      Camera(GameObject& parent);
      
      /// copy ctor
      Camera(Camera const& rhs);

      /// Destructor
      ~Camera();
      /**
        \brief Returns the currently active camera.

        \return The active camera.
      */
      static Camera * getActiveCamera();
      /**
        \brief Makes this camera the active one.

        \return Method Chaining.
      */
      Camera& activate();

      /**
        \brief Makes this camera not the active one.

        \return Method Chaining.
      */
      Camera& deactivate();
      
    glm::vec3 MouseToWorld();

    static void WindowResize(int x, int y);

    glm::mat4 CameraToWorld() const;
    glm::mat4 WorldToCamera() const;

    glm::mat4 CameraToNDC() const;
    glm::mat4 NDCToCamera() const;

    glm::vec3 const& getCenter() const;
    void setCenter(glm::vec2 const& c);

    void setUp(glm::vec2 const& u);

    glm::vec3 const& getUp() const;
    void setHeight(float h);

    static void SetAspectMode(AspectMode _mode);
    static AspectMode GetAspectMode();

    private:
      glm::vec3 up, right, center; // camera to world
      float     width, height;     // camera to NDC
      float     target_height;     // camera resizing is based on this target

      static Camera * active_cam_;
      /* Don't worry about it ;) */

      static std::set<Camera *> cameras;

      void Stretch();
      void Fit(float aspect);
      // DEPRECATED ( for now )
      void MaintainAspect(bool targetAspectIsLesser, float target_aspect);

    private:
      Component* Clone_() const;

      static AspectMode mode;
  };

}

#endif