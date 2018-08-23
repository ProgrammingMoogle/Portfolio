/**
  \file   Graphics.hpp
  \author Samuel Cook

  \brief The graphics engine.

  Includes header files that comprise the graphics engine.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _GRAPHICS_HPP_
#define _GRAPHICS_HPP_

#include <cstdlib>

#include <GL/glew.h>

#include "Background.hpp"
#include "GraphicsTypes.hpp"
#include "Sprite.hpp"
#include "Mesh.hpp"
#include "GraphicBase.hpp"

#include <Engine/System.h>


/**
  \brief The Graphics engine.
*/
namespace Graphics {

  void testGraphics();

  class Renderer;
  class Window;

  class Manager : public System {
    public:
      Manager();
      Manager(Window * w);
      bool Init();
      void Update(float dt);
      void Shutdown();
      Window * window() { return window_; }
    private:
      Window * window_;
      Renderer * renderer_;
  };
}

#endif