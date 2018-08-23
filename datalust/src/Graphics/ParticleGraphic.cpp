/**
  \file   ParticleGraphic.cpp
  \author Samuel Cook

  \brief Particle graphic implementation

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Geometry.hpp"
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Renderer.hpp"

#if 0
namespace Graphics {
  ParticleGraphic::ParticleGraphic(std::vector<KPE::Particle> * parent) : 
    parent_(parent), transforms_(), toWorld(nullptr) {
    Renderer::getCurrentRenderer()->addGraphic(this);

    setBlendMode(Graphics::BlendMode::Additive);
    setBlendColor(Graphics::Color{ 1.0f, 0.25f, 0.0f, 1.0f });
    setAlpha(0.5f);
    setBlendAlpha(0.5f);
    setDepth(-100);
    setSize(Graphics::Size{ 40,40 });
  }

  void ParticleGraphic::setWorldTransform(glm::mat4 const* _toWorld)
  {
    toWorld = _toWorld;
  }

  void ParticleGraphic::unsetWorldTransform()
  {
    toWorld = nullptr;
  }

  void ParticleGraphic::draw()
  {
    if (parent_ == nullptr)
      return;


    std::vector<KPE::Particle> & parent = *parent_;
    transforms_.clear();
    if (transforms_.size() < parent.size())
      transforms_.reserve(parent.size());


    Size const& size = mesh_.getSize();


    for (auto && particle : parent)
    {
      glm::mat4 const& scale = glm::scale(glm::mat4(1), glm::vec3(size.x * particle.size_, size.y * particle.size_, 1));
      glm::mat4 const& rotate = glm::rotate(scale, particle.rotation_, glm::vec3(0, 0, 1));
      glm::mat4 const& translate = glm::translate(rotate, glm::vec3(particle.offset_.x, particle.offset_.y, 1));

      transforms_.emplace_back((toWorld == nullptr) ? translate : translate  * (*toWorld));
    }

    Renderer::getCurrentRenderer()->drawTris(
      Renderer::ArrayRenderParams(
        Geometry::Circle,
        tex_,
        blend_mode_,
        blend_color_,
        alpha_,
        TexCoord{ 0, 0 },
        transforms_,
        mesh_
      )
    );
  }
}
#endif