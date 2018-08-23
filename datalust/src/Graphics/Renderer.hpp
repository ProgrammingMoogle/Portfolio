/**
  \file   Renderer.hpp
  \author Samuel Cook

  \brief class that performs actual OpenGL rendering

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

#include "Geometry.hpp"
#include "GraphicsTypes.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "BrightFilter.hpp"
#include "Blur.hpp"
#include "GraphicBase.hpp"
#include "FrameBuffer.hpp"
#include <Messaging/Messaging.hpp>
#include <vector>
#include <set>

namespace Graphics {
  /**
  \brief Handles OpenGL Rendering.
  */
  class Renderer {
  public:
    struct RenderBackgroundParams
    {
      constexpr RenderBackgroundParams(
        Texture            const& _texture,
        float              const& _parallax,
        BlendMode          const& _blendmode,
        Color              const& _color,
        float              const& _alpha,
        glm::mat4          const& _Object2Screen,
        SquareTexturedMesh const& _mesh
      ) :
        parallax(_parallax),
        blendmode(_blendmode),
        texture(_texture),
        color(_color),
        alpha(_alpha),
        Object2Screen(_Object2Screen),
        mesh(_mesh)
      {}


      Texture            const& texture;
      float              const& parallax;
      BlendMode          const& blendmode;
      Color              const& color;
      float              const& alpha;
      glm::mat4          const& Object2Screen;
      SquareTexturedMesh const& mesh;
    };

    struct RenderParams
    {
      constexpr RenderParams(
        Geometry           const& _geo,
        Texture            const& _texture,
        BlendMode          const& _blendmode,
        Color              const& _color,
        float              const& _alpha,
        TexCoord           const& _texCoord,
        glm::mat4          const& _Object2Screen,
        SquareTexturedMesh const& _mesh
      ) :
        geo(_geo),
        texture(_texture),
        blendmode(_blendmode),
        color(_color),
        alpha(_alpha),
        texCoord(_texCoord),
        Object2Screen(_Object2Screen),
        mesh(_mesh)
      {}

      Geometry           const& geo;
      Texture            const& texture;
      BlendMode          const& blendmode;
      Color              const& color;
      float              const& alpha;
      TexCoord           const& texCoord;
      glm::mat4          const& Object2Screen;
      SquareTexturedMesh const& mesh;

    };

    struct ArrayRenderParams
    {
      constexpr ArrayRenderParams(
      Geometry               const& _geo,
      Texture                const& _texture,
      BlendMode              const& _blendmode,
      Color                  const& _color,
      float                  const& _alpha,
      TexCoord               const& _texCoord,
      std::vector<glm::mat4> && _Object2Screens,
      SquareTexturedMesh     const& _mesh
      ) :
        geo(_geo),
        texture(_texture),
        blendmode(_blendmode),
        color(_color),
        alpha(_alpha),
        texCoord(_texCoord),
        Object2Screens(std::move(_Object2Screens)),
        mesh(_mesh)
      {}

        Geometry               const& geo;
        Texture                const& texture;
        BlendMode              const& blendmode;
        Color                  const& color;
        float                  const& alpha;
        TexCoord               const& texCoord;
        std::vector<glm::mat4> const& Object2Screens;
        SquareTexturedMesh     const& mesh;
    };
    /// Constructor
    Renderer(Window& win);
    /// Retrieves the Active Renderer
    static Renderer * getCurrentRenderer();
    /// Draws the scene.
    void draw();
    /// Makes this render the active one.
    void activate();
    /// Register a Graphics with this renderer.
    void addGraphic(Graphic *);
    /// Unregister a Graphic with this renderer.
    void removeGraphic(Graphic *);

    enum class ShaderEnum : unsigned int
    {
      color = 0,
      texture,
      background,
      circle,
      texture_circle
    };

    const Window * const window() const { return win_; }

    static void enableDebugDraw();

    static void disableDebugDraw();

    static void enableDraw();

    static void disableDraw();

    static void onDebugCommand(ObjectId, const Messaging::Message&);

    static void onPauseCommand(ObjectId, const Messaging::Message&);

    void drawTris(RenderParams const& params);
    void drawTris(ArrayRenderParams const& params);
    void drawTris(RenderBackgroundParams const& params);

    glm::mat4 const& getView() const;
    glm::mat4 const& getProj() const;

    void regenerateCircleBuffer();

  private:
    void LetterBox();

    void render();
    void selectShader(Texture const& texture, Geometry geo);
    void PreDraw(
      Texture const& texture, Geometry const& geo, TexCoord const& texCoord,
      BlendMode blendmode, Color const& color, float alpha, SquareTexturedMesh const& mesh);
    void postDraw();

    static Renderer * current_renderer_;
    std::vector<Graphic*> graphic_list_;
    std::multiset<Graphic*, GraphicDepthCompare> depth_graphic_list_;
    std::multiset<UISprite*, GraphicDepthCompare> depth_ui_list_;
    std::vector<Graphic*> bounds_list_;
    Window * win_;
    void sortObjects();
    void drawObjects();
    void drawUI();
    void drawBounds();

    Shader color_shader_{ "ColorShader" };
    Shader texture_shader_{ "TextureShader" };
    Shader background_shader_{ "BGShader" };
    Shader circle_shader_{ "CircleShader" };
    Shader texture_circle_shader_{ "TextureCircleShader" };

    Shader * current_shader = &color_shader_;

    GLuint circle_uv_buffer_;
    int draw_mode_;

    FrameBuffer primaryFBO;
    Blur blur_;
    //BrightFilter filter;
    //Bloom bloom;


    glm::mat4 view_;
    glm::mat4 proj_;

    void setColorShaderParams(Color const& color, float alpha);
    void setTextureShaderParams(BlendMode blendmode, Color const& color, float alpha);

    void setParallax(float parallax);

    void setMatrix(glm::mat4 const& transform);
    void setMesh(SquareTexturedMesh const& m);

    void setCurrentShader(ShaderEnum shader);
    void setBlendMode(BlendMode blend);
    void setAlpha(float alpha);

    void setColor(Color const& color);
    void setTexOffset(Graphics::TexCoord const& texCoord);


  };
}