#include <algorithm>
#include "Graphics\Shader.hpp"
#include "Graphics\FrameBuffer.hpp"
#include "BrightFilter.hpp"

namespace Graphics
{
  namespace detail
  {
    constexpr float clamp(float input, float min = 0.0f, float max = 1.0f)
    {
      return std::min(std::max(min, input), max);
    }
  }

  BrightFilter::BrightFilter(float threshold) : threshold_(detail::clamp(threshold))
  {
  }

  BrightFilter::~BrightFilter()
  {
  }

  void BrightFilter::Composite(FrameBuffer & source, FrameBuffer & dest, BlendMode mode)
  {
    // get shaders
    Shader & brightShader = BrightShader();

    // configure shader
    brightShader.setUniform1f("threshold", threshold_);

    // draw from source to dest with given mode
    source.Draw(brightShader, dest, mode);
  }

  void BrightFilter::setThreshold(float thresh)
  {
    // clamp to [0.0f, 1.0f]
    threshold_ = detail::clamp(thresh);
  }

  Shader & BrightFilter::BrightShader()
  {
    static Shader brightShader{ "FBOHighlight" };
    return brightShader;
  }
}