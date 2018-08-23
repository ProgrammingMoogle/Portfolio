#include <stdexcept>
#include "FrameBuffer.hpp"
#include "Graphics/Shader.hpp"
#include "GL\glew.h"

namespace std
{
  template<>
  void swap<Graphics::vec2i>(Graphics::vec2i & lhs, Graphics::vec2i & rhs)
  {
    std::swap(lhs.x, rhs.x);
    std::swap(lhs.y, rhs.y);
  }
}
namespace Graphics
{


  namespace {
    constexpr TexCoord tex_coord[4]{
      { 0, 1 },
      { 1, 1 },
      { 1, 0 },
      { 0, 0 },
    };
  }

  FrameBuffer::FrameBuffer(BackBufferSpecifier const&) :
    stdSquare_(tex_coord, 2, 2),
    res_{ 1920, 1080 },
    fbo_(0),
    tex_(0),
    isBackBuffer(true)
  {
  }

  GLuint FrameBuffer::GenBuffer(void)
  {
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    return fbo;
  }

  GLuint FrameBuffer::GenTexture(vec2i const& res, bool HDR)
  {
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      (HDR) ? GL_RGBA16F : GL_RGB,
      res.x,
      res.y, 
      0,
      (HDR) ? GL_RGBA  : GL_RGB,
      (HDR) ? GL_FLOAT : GL_UNSIGNED_BYTE,
      NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
  }

  void FrameBuffer::postInit()
  {
    Bind();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      throw(std::runtime_error("Failed to create writeable frame buffer"));
  }

  void FrameBuffer::setBlendMode(BlendMode blend)
  {
    switch (blend)
    {

    case BlendMode::Mix:
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      break;

    case BlendMode::Additive:
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      break;

    case BlendMode::Multiply:
      glEnable(GL_BLEND);
      glBlendFunc(GL_DST_COLOR, GL_ZERO);
      break;

    case BlendMode::None:
      glDisable(GL_BLEND);
      break;

    case BlendMode::Screen:
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
      break;

    case BlendMode::Overlay:
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
  }

  FrameBuffer::FrameBuffer(vec2i const & res, bool HDR) :
    HDR_(HDR),
    stdSquare_(tex_coord, 2, 2),
    res_(res),
    fbo_(GenBuffer()),
    tex_(GenTexture(res, HDR))

  {
    postInit();
  }

  FrameBuffer::FrameBuffer(int res_x, int res_y, bool HDR) :
    stdSquare_(tex_coord, 2, 2),
    res_(res_x, res_y),
    fbo_(GenBuffer()),
    tex_(GenTexture(res_, HDR))
  {
    postInit();
  }

  FrameBuffer::FrameBuffer(FrameBuffer const & other) :
    stdSquare_{other.stdSquare_},
    res_(other.res_),
    fbo_(GenBuffer()),
    tex_(GenTexture(other.res_))
  {
    postInit();
  }

  FrameBuffer::FrameBuffer(FrameBuffer && other) :
    stdSquare_{ std::move(other.stdSquare_) },
    res_(std::move(other.res_)),
    fbo_(std::move(other.fbo_)),
    tex_(std::move(other.tex_))
  {
    // remove references to the now-our GL handles
    other.fbo_ = 0;
    other.tex_ = 0;
  }

  FrameBuffer::~FrameBuffer()
  {
    Unbind();

    if(tex_)
      glDeleteTextures(1, &tex_);

    if(fbo_)
      glDeleteFramebuffers(1, &fbo_);
  }

  void FrameBuffer::ReInit()
  {
    stdSquare_.ReInit();

    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(1, &tex_);

    fbo_ = GenBuffer();
    tex_ = GenTexture(res_, HDR_);

    postInit();
  }

  void FrameBuffer::regenMesh()
  {
    //stdSquare_.regenerateBuffers();
  }

  FrameBuffer & FrameBuffer::operator=(FrameBuffer rhs)
  {
    swap(rhs);
    return *this;
  }

  void FrameBuffer::swap(FrameBuffer & rhs)
  {
    std::swap(res_, rhs.res_);
    std::swap(fbo_, rhs.fbo_);
    std::swap(tex_, rhs.tex_);
  }


  vec2i const & FrameBuffer::getResolution(void) const
  {
    return res_;
  }

  constexpr SquareTexturedMesh const & FrameBuffer::getMesh(void) const
  {
    return stdSquare_;
  }

  void FrameBuffer::Bind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  }

  void FrameBuffer::Unbind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void FrameBuffer::Clear(Color const & c)
  {
    Bind();
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void FrameBuffer::Draw(Shader & shader, BlendMode blend)
  {
    Draw(shader, 0, blend);
  }

  void FrameBuffer::Draw(Shader & shader, FrameBuffer const & target, BlendMode blend)
  {
    Draw(shader, target.fbo_, blend);
  }

  void FrameBuffer::Draw(Shader &shader, GLuint target, BlendMode blend)
  {
    if (isBackBuffer)
      return;

    setBlendMode(blend);

    glBindFramebuffer(GL_FRAMEBUFFER, target);

    glDisable(GL_DEPTH_TEST);

    shader.useProgram();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_);
    stdSquare_.bindVertexArray();
    stdSquare_.bindVertexData();


    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  void FrameBuffer::Combine(FrameBuffer const& src2, GLuint target, BlendMode blend, float srcScale, float src2Scale)
  {
    setBlendMode(blend);

    Shader & combine = getCombineShader();
    combine.useProgram();

    glBindFramebuffer(GL_FRAMEBUFFER, target);

    glDisable(GL_DEPTH_TEST);

    combine.setUniform1i("input0", 0);
    combine.setUniform1f("scale0", srcScale);
    combine.setUniform1i("input1", 1);
    combine.setUniform1f("scale1", src2Scale);

    stdSquare_.bindVertexArray();
    stdSquare_.bindVertexData();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, src2.tex_);


    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  }

  GLuint FrameBuffer::getFBO(void) const
  {
    return fbo_;
  }

  Shader & FrameBuffer::getDefaultShader()
  {
    static Shader shader{"FBOShader"} ;
    return shader;
  }

  Shader & FrameBuffer::getCombineShader()
  {
    static Shader combine{ "FBOCombine" };
    return combine;
  }

  FrameBuffer & FrameBuffer::getBackBuffer()
  {
    static FrameBuffer backBuffer{ BackBufferSpecifier{} };
    return backBuffer;
  }


}