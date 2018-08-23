// defines a step-by-step procedure to composite a shot onto the main frame buffer
#pragma once
#include "Graphics/GraphicsTypes.hpp"

namespace Graphics
{
  typedef unsigned GLuint;

  class FrameBuffer;
  class Shader;

  class PostProcess
  {
  public:
    virtual ~PostProcess() {};

    virtual void Composite(FrameBuffer & source, FrameBuffer & dest, BlendMode = BlendMode::None) = 0;

  };
}