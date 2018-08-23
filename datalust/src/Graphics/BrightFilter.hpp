#pragma once
#include "Graphics/PostProcessing.hpp"

namespace Graphics
{
  class BrightFilter : public PostProcess
  {
  public:
    BrightFilter(float threshold = 0.7f);

    virtual ~BrightFilter();

    virtual void Composite(FrameBuffer & source, FrameBuffer & dest, BlendMode = BlendMode::None);

    void setThreshold(float thresh);

  private:
    float threshold_;

    static Shader& BrightShader();
  };
} // namespace Graphics