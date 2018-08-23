#include <algorithm>
#include <cmath>
#include "Graphics/Shader.hpp"
#include "Blur.hpp"

namespace Graphics
{
  namespace detail
  {
    constexpr float pi = 3.14159265359f;
    constexpr float e = 2.718281828459f;
  }

Blur::Blur(int sigma, int taps) : 
  sigma_(sigma),
  taps_(taps),
  intermediate_(1280, 720, true)
{
}

int Blur::TAPS = 40;
int Blur::SIGMA = 20;

Blur::~Blur()
{
}

void Blur::Composite(FrameBuffer & source, FrameBuffer & dest, BlendMode mode)
{
  setBlurSettings(sigma_, taps_);

  std::vector<Shader> & shaders = gaussianShader();


  Shader & vertPass = shaders[Vert];

  vertPass.setUniform1i("height", source.getResolution().y);

  source.Draw(vertPass, intermediate_);

  Shader & horizPass = shaders[Horiz];

  horizPass.setUniform1i("width", source.getResolution().x);

  glViewport(0, 0, 1920, 1080);
  intermediate_.Draw(horizPass, dest, mode);
}


void Blur::setSigma(int sigma)
{
  sigma_ = sigma;
}

void Blur::setRadius(int taps)
{
  taps_ = std::min(std::abs(taps), 100);
}

void Blur::ReInit()
{
  intermediate_.ReInit();
  SIGMA = -1;
  TAPS = -1;
}

std::vector<Shader> & Blur::gaussianShader()
{
  static std::vector<Shader> shaders;
  static bool first = true;

  if (first)
  {
    static constexpr char * names[] = {
      "FBOGaussianVert", "FBOGaussianHoriz"
    };

    shaders.reserve(sizeof(names) / sizeof(names[0]));

    for (auto && name : names)
      shaders.emplace_back( name );

    first = false;
  }

  return shaders;
}

void Blur::setBlurSettings(int sigma, int taps)
{
  if (taps == TAPS && sigma == SIGMA)
    return;

  SIGMA = sigma;
  TAPS = taps;

  static std::vector<float> kernel;
  static std::vector<float> offsets;

    // calculate new kernel
  kernel.reserve(taps);

  // calculate new offsets
  offsets.reserve(taps);

  // https://en.wikipedia.org/wiki/Gaussian_blur
  const int half_taps = taps / 2;
  for (int i = 0; i != taps; i++)
  {
    const int offset = i - half_taps;
    kernel.emplace_back(
      (1.0f / (sigma * std::sqrt(2.0f * detail::pi))) *

      std::pow(detail::e, -0.5f * offset * offset / (sigma * sigma))
    );

    offsets.emplace_back(static_cast<float>(offset));
  }


  // normalize kernel
  
  float sum = 0;
  for (auto && component : kernel)
    sum += component;

  for (auto && component : kernel)
    component /= sum;
  

  // update shaders
  std::vector<Shader>& shaders = gaussianShader();
  for (auto && shader : shaders)
  {
    shader.setUniform1i("taps", taps);
    shader.setUniformArrayf("offset", taps, offsets.data());
    shader.setUniformArrayf("kernel", taps, kernel.data());
  }

  kernel.clear();
  offsets.clear();
}

}