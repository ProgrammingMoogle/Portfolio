#pragma once
#include "Graphics/GraphicBase.hpp"

namespace KPE {
  class Colliders;
}


namespace Graphics
{
  class ColliderSprite : public GraphicBase<ColliderSprite>
  {
  public:
   ColliderSprite(KPE::Colliders& parent);

   virtual void draw();

   Graphic::Type graphicType() const;

  private:
    KPE::Colliders * parent_;
  };
}