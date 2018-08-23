/**
  \file   Texture.cpp
  \author Samuel Cook

  \brief Texture wrapper implementation



  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include <cstdlib>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include "Texture.hpp"
#include <string>
#include <Trace/Trace.hpp>

namespace Graphics {
  Texture::Texture( const char* filename ) : loaded_( false ) {
    if ( filename )
      filename_ = filename;
    loadFromFile( filename );
  }

  Texture::Texture( const Texture & o ) : loaded_( false ), filename_( o.filename_ ) {
    loadFromFile( filename_.c_str() );
  }

  Texture::Texture( Texture && o ) :
    tex_( std::move( o.tex_ ) ),
    width_( std::move( o.width_ ) ),
    height_( std::move( o.height_ ) ),
    loaded_( std::move( o.loaded_ ) ),
    filename_( std::move( o.filename_ ) ) {
    o.tex_ = 0;
  }

  Texture::~Texture() {
    clearTexture();
  }

  void Texture::loadFromFile( const char * filename ) {
    if ( filename && filename[0] ) {
      sf::Image img;
      if ( img.loadFromFile( filename ) ) {
        img.flipVertically(); // < fixes uv mapping issues
        width_ = img.getSize().x;
        height_ = img.getSize().y;
        if ( !tex_ )
          glGenTextures( 1, &tex_ );

        glBindTexture( GL_TEXTURE_2D, tex_ );

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr() );

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // these are the default values, no need to set explicitly
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        Color c{ 0.0, 0.0, 0.0, 0.0 };
        glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, ( float* )&c );


        glBindTexture( GL_TEXTURE_2D, 0 );

        loaded_ = true;
      } else { // file not loaded
        defaultTexture();
        engine.Find<Trace::TraceHandler>()->TraceMessage( TraceLevel::WARNING, "Failed to load texture %s.", filename );
      }
    } else { // requested "file" was empty string, clear the texture
      clearTexture();
      width_ = 0;
      height_ = 0;
      loaded_ = false;
    }

  }

  void Texture::bindTexture() const {
    glBindTexture( GL_TEXTURE_2D, tex_ );
  }

  Size Texture::getSize() const {
    return Size{ ( float )width_, ( float )height_ };
  }

  bool Texture::loaded() const {
    return loaded_;
  }

  void Texture::clearTexture() {
    if ( tex_ ) {
      glDeleteTextures( 1, &tex_ );
      tex_ = 0;
      loaded_ = false;
    }
  }
  void Texture::defaultTexture() {
    width_ = 128;
    height_ = 128;
    if ( !tex_ )
      glGenTextures( 1, &tex_ );

    glBindTexture( GL_TEXTURE_2D, tex_ );

    // generate a default error texture
    unsigned char img[128 * 128 * 4];
    for ( int i = 0; i < 128; ++i ) {
      for ( int j = 0; j < 128; ++j ) {
        if ( (j % 32 < 16  && i % 32 < 16) || (j % 32 >= 16  && i % 32 >= 16) ) {
          img[i * 512 + j * 4 + 0] = 0x7F;
          img[i * 512 + j * 4 + 1] = 0x00;
          img[i * 512 + j * 4 + 2] = 0xFF;
          img[i * 512 + j * 4 + 3] = 0xFF;
        } else {
          img[i * 512 + j * 4 + 0] = 0xFF;
          img[i * 512 + j * 4 + 1] = 0x00;
          img[i * 512 + j * 4 + 2] = 0x7F;
          img[i * 512 + j * 4 + 3] = 0xFF;
        }
      }
    }

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, img );

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // these are the default values, no need to set explicitly
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    Color c{ 0.0, 0.0, 0.0, 0.0 };
    glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, ( float* )&c );


    glBindTexture( GL_TEXTURE_2D, 0 );

    loaded_ = true;
  }
}