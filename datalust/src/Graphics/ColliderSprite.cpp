#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "HealthComponent.h"
#include "Behaviors/BulletBehavior.h"

#include "Graphics/Renderer.hpp"

#include "Physics/Colliders.hpp"

#include "Graphics/ColliderSprite.hpp"

namespace Graphics
{
  ColliderSprite::ColliderSprite(KPE::Colliders& parent) :
    GraphicBase{ Size(1,1), nullptr, 0.0f, 0.5f }, 
    parent_{&parent}
  {
  }

  static const Color colors[3] =
  {
    // boundbox
    Color{ 1.0f, 0.1f,  0.5f,  1.0f },
    // hitbox
    Color{ 1.0f, 0.25f, 0.25f, 1.0f },
    // hurtbox
    Color{ 0.5f, 1.0f,  0.5f,  1.0f }
  };

  void ColliderSprite::draw()
  {
    if (parent_ == nullptr || !parent_->GetParent().IsActive()) return;

    std::vector<KPE::BoundingBox *> const* boundaries = parent_->getBounds();
    if (boundaries == nullptr) return;

    BulletType color = NONE;
    HealthComponent * health = parent_->GetParent().Find<HealthComponent>();
    if (health && health->getType() != PLAYER)
    {
      color = health->GetColor();
    }
    else
    {
      BulletBehavior * bullet = parent_->GetParent().Find<BulletBehavior>();

      if (bullet)
      {
        color = bullet->GetBullet();
      }
    }

    if (color > COUNT || color < BLUE)
      return;


    const glm::mat4 toScreen = owner_->getProj() * owner_->getView();
    for (auto && box : *boundaries)
    {
      glm::mat4 toWorld = glm::translate(toScreen, glm::vec3(box->getPosition() + box->getOffset(), 0));
                toWorld = glm::scale(toWorld, glm::vec3(2.0f * box->getHalfsize(), 1));
      
      owner_->drawTris(
        Renderer::RenderParams{
          Geometry::Square,
          tex_,
          blend_mode_,
            color != NONE 
              ? BulletColors[color]
              : colors[box->getType()],
          alpha_,
          uv_offset_,
          toWorld,
          mesh_
        }
      );
    }
  }

  Graphic::Type ColliderSprite::graphicType() const
  {
    return Graphic::Type::Debug;
  }
}