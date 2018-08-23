#pragma once
#include "Graphics\GraphicsTypes.hpp"
#include "Graphics\Mesh.hpp"


namespace Graphics
{
  class Shader;
  typedef glm::vec<2, int> vec2i;
  typedef unsigned GLuint;

  class FrameBuffer
  {
  private:
    struct BackBufferSpecifier {};
    FrameBuffer(BackBufferSpecifier const&);

    SquareTexturedMesh stdSquare_;

    // PRIVATE VARS
    // high-dynamic ranging buffer
    bool HDR_;

    // resolution of frame buffer
    vec2i res_;

    // frame buffer handle
    GLuint fbo_;

    // texture rendered onto fbo
    GLuint tex_;

    const bool isBackBuffer = false;

    // STATIC HELPERS
    static GLuint GenBuffer(void);
    static GLuint GenTexture(vec2i const& res, bool HDR = false);


    void postInit();
    void setBlendMode(BlendMode mode);
  public:
    // ctors

    // conversion/non-default
    FrameBuffer(vec2i const& res, bool HDR = false);
    FrameBuffer(int res_x, int res_y, bool HDR = false);

    // copy
    FrameBuffer(FrameBuffer const& other);
    // move
    FrameBuffer(FrameBuffer && other);

    // dtors
    ~FrameBuffer();

    void ReInit();
    static void regenMesh();

    FrameBuffer & operator=(FrameBuffer rhs);
    void swap(FrameBuffer & rhs);
    // getters
    vec2i const& getResolution(void) const;
    constexpr SquareTexturedMesh const& getMesh(void) const;

    // methods
    void Bind();                // render to this buffer
    void Unbind();              // return to rendering to backbuffer

    // The following methods require the buffer to be bound
    void Clear(Color const& c); // clears buffer to a selected color
    void Draw(Shader &shader, BlendMode = BlendMode::None); // render buffer as texture to backbuffer

    void Draw(Shader &shader, FrameBuffer const& target, BlendMode = BlendMode::None);

    void Draw(Shader &shader, GLuint target, BlendMode = BlendMode::None);


    void Combine(FrameBuffer const& src2, GLuint target, BlendMode = BlendMode::None, float srcScale = 1.0f, float src2Scale = 1.0f);

    GLuint getFBO(void) const;

    static Shader & getDefaultShader();
    static Shader & getCombineShader();
    static FrameBuffer & getBackBuffer();

  };
} // namespace Graphics