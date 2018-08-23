#include <glm\gtc\matrix_transform.hpp>
#include "Transform\Transform.hpp"
#include "GameObject\Component.h"
#include "GameObject\GameObject.h"
#include "Graphics\Renderer.hpp"
#include "Physics\ParticleSystem.hpp"
#include "Physics\ParticleEmitter.hpp"

namespace KPE
{

  ParticleSystem::ParticleSystem(GameObject& parent) : Graphics::GraphicBase<ParticleSystem>(), 
    Component(parent),
    space_(SystemSpace::World)
  {

  }

  ParticleSystem::~ParticleSystem(void)
  {

  }

  Component * ParticleSystem::Clone_() const
  {
    return new ParticleSystem(*this);
  }

  void ParticleSystem::Update(float)
  {
  }

  void ParticleSystem::setSystemSpace(SystemSpace space)
  {
    space_ = space;
  }

  void ParticleSystem::setGeometry(Graphics::Geometry geo)
  {
    geo_ = geo;
  }

  glm::mat4 ParticleSystem::calcTransform(Particle const& particle) const
  {
    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(particle.offset_, 1));
    model = glm::rotate   (model, particle.rotation_, glm::vec3(0, 0, 1));
    model = glm::scale    (model, glm::vec3(particle.size_, particle.size_, 1));
    return model;
   // return glm::translate(glm::mat4(1), ) * 
    //  glm::rotate(glm::mat4(1), particle.rotation_, glm::vec3(0, 0, 1)) * 
     // glm::scale(glm::mat4(1), glm::vec3(size.x * particle.size_, size.y * particle.size_, 1));
  }

  void ParticleSystem::draw()
  {
    Graphics::Renderer * render = Graphics::Renderer::getCurrentRenderer();
    if (render == nullptr)
      return;

    // calculate transforms
    ParticleEmitter * emitter = GetParent().Find<ParticleEmitter>();
    if (emitter == nullptr)
      return;

    transforms_.clear();
    auto && particles = emitter->GetParticles();
    if (transforms_.size() < particles.size())
      transforms_.reserve(particles.size());

    Transform * transform = GetParent().Find<Transform>();
    glm::mat4 World2Screen = render->getProj() * render->getView();

    if (transform != nullptr && SystemSpace::Object == space_)
    {
      transform->Update(0);
      glm::mat4 toworld = glm::translate(glm::mat4(1), glm::vec3(transform->getPosition(), 0));

      for (auto && particle : particles)
        transforms_.emplace_back(World2Screen * toworld * calcTransform(particle));
    }
    else
    {
      for (auto && particle : particles)
        transforms_.emplace_back(World2Screen * calcTransform(particle));
    }

    render->drawTris(
      Graphics::Renderer::ArrayRenderParams
      (
        geo_,
        tex_,
        blend_mode_,
        blend_color_,
        alpha_,
        Graphics::TexCoord(0, 0),
        std::move(transforms_),
        mesh_
      )
    );
  }

  ParticleSystem::SystemSpace ParticleSystem::getSpace() const {
    return space_;
  }

  void ParticleSystem::loadTexture(std::string const& filename)
  {
    tex_.loadFromFile(filename.c_str());
  }


}