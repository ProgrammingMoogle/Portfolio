// High-brightness pass -> blur pass -> composite
#pragma once

#include <vector>
#include "PostProcessing.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/FrameBuffer.hpp"

namespace Graphics
{
  class Bloom : public PostProcess
  {
  public:
    Bloom(int resX, int resY, size_t passes = 4);

    virtual ~Bloom();

    virtual void Composite(FrameBuffer & source, GLuint target);

    void setThreshold(float _thresh);
    void setRadius(glm::vec2 const& _radius);

  private:
    typedef std::vector<FrameBuffer> Buffers;
    typedef std::vector<Shader> Shaders;

    enum BloomShader
    {
      Highlight = 0,
      GaussianV = 1,
      GaussianH = 2
    };


    std::vector<FrameBuffer> hPassB_;
    std::vector<FrameBuffer> vPassB_;

    float threshold;
    glm::vec2 radius;
    const size_t passes;


    static Buffers createBuffers(int resX, int resY, size_t passes);
    static Shaders& shaders();
  };
}