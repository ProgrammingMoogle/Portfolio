#include <glm/gtc/type_ptr.hpp>
#include <limits>
#include "Bloom.hpp"
#include "FrameBuffer.hpp"
#include "Shader.hpp"

namespace Graphics {


  namespace detail {
    constexpr float kernel[] = {
      0.035596f, 0.036634f, 0.037609f, 0.038512f, 0.039339f, 0.040084f, 0.04074f, 0.041304f, 0.041771f, 0.042138f, 0.042402f, 0.042562f, 0.042615f, 0.042562f, 0.042402f, 0.042138f, 0.041771f, 0.041304f, 0.04074f, 0.040084f, 0.039339f, 0.038512f, 0.037609f, 0.036634f, 0.035596f
    };
    constexpr float offsets[] =
    {
      -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 
      0,
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
    };
  }

  Bloom::Bloom(int resX, int resY, size_t _passes) : 
    hPassB_(createBuffers(resX, resY, _passes)),
    vPassB_(createBuffers(resX, resY, _passes)),
    threshold(0.7f),
    radius(glm::vec2(1.0f, 1.0f)),
    passes(_passes)
  {
  }

  Bloom::~Bloom()
  {
  }

  void Bloom::Composite(FrameBuffer & source, GLuint dest)
  {
    for (auto && buffer : hPassB_)
      buffer.Clear(Color{ 0,0,0,1 });
    for (auto && buffer : vPassB_)
      buffer.Clear(Color{ 0,0,0,1 });

    Shaders& shaders_ = shaders();

    Shader & highlight = shaders_[BloomShader::Highlight];
    Shader & vGauss    = shaders_[BloomShader::GaussianV];
    Shader & hGauss    = shaders_[BloomShader::GaussianH];

    // Bright Pass
    // hPassB will contain bright pixels in source
    for (auto && buffer : hPassB_)
    {
      glViewport(0, 0, buffer.getResolution().x, buffer.getResolution().y);
      source.Draw(highlight, buffer);
    }

    // V gauss pass
    // vPassB will contain vertically blurred pixels from the Bright Pass
    for (
      auto   src = hPassB_.begin(), 
             dst = vPassB_.begin(), 
             end = hPassB_.end();

      src != end;

      ++src, ++dst
      )
    {
      glViewport(0, 0, dst->getResolution().x, dst->getResolution().y);
      src->Draw(vGauss, *dst);
    }
    
    // H gauss pass
    // HPassB will contain horizontally blurred pixels from the bright Pass
    for (auto && buffer : hPassB_)
    {
      glViewport(0, 0, buffer.getResolution().x, buffer.getResolution().y);
      buffer.Draw(hGauss, buffer);
    }

    glViewport(0, 0, 1920, 1080);

    // composite
    // the results from the vertical and horizontal passes will be added into the destination buffer
    for (
      auto   src0 = hPassB_.begin(),
      src1 = vPassB_.begin(),
      end = hPassB_.end();

      src0 != end;

      ++src0, ++src1
      )
    {
      src0->Combine(*src1, dest, BlendMode::Additive, 10.0f, 10.0f);
    }

  }

  void Bloom::setThreshold(float _thresh)
  {
    threshold = _thresh;
  }

  void Bloom::setRadius(glm::vec2 const& _radius)
  {
    radius = _radius;
  }

  std::vector<FrameBuffer> Bloom::createBuffers(int resX, int resY, size_t passes)
  {
    static std::vector<FrameBuffer> buffers;

    buffers.reserve(passes);

    for (size_t i = 0; i != passes; ++i, resX /= 2, resY /= 2)
      buffers.emplace_back(resX, resY);

    return std::move(buffers);
  }

  std::vector<Shader> & Bloom::shaders()
  {
    static bool firstTime = true;
    static std::vector<Shader> shaders;

    if (firstTime)
    {
      static constexpr char * shaderName[] = { "FBOHighlight", "FBOGaussianVert", "FBOGaussianHoriz" };
      
      shaders.reserve(sizeof(shaderName) / sizeof(shaderName[0]));

      for (auto && name : shaderName)
        shaders.emplace_back(name);

      constexpr int size = sizeof(detail::kernel) / sizeof(detail::kernel[0]);

      shaders[BloomShader::GaussianH].setUniform1i("taps", size);
      shaders[BloomShader::GaussianV].setUniform1i("taps", size);

      shaders[BloomShader::GaussianH].setUniformArrayf("kernel", size, detail::kernel);
      shaders[BloomShader::GaussianV].setUniformArrayf("kernel", size, detail::kernel);

      shaders[BloomShader::GaussianH].setUniformArrayf("offset", size, detail::offsets);
      shaders[BloomShader::GaussianV].setUniformArrayf("offset", size, detail::offsets);

      firstTime = false;
    }

    return shaders;
  }

}