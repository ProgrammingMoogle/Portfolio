/**
  \file   GraphicBase.cpp
  \author Samuel Cook

  \brief Base class for all rendered objects.

  Defines the implementation of the common interface of all visible objects.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "GraphicBase.hpp"
#include "Graphics.hpp" // renderer.hpp
#include "Renderer.hpp"
#include <GameObject/GameObject.h>

#include <stdexcept>

namespace Graphics {
  namespace {
    constexpr TexCoord tex_coord[4]{
      {0, 1},
      {1, 1},
      {1, 0},
      {0, 0}
    };
  }

  // compares two graphics based on depth and then by type
  bool GraphicDepthCompare::operator()(Graphic * const lhs, Graphic * const rhs) const {
    float lhsd, rhsd;
    lhs->getDepth(&lhsd);
    rhs->getDepth(&rhsd);
    return (lhsd > rhsd);
  }

  Graphic::Graphic(const Graphics::Size& size, const char* filename, float depth, float alpha, const Color& color, BlendMode mode) :
    depth_(depth),
    alpha_(alpha),
    blend_color_(color),
    blend_mode_(mode),
    mesh_(tex_coord, size.x, size.y),
    tex_(filename),
    uv_offset_{0.0f,0.0f} 
  {
    // add graphic to graphic manager
    owner_ = Renderer::getCurrentRenderer();
    if(owner_) {
      owner_->addGraphic(this);
    } else {
      throw std::runtime_error("Cannot create Graphics if no Renderer is active. Please activate a renderer with Renderer::Activate()");
    }
  }

  Graphic::Graphic(const Graphic & o) :
	depth_(o.depth_),
	alpha_(o.alpha_),
	blend_color_(o.blend_color_),
	blend_mode_(o.blend_mode_),
	mesh_(o.mesh_),
	tex_(o.tex_),
	uv_offset_(o.uv_offset_),
	owner_(o.owner_)
  {
	  if (owner_) {
		  owner_->addGraphic(this);
	  }
	  else {
		  throw std::runtime_error("Cannot create Graphics if no Renderer is active. Please activate a renderer with Renderer::Activate()");
	  }
  }

  Graphic::Graphic(Graphic && o) :
	  depth_(std::move(o.depth_)),
	  alpha_(std::move(o.alpha_)),
	  blend_color_(std::move(o.blend_color_)),
	  blend_mode_(std::move(o.blend_mode_)),
	  mesh_(std::move(o.mesh_)),
	  tex_(std::move(o.tex_)),
	  uv_offset_(std::move(o.uv_offset_)),
	  owner_(std::move(o.owner_))
  {
	  owner_->removeGraphic(&o);
	  owner_->addGraphic(this);
  }

  Graphic::~Graphic() {
    // remove graphic from graphic manager
    if(owner_) {
      owner_->removeGraphic(this);
    }
  }

  Graphic& Graphic::setDepth(float depth) {
    depth_ = depth;
    return *this;
  }

  const Graphic& Graphic::getDepth(float *depth) const {
    if(depth) {
      *depth = depth_;
    }
    return *this;
  }

  Graphic& Graphic::getDepth(float *depth) {
    if(depth) {
      *depth = depth_;
    }
    return *this;
  }

  Graphic& Graphic::setAlpha(float alpha) {
    alpha_ = alpha;
    return *this;
  }
  Graphic& Graphic::getAlpha(float *alpha) {
    if(alpha) {
      *alpha = alpha_;
    }
    return *this;
  }
  const Graphic& Graphic::getAlpha( float *alpha ) const {
      if ( alpha ) {
          *alpha = alpha_;
      }
      return *this;
  }

  Graphic& Graphic::setBlendColor(const Color& color) {
    blend_color_ = color;
    blend_color_.a = (1.0f > color.a) ? ((color.a < 0.0f) ? (0.0f) : (color.a)) : (1.0f);
    return *this;
  }
  Graphic& Graphic::getBlendColor(Color *color) {
    if(color) {
      *color = blend_color_;
    }
    return *this;
  }

  const Graphic& Graphic::getBlendColor( Color *color ) const {
      if ( color ) {
          *color = blend_color_;
      }
      return *this;
  }

  Graphic& Graphic::setBlendAlpha(float alpha) {
    blend_color_.a = (1.0f > alpha) ? ((alpha < 0.0f) ? (0.0f) : (alpha)) : (1.0f);
    return *this;
  }

  Graphic& Graphic::getBlendAlpha(float *alpha) {
    if(alpha) {
      *alpha = blend_color_.a;
    }
    return *this;
  }

  const Graphic& Graphic::getBlendAlpha( float *alpha ) const {
      if ( alpha ) {
          *alpha = blend_color_.a;
      }
      return *this;
  }

  Graphic& Graphic::setBlendMode(BlendMode mode) {
    blend_mode_ = mode;
    return *this;
  }

  Graphic& Graphic::getBlendMode(BlendMode *mode) {
    if(mode) {
      *mode = blend_mode_;
    }
    return *this;
  }

  const Graphic& Graphic::getBlendMode( BlendMode *mode ) const {
      if ( mode ) {
          *mode = blend_mode_;
      }
      return *this;
  }

  Graphic& Graphic::setSize(const Graphics::Size& size) {
    mesh_.setSize(size);
    return *this;
  }

  Graphic& Graphic::getSize( Graphics::Size* size ) {
      if ( size ) {
          mesh_.getSize( size );
      }
      return *this;
  }

  const Graphic& Graphic::getSize( Graphics::Size* size ) const {
      if ( size ) {
          mesh_.getSize( size );
      }
      return *this;
  }

  Graphic& Graphic::draw() {
    // TODO: factor out code from Renderer::Draw() into here
    return *this;
  }

  Graphic & Graphic::ReInit()
  {
    getMesh().ReInit();
    return *this;
  }

  SquareTexturedMesh& Graphic::getMesh() {
    return mesh_;
  }

  Texture& Graphic::getTexture() {
    return tex_;
  }
}