#pragma once
#include <vector>
#include "Graphics/FrameBuffer.hpp"
#include "Graphics/PostProcessing.hpp"

namespace Graphics
{
  class Blur : public PostProcess
  {
  public:
    Blur(int sigma = 30, int taps = 45);

    virtual ~Blur();

    virtual void Composite(FrameBuffer & source, FrameBuffer & dest, BlendMode = BlendMode::None);

    
    void setSigma(int sigma);
    void setRadius(int taps);

    void ReInit();

  private:
    enum GaussShader
    {
      Vert = 0,
      Horiz = 1
    };

    int taps_;
    int sigma_;

    static int TAPS;
    static int SIGMA;

    std::vector<float> kernel_;
    std::vector<float> offsets_;

    static std::vector<Shader>& gaussianShader();
    static void setBlurSettings(int taps, int sigma);

    FrameBuffer intermediate_;
  };
} // namespace Graphics