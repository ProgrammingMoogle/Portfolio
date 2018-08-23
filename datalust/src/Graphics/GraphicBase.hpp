/**
  \file   GraphicBase.hpp
  \author Samuel Cook

  \brief Base class for all rendered objects.

  Defines the component that represents the common interface of all visible objects.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _GRAPHIC_BASE_HPP_
#define _GRAPHIC_BASE_HPP_

#include "GraphicsTypes.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

namespace Graphics {
  class Renderer;
  /**
    \brief Polymorphic base class for all graphics.
  */
  class Graphic {
    public:
      enum class Type {
        Sprite,
        Background,
        UISprite,
        CircleSprite,
        Particles,
        Debug
      };

      /**
        Constructor
      */
      Graphic(const Size& size = Size{1,1}, const char* filename = nullptr, float depth = 0.0f, float alpha = 1.0f, const Color& color = Color{1,1,1,1}, BlendMode mode = BlendMode::Mix);

	  Graphic(const Graphic& o);

	  Graphic(Graphic&& o);
      /**
        Virtual destructor
      */
      virtual ~Graphic();

      /**
        Sets the z-order depth of the graphic. Depths towards -infinity will be drawn infront of
        depths towards +infinity.

        \param depth
          The value to set the depth to.
      */
      Graphic& setDepth(float depth);
      /**
        Gets the z-order depth of the graphic. Depths towards -infinity will be drawn infront of
        depths towards +infinity.

        \param depth
          The address to place the current depth value at. Pass nullptr to ignore this value.
      */
      const Graphic& getDepth(float *depth) const;
      Graphic& getDepth(float *depth);

      /**
        Sets the alpha transparency of the graphic. Alpha values towards zero make the graphic more
        transparent, values towards 1 make the graphic more opaque. Values outside the range [0, 1]
        are clamped.

        \param alpha
          The value to set the alpha to.
      */
      Graphic& setAlpha(float alpha);
      /**
        Gets the alpha transparency of the graphic. Alpha values towards zero make the graphic more
        transparent, values towards 1 make the graphic more opaque. Values will be within the range
        [0, 1].

        \param alpha
          The address to place the current alpha value at. Pass nullptr to ignore this value.
      */
      Graphic& getAlpha(float *alpha);
      const Graphic& getAlpha( float *alpha ) const;

      /**
        Sets the blend color of the graphic.

        \param color
          The color to set the blend color to.
      */
      Graphic& setBlendColor(const Color& color);
      /**
        Gets the blend color of the graphic.

        \param color
          The address to place the current blend color at. Pass nullptr to ignore this value.
      */
      Graphic& getBlendColor(Color *color);
      const Graphic& getBlendColor( Color *color ) const;

      /**
        Sets the blend alpha of the graphic. Alpha values towards zero show more of the texture
        color, values towards 1 show more of the blend color. Values outside the range [0, 1]
        are clamped.

        \param alpha
          The value to set the blend alpha to.
      */
      Graphic& setBlendAlpha(float alpha);
      /**
        Gets the blend alpha of the graphic. Alpha values towards zero show more of the texture
        color, values towards 1 show more of the blend color. Values will be within the range
        [0, 1].

        \param alpha
          The address to place the current blend alpha at. Pass nullptr to ignore this value.
      */
      Graphic& getBlendAlpha(float *alpha);
      const Graphic& getBlendAlpha( float *alpha ) const;

      /**
        Sets the blend mode of the graphic. See the description of Graphic::BlendMode.

        \param mode
          The value to set the blend mode to.
      */
      Graphic& setBlendMode(BlendMode mode);
      /**
        Gets the blend mode of the graphic. See the description of Graphic::BlendMode.

        \param mode
          The address to place the current blend mode at. Pass nullptr to ignore this value.
      */
      Graphic& getBlendMode(BlendMode *mode);  
      const Graphic& getBlendMode( BlendMode *mode ) const;

      Graphic& setSize(const Graphics::Size& size);

      Graphic& getSize( Graphics::Size* size );

      const Graphic& getSize( Graphics::Size* size ) const;
      
      /**
       Draws the graphic.
       */
      Graphic& draw();
      
      virtual Graphic::Type graphicType() const = 0;

      Graphic& ReInit();

    protected:
      
      SquareTexturedMesh& getMesh();

      Texture& getTexture();

	  void setUVOffset(const TexCoord& offset) { uv_offset_ = offset; };

    const Renderer * const owner() const { return owner_; }

    protected:
      float depth_;
      float alpha_;
      float bloom_ = 1.0f;
      Color blend_color_;
      BlendMode blend_mode_;
      SquareTexturedMesh mesh_;
      Texture tex_;
      TexCoord uv_offset_;
      Renderer * owner_;

    private:
      friend class Renderer;
  };

  class GraphicDepthCompare {
  public:
    bool operator()(Graphic* const lhs, Graphic* const rhs) const;
  };

  /**
    \brief CRTP base class for all graphics.
  */
  template <typename T>
  class GraphicBase: public Graphic {
    public:
      /**
        Constructor

        \param depth
          The initial z-order depth

        \param alpha
          The initial alpha transparency value.

        \param color
          The initial blend color.

        \param mode
          The initial blend mode.
      */
      GraphicBase(const Size& size = Size{1,1}, const char* filename = nullptr, float depth = 0.0f, float alpha = 1.0f, Color color = Color{0}, BlendMode mode = BlendMode::Mix):
         Graphic(size, filename, depth, alpha, color, mode) {}

      /**
        Sets the z-order depth of the graphic. Depths towards -infinity will be drawn infront of
        depths towards +infinity.

        \param depth
          The value to set the depth to.

        \return
          Reference for method chaining.
      */
      T& setDepth(float depth) {
        Graphic::setDepth(depth);
        return *static_cast<T*>(this);
      }
      /**
        Gets the z-order depth of the graphic. Depths towards -infinity will be drawn infront of
        depths towards +infinity.

        \param depth
          The address to place the current depth value at. Pass nullptr to ignore this value.

        \return
          Reference for method chaining.
      */
      T& getDepth(float *depth) {
        Graphic::getDepth(depth);
        return *static_cast<T*>(this);
      }
      const T& getDepth( float *depth ) const {
          Graphic::getDepth( depth );
          return *static_cast<const T*>( this );
      }

      /**
        Sets the alpha transparency of the graphic. Alpha values towards zero make the graphic more
        transparent, values towards 1 make the graphic more opaque. Values outside the range [0, 1]
        are clamped.

        \param alpha
          The value to set the alpha to.

        \return
          Reference for method chaining.
      */
      T& setAlpha(float alpha) {
        Graphic::setAlpha(alpha);
        return *static_cast<T*>(this);
      }
      /**
        Gets the alpha transparency of the graphic. Alpha values towards zero make the graphic more
        transparent, values towards 1 make the graphic more opaque. Values will be within the range
        [0, 1].

        \param alpha
          The address to place the current alpha value at. Pass nullptr to ignore this value.

        \return
          Reference for method chaining.
      */
      T& getAlpha(float *alpha) {
        Graphic::getAlpha(alpha);
        return *static_cast<T*>(this);
      }
      const T& getAlpha( float *alpha ) const{
          Graphic::getAlpha( alpha );
          return *static_cast<const T*>( this );
      }

      /**
        Sets the blend color of the graphic.

        \param color
          The color to set the blend color to.

        \return
          Reference for method chaining.
      */
      T& setBlendColor(const Color& color) {
        Graphic::setBlendColor(color);
        return *static_cast<T*>(this);
      }
      /**
        Gets the blend color of the graphic.

        \param color
          The address to place the current blend color at. Pass nullptr to ignore this value.

        \return
          Reference for method chaining.
      */
      T& getBlendColor(Color *color) {
        Graphic::getBlendColor(color);
        return *static_cast<T*>(this);
      }
      const T& getBlendColor( Color *color ) const {
          Graphic::getBlendColor( color );
          return *static_cast<const T*>( this );
      }

      /**
        Sets the blend alpha of the graphic. Alpha values towards zero show more of the texture
        color, values towards 1 show more of the blend color. Values outside the range [0, 1]
        are clamped.

        \param alpha
          The value to set the blend alpha to.

        \return
          Reference for method chaining.
      */
      T& setBlendAlpha(float alpha) {
        Graphic::setBlendAlpha(alpha);
        return *static_cast<T*>(this);
      }
      /**
        Gets the blend alpha of the graphic. Alpha values towards zero show more of the texture
        color, values towards 1 show more of the blend color. Values will be within the range
        [0, 1].

        \param alpha
          The address to place the current blend alpha at. Pass nullptr to ignore this value.

        \return
          Reference for method chaining.
      */
      T& getBlendAlpha(float *alpha) {
        Graphic::getBlendAlpha(alpha);
        return *static_cast<T*>(this);
      }
      const T& getBlendAlpha( float *alpha ) const {
          Graphic::getBlendAlpha( alpha );
          return *static_cast<const T*>( this );
      }

      /**
        Sets the blend mode of the graphic. See the description of Graphic::BlendMode.

        \param mode
          The value to set the blend mode to.

        \return
          Reference for method chaining.
      */
      T& setBlendMode(BlendMode mode) {
        Graphic::setBlendMode(mode);
        return *static_cast<T*>(this);
      }
      /**
        Gets the blend mode of the graphic. See the description of Graphic::BlendMode.

        \param mode
          The address to place the current blend mode at. Pass nullptr to ignore this value.

        \return
          Reference for method chaining.
      */
      T& getBlendMode(BlendMode *mode) {
        Graphic::getBlendMode(mode);
        return *static_cast<T*>(this);
      }
      const T& getBlendMode( BlendMode *mode ) const{
          Graphic::getBlendMode( mode );
          return *static_cast<const T*>( this );
      }

      T& setSize(const Graphics::Size& size) {
        Graphic::setSize(size);
        return *static_cast<T*>(this);
      }

      T& getSize(Graphics::Size* size ) {
          Graphic::getSize( size );
          return *static_cast<T*>( this );
      }
      const T& getSize( Graphics::Size* size ) const {
          Graphic::getSize( size );
          return *static_cast<const T*>( this );
      }

      /**
        Draws the graphic.

        \return
          Reference for method chaining.
      */
      T& draw() {
        Graphic::draw();
        return *static_cast<T*>(this);
      }

    private:
      /* nothing */
  };
}

#endif