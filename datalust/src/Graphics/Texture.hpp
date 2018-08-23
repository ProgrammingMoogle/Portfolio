/**
  \file   Texture.hpp
  \author Samuel Cook

  \brief Encapsulates an OpenGL Texture.

  

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include <string>
#include "GraphicsTypes.hpp"

typedef unsigned int GLuint;

namespace Graphics {
  class Texture {
    public:
      Texture(const char * filename);
	    Texture(const Texture& o);
	    Texture(Texture&& o);
      ~Texture();
      void loadFromFile(const char * filename);
      void bindTexture() const;
      Size getSize() const;
      bool loaded() const;
    private:
      void clearTexture();
      void defaultTexture();
      GLuint tex_ = 0;
      size_t width_ = 0;
      size_t height_ = 0;
      bool loaded_ = false;
	  std::string filename_;
  };
}

#endif