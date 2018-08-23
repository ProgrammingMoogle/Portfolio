/**
  \file   Sprite.cpp
  \author Samuel Cook

  \brief A component that draws to the screen.

  A sprite is a component representing an animated image that can be freely positioned on the
  screen.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include <glm/gtc/matrix_transform.hpp>
#include "Transform\Transform.hpp"
#include "Graphics\Renderer.hpp"
#include "Engine\Engine.h"
#include "Graphics\Graphics.hpp"
#include "Sprite.hpp"


namespace Graphics {
    Sprite::Sprite(GameObject& parent, const char *filename, const Size& size, float depth, size_t frame, float alpha,
      const Color& color, BlendMode mode) : 
      GraphicBase(size, filename, depth, alpha, color, mode), Component(parent), index_(frame), animation_controller_(*this) {
        cols_ = (size_t)(getTexture().getSize().x / size.x);
		    rows_ = (size_t)(getTexture().getSize().y / size.y);
        TexCoord tc[4] = {
          TexCoord{0, size.y / getTexture().getSize().y},
          TexCoord{size.x / getTexture().getSize().x, size.y / getTexture().getSize().y},
          TexCoord{size.x / getTexture().getSize().x, 0},
          TexCoord{0, 0}
        };
        getMesh().setUV(tc);
        setFrame(index_);
        if( filename && filename[0] ) {
            std::string fname( filename );
          fname.erase( fname.find( '.' ) );
          animation_controller_.load( ( fname + ".ani" ).c_str() );
        }
    }

    Sprite& Sprite::loadFromFile(const char *filename) {
      getTexture().loadFromFile(filename);
		if (filename[0] != 0) {
      // calculate the number of indicies in the sprite
			Size size;
	    getMesh().getSize(&size);
			cols_ = (size_t)(getTexture().getSize().x / size.x);
			rows_ = (size_t)(getTexture().getSize().y / size.y);
      // calculate the uv texture coordinates
			TexCoord tc[4] = {
        TexCoord{ 0, size.y / getTexture().getSize().y                                },
        TexCoord{ size.x / getTexture().getSize().x, size.y / getTexture().getSize().y},
        TexCoord{ size.x / getTexture().getSize().x, 0                                },
        TexCoord{ 0, 0                                                                }
			};
			getMesh().setUV(tc);
      setFrame(index_);

      // load the animations
      std::string fname( filename );
      fname.erase( fname.find( '.' ) );
      animation_controller_.load( ( fname + ".ani" ).c_str() );
		}
      return *this;
    }

    Sprite& Sprite::setSize(const Graphics::Size& size) {
      Graphic::setSize(size);
      if(getTexture().loaded()) {
      // calculate the number of indicies in the sprite
        cols_ = (size_t)(getTexture().getSize().x / size.x);
        rows_ = (size_t)(getTexture().getSize().y / size.y);
      // calculate the uv texture coordinates
        TexCoord tc[4] = {
          TexCoord{0, size.y / getTexture().getSize().y                                },
          TexCoord{size.x / getTexture().getSize().x, size.y / getTexture().getSize().y},
          TexCoord{size.x / getTexture().getSize().x, 0                                },
          TexCoord{0, 0                                                                }
        };
        getMesh().setUV(tc);
        TexCoord offset = {
          (float)(index_ % cols_) / cols_,
          1.0f - ((float)((index_ / cols_) + 1) / rows_)
        };
        setUVOffset(offset);
      }
      return *this;
    }
    Sprite & Sprite::play( const char * animation, float speed ) {
      animation_controller_.play( animation );
      return *this;
    }
    void Sprite::Update( float dt ) {
      animation_controller_.update( dt );
    }
    // Sprite& Sprite::changeSize(const Size& size) {
    //   cols_ = getTexture().getSize().x / size.x;
    // }
    Sprite& Sprite::setFrame(size_t index) {
      index_ = index;
	    if (getTexture().loaded()) {
		    TexCoord offset = {
			    (float)(index_ % cols_) / cols_,
          1.0f - ((float)((index_ / cols_) + 1) / rows_)
		    };
        setUVOffset(offset);
	    }
      return *this;
    }
    Sprite & Sprite::setOffset( float xoffset, float yoffset ) {
      xoff_ = xoffset;
      yoff_ = yoffset;
      return *this;
    }
    const Sprite & Sprite::getOffset( float * xoffset, float * yoffset ) const {
      if ( xoffset )
        *xoffset = xoff_;
      if ( yoffset )
        *yoffset = yoff_;
      return *this;
    }
    Sprite & Sprite::getOffset( float * xoffset, float * yoffset ) {
      const_cast< const Sprite* >( this )->getOffset( xoffset, yoffset );
      return *this;
    }
    Sprite& Sprite::getFrame(size_t *index) {
      if(index) {
        *index = index_;
      }
      return *this;
    }
	Sprite const & Sprite::getFrame(size_t *index) const {
		if (index) {
			*index = index_;
		}
		return *this;
	}
    Graphic::Type Sprite::graphicType() const {
      return Graphic::Type::Sprite;
    }
    // Sprite& Sprite::play(const char *animation, float speed) {
    //   // TODO: implement animations
    //   speed += (float)animation[0];
    //   return *this;
    // }
    // Sprite& Sprite::loop(const char *animation, float speed) {
    //   // TODO: implement animations
    //   speed += (float)animation[0];
    //   return *this;
    // }
    // Sprite& Sprite::stop() {
    //   // TODO: implement animations
    //   return *this;
    // }


    void Sprite::draw()
    {
      using Graphics::Renderer;
      Renderer * render = Renderer::getCurrentRenderer();
      GameObject & parent = GetParent();

      Transform *trans = parent.Find<Transform>();
      trans->Update(0);

      Graphics::Size size = (graphicType() != Type::UISprite) ? mesh_.getSize() : Size{ 16.0f/9.0f, 1 };
      
      // object space
      glm::mat4 toScreen = glm::scale(glm::mat4(1), glm::vec3(size.x, size.y, 1));
                toScreen = glm::translate(toScreen, glm::vec3(xoff_, yoff_, 0.0f));
      // world space
                toScreen = trans->getGLTransform() * toScreen;

      // camera space
      // screen space
      if (graphicType() != Type::UISprite)
          toScreen = render->getProj() * render->getView() * toScreen;
      else
      {
        Graphics::Window * win = engine.Find<Graphics::Manager>()->window();
        if (win)
          toScreen = glm::scale(toScreen, glm::vec3(2 * 9.0f / 16.0f, 2, 1));
      }

      render->drawTris(
        Renderer::RenderParams
        (
          geometry(),
          getTexture(),
          blend_mode_,
          blend_color_,
          alpha_,
          uv_offset_,
          toScreen,
          mesh_
        )
      );
    }
 

    Component* Sprite::Clone_() const {
      return new Sprite(*this);
    }
}