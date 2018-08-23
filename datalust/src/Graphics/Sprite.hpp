/**
  \file   Sprite.hpp
  \author Samuel Cook

  \brief A component that draws to the screen.

  A sprite is a component representing an animated image that can be freely positioned on the
  screen.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _SPRITE_HPP_
#define _SPRITE_HPP_

#include "Geometry.hpp"
#include "AnimationController.hpp"
#include "GraphicBase.hpp"
#include <GameObject/GameObject.h>
#include <GameObject/Component.h>


namespace Graphics {
  class Sprite : public GraphicBase<Sprite>, public Component {
    public:

      Sprite(GameObject& parent, const char *filename = NULL, const Size& size = {1,1}, float depth = 0.0f, size_t frame = 0, float alpha = 1.0f,
        const Color& color = {0, 0, 0, 0}, BlendMode mode = BlendMode::Mix);
      
      virtual void draw();

      Sprite& loadFromFile(const char *filename);
      Sprite& setFrame(size_t index);
      Sprite& setOffset( float xoffset, float yoffset );
      const Sprite& getOffset( float* xoffset, float* yoffset ) const;
      Sprite& getOffset( float* xoffset, float* yoffset );
      Sprite& getFrame(size_t *index);
	    Sprite const& getFrame(size_t *index) const;
      Sprite& Sprite::setSize(const Graphics::Size& size);
      Sprite& play(const char *animation, float speed = 1.0f);

      void Update( float dt );

      // Sprite& animationCallback(void (*)(void));
      // Sprite& loop(const char *animation, float speed = 1.0f);
      // Sprite& stop();
      virtual Graphic::Type graphicType() const;
      virtual Geometry geometry() const { return Geometry::Square; }


    private:
;      size_t index_;
      size_t cols_, rows_;
      AnimationController animation_controller_;
      float xoff_ = 0.0f, yoff_ = 0.0f;
      
      Component* Clone_() const;
  };

  class UISprite : public Sprite {
    public:
      UISprite(GameObject& parent, const char *filename = NULL, const Size& size = { 1,1 }, float depth = 0.0f, size_t frame = 0, float alpha = 1.0f,
        const Color& color = { 0, 0, 0, 0 }, BlendMode mode = BlendMode::Mix) : Sprite(parent, filename, size, depth, frame, alpha, color, mode) {}
      virtual Graphic::Type graphicType() const { return Graphic::Type::UISprite; }
  };

  class CircleSprite : public Sprite {
    public:
      CircleSprite(GameObject& parent, const char *filename = NULL, const Size& size = { 1,1 }, float depth = 0.0f, size_t frame = 0, float alpha = 1.0f,
        const Color& color = { 0, 0, 0, 0 }, BlendMode mode = BlendMode::Mix) : Sprite(parent, filename, size, depth, frame, alpha, color, mode) {}
      virtual Geometry geometry() const { return Geometry::Circle; }
  };

  class UICircleSprite : public UISprite {
    public:
      UICircleSprite(GameObject& parent, const char *filename = NULL, const Size& size = { 1,1 }, float depth = 0.0f, size_t frame = 0, float alpha = 1.0f,
        const Color& color = { 0, 0, 0, 0 }, BlendMode mode = BlendMode::Mix) : UISprite(parent, filename, size, depth, frame, alpha, color, mode) {}
      virtual Geometry geometry() const { return Geometry::Circle; }
  };
}

#endif