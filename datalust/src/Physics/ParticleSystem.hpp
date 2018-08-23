// ParticleSystem.hpp
// Patrick Cook
// Declares a component containing particle rendering information

#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <vector>
#include <glm\mat4x4.hpp>
#include "GameObject\Component.h"
#include "Graphics\GraphicsTypes.hpp"
#include "Graphics\GraphicBase.hpp"
#include "Graphics\Geometry.hpp"
#include "Physics\Particle.hpp"

namespace KPE {
  class ParticleSystem : public Graphics::GraphicBase<ParticleSystem>, public Component
  {
  public:
    enum class SystemSpace
    {
      World,
      Object
    };

    // ctors and dtors
    ParticleSystem(GameObject& parent);

    virtual ~ParticleSystem(void);

    virtual Graphic::Type graphicType() const { return Graphic::Type::Particles; }

    virtual Component * Clone_() const;

    virtual void Update(float dt);

    void draw();

    void setSystemSpace(SystemSpace space);

    void setGeometry(Graphics::Geometry geo);

    void loadTexture(std::string const& filename);

    SystemSpace getSpace() const;

  private:
    glm::mat4 calcTransform(Particle const& particle
    ) const;

    Graphics::Geometry geo_ = Graphics::Geometry::Square;
    SystemSpace space_ = SystemSpace::World;
    std::vector<glm::mat4> transforms_;

    
  };
}


#endif // PARTICLE_SYSTEM_HPP