#include "CameraSystem.hpp"
#include "Camera.hpp"
namespace Graphics {

void CameraSystem::resize(unsigned w, unsigned h)
{
  aspect_ = static_cast<float>(w) / static_cast<float>(h);

  for (auto && named_camera : cameras_)
    named_camera.second->changeAspect(aspect_);
}

float CameraSystem::getAspect() const
{
  return aspect_;
}

void CameraSystem::Register(std::string const & name, CameraPtr cam)
{
  cameras_.try_emplace(hash_(name), cam);
}

void CameraSystem::Unregister(std::string const & name)
{
  auto result = cameras_.find(hash_(name));
  
  if (result != cameras_.end())
    cameras_.erase(result);
}

void CameraSystem::setActiveCamera(std::string const & name)
{
  active_camera_ = hash_(name);
}

CameraPtr CameraSystem::getActiveCamera(void)
{
  auto result = cameras_.find(active_camera_);

  return (result != cameras_.end())
    ? result->second
    : nullptr;
}

void CameraSystem::deactivate(void)
{
  active_camera_ = hash_("");
}

CameraPtr CameraSystem::GetCamera(std::string const & name)
{
  auto result = cameras_.find(hash_(name));

  return (result != cameras_.end())
    ? result->second
    : nullptr;
}

CameraConstPtr CameraSystem::GetCamera(std::string const & name) const
{
  auto result = cameras_.find(hash_(name));

  return (result != cameras_.end())
    ? result->second
    : nullptr;
}

} // namespace Graphics
