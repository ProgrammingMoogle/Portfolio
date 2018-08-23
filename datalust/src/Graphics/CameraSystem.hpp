#pragma once
#include <map>
#include <string>
#include <functional>
#include "Engine\System.h"

namespace Graphics {
  // fwd decls
  class Camera;
  typedef Camera * CameraPtr;
  typedef Camera const * CameraConstPtr;

  class CameraSystem : public System
  {
  public:
    void resize(unsigned w, unsigned h);
    float getAspect() const;

    void Register(std::string const& name, CameraPtr cam);
    void Unregister(std::string const& name);

    void setActiveCamera(std::string const& name);
    CameraPtr getActiveCamera(void);
    void deactivate(void);

    CameraPtr      GetCamera(std::string const& name);       // l-value
    CameraConstPtr GetCamera(std::string const& name) const; // r-value

  private:
    std::map<std::size_t, CameraPtr > cameras_;
    std::hash<std::string> hash_;

    std::size_t active_camera_;

    float aspect_ = 16.0f / 9.0f;

  }; // class CameraSystem

} // namespace Graphics