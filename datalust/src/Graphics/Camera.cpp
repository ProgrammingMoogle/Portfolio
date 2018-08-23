/**
  \file   Camera.cpp
  \author Samuel Cook

  \brief Component that represents a viewport.



  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include <SFML/Graphics/Glsl.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "Engine/Engine.h"
#include "Window.hpp"
#include "Graphics.hpp"
#include "GraphicsConstants.hpp"
#include "Transform/Transform.hpp"
#include "Camera.hpp"

namespace Graphics {

  const float pi = 4.0f * std::atan(1.0f);

  // rotates vector by -90 degreess about z-axis
  const glm::mat3 findU =
    glm::mat3(
      glm::vec3( std::cos(-0.5f * pi), std::sin(-0.5f * pi), 0),
      glm::vec3(-std::sin(-0.5f * pi), std::cos(-0.5f * pi), 0),
      glm::vec3(                    0,                    0, 1)
    );


  Camera::Camera(GameObject& parent) : Component(parent),
    up(0, 1, 0), right(1, 0, 0), center(0, 0, 1),
    width(5.0f * 720), height(5.0f * 720),
    target_height(5.0f * 720)
  {
    cameras.emplace(this);


    Manager * man = engine.Find<Manager>();
    if (man == nullptr) return;
    Window const* win = man->window();
    if (win == nullptr) return;

    switch (mode)
    {
    case AspectMode::Stretch:
      break;

    case AspectMode::Fit:
    {
      Fit(ASPECT);
      break;
    }

    }
  }

  Camera::Camera(Camera const & rhs) : Component(rhs),
    up(rhs.up), right(rhs.right), center(rhs.center),
    width(rhs.width), height(rhs.height),
    target_height(rhs.target_height)
  {
    cameras.emplace(this);
  }

  Camera::~Camera() {
    auto result = cameras.find(this);

    if (result != cameras.end())
      cameras.erase(result);

    deactivate();
  }
  Camera * Camera::getActiveCamera() {
    return active_cam_;
  }
  Camera& Camera::activate() {
    active_cam_ = this;
    return *this;
  }
  Camera& Camera::deactivate() {
    // if this is the active camera, set the active camera to null
    if (active_cam_ == this)
      active_cam_ = nullptr;
    return *this;
  }

  glm::vec3 Camera::MouseToWorld()
  {
    Window *win = engine.Find<Graphics::Manager>()->window();
    
    const glm::mat3 ToCamera(
      glm::vec3(width / win->width(),                       0, 0),
      glm::vec3(                   0, -height / win->height(), 0),
      glm::vec3(       -0.5f * width,           0.5f * height, 1)
    );

    return glm::mat3(right, up, center) * ToCamera * glm::vec3(win->GetMousePos(), 1);
  }

  void Camera::WindowResize(int x, int y)
  {
    switch (mode)
    {
      case AspectMode::Stretch:
        for (auto && camera : cameras)
          camera->Stretch();
        break;

      case AspectMode::Fit:
      {
        const float aspect = static_cast<float>(x) / static_cast<float>(y);
        for (auto && camera : cameras)
          camera->Fit(ASPECT);
        break;
      }

    }
  }

  glm::mat4 Camera::CameraToWorld() const
  {
    return glm::mat4(   
      glm::vec4(right,   0),
      glm::vec4(up,      0),
      glm::vec4(0, 0, 1, 0),
      glm::vec4(center , 1)
    );
  }

  glm::mat4 Camera::WorldToCamera() const
  {
    return glm::inverse(CameraToWorld());
  }

  glm::mat4 Camera::CameraToNDC() const
  {
    return glm::scale(glm::mat4(1), glm::vec3(2.0f / width, 2.0f / height, 1));
  }

  glm::mat4 Camera::NDCToCamera() const
  {
    return glm::scale(glm::mat4(1), glm::vec3(0.5f * width, 0.5f * height, 1));
  }

  void Camera::Stretch()
  {
    width = height = target_height;
  }

  void Camera::Fit(float aspect)
  {
    width  = target_height * aspect;
    height = target_height;
  }

  // DEPRECATED
  void Camera::MaintainAspect(bool targetAspectIsLesser, float target_aspect)
  {
    Fit(target_aspect);
  }

  Component* Camera::Clone_() const {
    return new Camera(*this);
  }


  glm::vec3 const & Camera::getCenter() const
  {
    return center;
  }

  void Camera::setCenter(glm::vec2 const & c)
  {
    center = glm::vec3(c, 1);
  }

  void Camera::setUp(glm::vec2 const & u)
  {
    up = glm::vec3(u, 0);
    right = findU * up;
  }

  glm::vec3 const & Camera::getUp() const
  {
    return up;
  }

  void Camera::setHeight(float h)
  {
    target_height = std::abs(h);

    switch (mode)
    {
    case AspectMode::Stretch:
      Stretch();
      break;

    case AspectMode::Fit:
    {
      Fit(ASPECT);
      break;
    }
    }
  }

  void Camera::SetAspectMode(AspectMode _mode)
  {
    const AspectMode lastMode = mode;

    mode = _mode;

    if (_mode == lastMode)
      return;

    Graphics::Manager * manager = engine.Find<Graphics::Manager>();

    if (manager == nullptr)
      return;

    Window * window = manager->window();

    if (window == nullptr)
      return;

    WindowResize(window->width(), window->height());
  }

  AspectMode  Camera::GetAspectMode()
  {
    return mode;
  }

  AspectMode Camera::mode = AspectMode::Fit;
  std::set<Camera *> Camera::cameras;
  Camera* Camera::active_cam_ = nullptr;
}
