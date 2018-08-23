/**
  \file   Background.hpp
  \author Samuel Cook

  \brief Components that represent mostly static environments.

   

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _BACKGROUND_HPP_
#define _BACKGROUND_HPP_

#include <memory>
#include "GraphicsTypes.hpp"
#include "GraphicBase.hpp"
#include "GameObject/Component.h"
#include "GameObject/GameObject.h"

namespace Graphics {

  /**
    \brief Base class for all types of backgrounds.
  */
  template<typename T>
  class Background : public GraphicBase<T> {
    public:
      Background(float depth = 0.0f, float parallax_factor = 1.0f,
        float alpha = 1.0f, float r = 0.0f, float g = 0.0f,
        float b = 0.0f, float a = 0.0f,
        BlendMode mode = BlendMode::Mix) : 
          GraphicBase(
            Size{ 2,2 },
            nullptr,
            depth,
            alpha,
            Color{ r, g, b, a },
            mode
          ),
        parallax_factor_(parallax_factor) {}
      
      T& setParallaxFactor(float parallax_factor) { 
          parallax_factor_ = parallax_factor; return *static_cast<T*>(this); 
      }
      T& getParallaxFactor(float *parallax_factor) { if(parallax_factor) *parallax_factor = parallax_factor_; return *static_cast<T*>(this); }
	  T const & getParallaxFactor(float *parallax_factor) const { if (parallax_factor) *parallax_factor = parallax_factor_; return *static_cast<T const*>(this); }

    T const& draw()
    {
      Graphic::draw();
      return *this;
    }
    protected:
      /* TODO: Implement class */
      float parallax_factor_;
  };

  /**
    DEPRECATED
  */
  //class BackgroundTileMap : public Background<BackgroundTileMap> {
  //  public:
  //    BackgroundTileMap(const Tileset& tileset, float depth = 0.0f, float parallax_factor = 1.0f,
  //                    float alpha = 1.0f, float r = 0.0f, float g = 0.0f,
  //                    float b = 0.0f, float a = 0.0f,
  //                    BlendMode mode = BlendMode::Mix);
  //  
  //    BackgroundTileMap& loadTileSetFromFile(const char *tileset);
  //  
  //    BackgroundTileMap& loadTileMapFromFile(const char *tilemap);
  //    BackgroundTileMap& loadTileMapFromBuffer(unsigned short *tilemap);
  //  
  //    BackgroundTileMap& setTileSize(float w, float h);
  //    BackgroundTileMap& getTileSize(float *w, float *h);
  //  
  //    BackgroundTileMap& setTileMapSize(float cols, float rows);
  //    BackgroundTileMap& getTileMapSize(float *cols, float *rows);
  //  private:
  //    std::unique_ptr<Tileset> tile_set_;
  //    /* TODO: Implement class */
  //};

  /**
    \brief An image that covers the entire veiwport. May appear anywhere in the depth order.
  */
  class BackgroundImage : public Background<BackgroundImage>, public Component {
    public:
      /// Constructor
      BackgroundImage(GameObject& parent, const char *filename = nullptr, float depth = 0.0f,
                      float parallax_factor = 1.0f, float alpha = 1.0f,
                      float r = 0.0f, float g = 0.0f, float b = 0.0f,
                      float a = 0.0f, BlendMode mode = BlendMode::Mix);
      /**
        \brief Loads an image to use as the background.

        \param filename
          Name of the image to load.

        \return Method Chaining.
      */
      BackgroundImage& loadFromFile(const char *filename);
    
      /**
        \brief Sets how the background will tile.

        \param horizontal
          The horizontal repeat mode

        \param vertical
          The vertical repeat mode

        \return Method Chaining.
      */
      BackgroundImage& setRepeat(RepeatMode horizontal, RepeatMode vertical);

      /**
        \brief Gets how the background will tile.

        \param horizontal
          The horizontal repeat mode

        \param vertical
          The vertical repeat mode

        \return Method Chaining.
      */
      BackgroundImage& getRepeat(RepeatMode *horizontal,
                                 RepeatMode *vertical);

      BackgroundImage& calculateUVs();

      BackgroundImage const& draw();

      virtual Graphic::Type graphicType() const { return Graphic::Type::Background;  }
    private:
      Component* Clone_() const;
  };


}
#endif
