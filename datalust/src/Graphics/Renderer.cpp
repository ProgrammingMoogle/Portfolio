/**
  \file   Renderer.cpp
  \author Patrick Cook & Samuel Cook

  \brief Class that performs actual OpenGL rendering

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/

#include <glm/gtc/type_ptr.hpp>
#include "Graphics.hpp"
#include "Graphics/GraphicsConstants.hpp"
#include "Graphics\ColliderSprite.hpp"
#include "Graphics\Camera.hpp"
#include "Physics\ParticleSystem.hpp"
#include "Renderer.hpp"


enum DrawModeMask
{
  SHOW_SPRITES = 0b01,
  SHOW_BOUNDS = 0b10
};


namespace Graphics {
  void Renderer::enableDebugDraw() {
    getCurrentRenderer()->draw_mode_ |= DrawModeMask::SHOW_BOUNDS;
  }

  void Renderer::disableDebugDraw() {
    getCurrentRenderer()->draw_mode_ &= ~DrawModeMask::SHOW_BOUNDS;
  }

  void Renderer::enableDraw() {
    getCurrentRenderer()->draw_mode_ |= DrawModeMask::SHOW_SPRITES;
  }

  void Renderer::disableDraw() {
    getCurrentRenderer()->draw_mode_ &= ~DrawModeMask::SHOW_SPRITES;
  }

  void Renderer::onDebugCommand(ObjectId, const Messaging::Message & message) {
    int draw_mode = getCurrentRenderer()->draw_mode_;
    if (message.message.debug.key_state == Messaging::KeyState::Released) {
      switch (draw_mode) {
      case DrawModeMask::SHOW_SPRITES:
        enableDebugDraw();
        break;
      case DrawModeMask::SHOW_BOUNDS:
        enableDraw();
        disableDebugDraw();
        break;
      case (DrawModeMask::SHOW_BOUNDS | DrawModeMask::SHOW_SPRITES):
        disableDraw();
        break;
      default:
        enableDraw();
        break;
      }
    }
  }

  void Renderer::onPauseCommand(ObjectId, const Messaging::Message &m)
  {
    //if (current_renderer_ && m.id == Messaging::ID::PauseRender)
    //  current_renderer_->paused_ = m.message.render_pause.isPaused;
  }

  Renderer::Renderer(Window& win) : win_(&win), draw_mode_(DrawModeMask::SHOW_SPRITES),
    primaryFBO(INTERNAL_WIDTH, INTERNAL_HEIGHT, true),
    /*filter(0.2f),*/ blur_()
  {
    glGenBuffers(1, &circle_uv_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, circle_uv_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle_uvs_), circle_uvs_, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    blur_.setRadius(75);
    blur_.setSigma(25);
    win_->activate();
  }

  void Renderer::draw() 
  {
    Camera * cam = Camera::getActiveCamera();
      if (cam == nullptr)  return;

    // get view and proj matrices
    view_ = cam->WorldToCamera();
    proj_ = cam->CameraToNDC();

    // depths-sort objects
    sortObjects();

    // draw pass
    glViewport(0, 0, 1920, 1080);
    primaryFBO.Clear(Color{ 0, 0, 0, 0 });

    if (draw_mode_ & SHOW_SPRITES)
    {
      drawObjects();

      // post processing stage

      // bloom
      //static FrameBuffer inbetween(1920, 1080);
      //filter.Composite(primaryFBO, inbetween);

      glViewport(0, 0, 1280, 720);
      blur_.Composite(primaryFBO, primaryFBO, BlendMode::Additive);
      glViewport(0, 0, 1920, 1080);
    }

    // Debug pass
    if (draw_mode_ & SHOW_BOUNDS)
      drawBounds();

    // UI pass
    drawUI();

    // draw to backbuffer
    LetterBox();
    primaryFBO.Draw(FrameBuffer::getDefaultShader(), FrameBuffer::getBackBuffer());

    // present
    win_->swapBuffers();
  }

  void Renderer::activate() {
    current_renderer_ = this;
  }

  Renderer * Renderer::getCurrentRenderer() {
    return current_renderer_;
  }

  Renderer * Renderer::current_renderer_ = nullptr;

  void Renderer::addGraphic(Graphic * g) {
    graphic_list_.emplace_back(g);
    //graphic_list_.insert(g);
  }
  void Renderer::removeGraphic(Graphic * g) {

    auto findresult = std::find(graphic_list_.begin(), graphic_list_.end(), g);
    if (findresult != graphic_list_.end()) graphic_list_.erase(findresult);
    //graphic_list_.erase(g);
  }

  glm::mat4 const& Renderer::getView() const
  {
    return view_;
  }

  glm::mat4 const& Renderer::getProj() const
  {
    return proj_;
  }

  void Renderer::setMatrix(glm::mat4 const& transform)
  {
    current_shader->setUniformMatrix4fv("transform", glm::value_ptr(transform));
  }

  void Renderer::setMesh(SquareTexturedMesh const& m)
  {
    m.bindFaceData();
    m.bindVertexData();
  }

  void Renderer::setCurrentShader(ShaderEnum shader)
  {
    switch (shader)
    {
    case ShaderEnum::color:
      current_shader = &color_shader_;
      break;

    case ShaderEnum::texture:
      current_shader = &texture_shader_;
      break;

    case ShaderEnum::background:
      current_shader = &background_shader_;
      break;

    case ShaderEnum::circle:
      current_shader = &circle_shader_;
      break;

    case ShaderEnum::texture_circle:
      current_shader = &texture_circle_shader_;
      break;
    }

    current_shader->useProgram();
  }

  void Renderer::setBlendMode(BlendMode blend)
  {
    //current_shader->setUniform1i("blendmode", static_cast<int>(blend));
#if 1

    current_shader->setUniform1i("blendmode", static_cast<int>(BlendMode::None));
    switch (blend)
    {

    case BlendMode::Mix:
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      break;

    case BlendMode::Additive:
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE);
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
#endif
  }

  void Renderer::setAlpha(float alpha)
  {
    current_shader->setUniform1f("alpha", alpha);
  }

  void Renderer::setColor(Color const& color)
  {
    //Color color{ 1.0f,1.0f,1.0f,1.0f };
    current_shader->setUniform4fv("color", glm::value_ptr(color));
  }

  void Renderer::setTexOffset(Graphics::TexCoord const& texCoord)
  {
    current_shader->setUniform2fv("tex_offset", reinterpret_cast<float const *>(&texCoord));
  }

  void Renderer::regenerateCircleBuffer()
  {
    glGenBuffers(1, &circle_uv_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, circle_uv_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle_uvs_), circle_uvs_, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    for (auto && object : graphic_list_)
    {
      object->ReInit();
    }

    primaryFBO.ReInit();
    blur_.ReInit();
  }

  void Renderer::setParallax(float parallax)
  {
    current_shader->setUniform1f("parallax_factor", parallax);
  }

  void Renderer::selectShader(Texture const& texture, Geometry geo)
  {

    if (texture.loaded())
    {
      if (geo == Geometry::Circle)
        setCurrentShader(ShaderEnum::texture_circle);
      else
        setCurrentShader(ShaderEnum::texture);
    }
    else
    {
      if (geo == Geometry::Circle)
        setCurrentShader(ShaderEnum::circle);
      else
        setCurrentShader(ShaderEnum::color);
    }
  }

  void Renderer::PreDraw(
    Texture const& texture, Geometry const& geo, TexCoord const& texCoord,
    BlendMode blendmode, Color const& color, float alpha, SquareTexturedMesh const& mesh)
  {
    selectShader(texture, geo);

    if (texture.loaded())
    {
      setTextureShaderParams(blendmode, color, alpha);
      setTexOffset(texCoord);
      texture.bindTexture();
      mesh.bindTexCoordData();
    }
    else
    {
      setTextureShaderParams(blendmode, color, alpha);
    }

    mesh.bindVertexArray();
    if (geo == Geometry::Circle)
    {
      glBindBuffer(GL_ARRAY_BUFFER, circle_uv_buffer_);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(2);
    }
    mesh.bindFaceData();
    mesh.bindVertexData();
  }

  void Renderer::drawTris(ArrayRenderParams const& params)
  {
    PreDraw(params.texture, params.geo, params.texCoord, params.blendmode,
      params.color, params.alpha, params.mesh);

    for (auto && transform : params.Object2Screens)
    {
      setMatrix(transform);
      render();
    }
  }

  void Renderer::drawTris(RenderParams const& params)
  {
    PreDraw(params.texture, params.geo, params.texCoord, params.blendmode,
      params.color, params.alpha, params.mesh);

    setMatrix(params.Object2Screen);
    render();
  }

  void Renderer::setColorShaderParams(Color const& color, float alpha)
  {
    setColor(color);
    setAlpha(alpha);
  }

  void Renderer::setTextureShaderParams(BlendMode blendmode, Color const& color, float alpha)
  {
    setBlendMode(blendmode);
    setColorShaderParams(color, alpha);
  }

  void Renderer::drawTris(RenderBackgroundParams const& params)
  {
    setCurrentShader(ShaderEnum::background);

    if (params.texture.loaded())
    {
      setParallax(params.parallax);
      setTextureShaderParams(params.blendmode, params.color, params.alpha);
      setMatrix(params.Object2Screen);
      params.texture.bindTexture();
    }
    else
    {
      setParallax(params.parallax);
      setTextureShaderParams(params.blendmode, params.color, params.alpha);
      setMatrix(params.Object2Screen);
    }

    params.mesh.bindVertexArray();
    params.mesh.bindFaceData();

    render();
  }


  void Renderer::LetterBox()
  {
    static constexpr float target_aspect = static_cast<float>(ASPECT);
    const float aspect = (float)win_->width() / (float)win_->height();
    size_t view_width;
    size_t view_height;
    if (aspect > target_aspect) {
      //proj = glm::scale(proj, glm::vec3(2.0f / (720.0f * aspect), 2.0f / 720.0f, 1));
      view_width = static_cast<size_t>(win_->height() * target_aspect);
      view_height = static_cast<size_t>(win_->height());
    }
    else {
      //proj = glm::scale(proj, glm::vec3(2.0f / 1280.0f, (2.0f * aspect) / 1280.0f, 1));
      view_width = static_cast<size_t>(win_->width());
      view_height = static_cast<size_t>(win_->width() / target_aspect);
    }

    // calculate the viewport
    int xoff = (win_->width() - view_width) / 2;
    int yoff = (win_->height() - view_height) / 2;
    // set the viewport
    glViewport(xoff, yoff, view_width, view_height);
  }

  void Renderer::render()
  {
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  void Renderer::postDraw()
  {
    // unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void Renderer::sortObjects()
  {
    // depth-sort the graphics
    for (auto it : graphic_list_) {
      if (it->graphicType() == Graphic::Type::UISprite)
      {
        UISprite * ui = dynamic_cast<UISprite *>(it);
        if (ui)
          if(ui->GetParent().IsActive())
            depth_ui_list_.insert(ui);
      }
      else if (it->graphicType() == Graphic::Type::Debug && (draw_mode_ & SHOW_BOUNDS))
      {
        bounds_list_.emplace_back(it);
      }
      else if(draw_mode_ & SHOW_SPRITES)
      {
        depth_graphic_list_.insert(it);
      }
    }
  }

  void Renderer::drawObjects()
  {


    for (Graphic * object : depth_graphic_list_)
    {
      switch (object->graphicType())
      {
      case Graphic::Type::Sprite:
      {
        Sprite * sprite = dynamic_cast<Sprite *>(object);
        if (sprite)
          if(sprite->GetParent().IsActive())
            sprite->draw();
        break;
      }
      case Graphic::Type::Background:
      {

        BackgroundImage * background = dynamic_cast<BackgroundImage *>(object);
        if (background)
          if(background->GetParent().IsActive())
            background->draw();
        break;
      }
      case Graphic::Type::CircleSprite:
      {
        CircleSprite * csprite = dynamic_cast<CircleSprite *>(object);
        if (csprite)
          if(csprite->GetParent().IsActive())
            csprite->draw();
        break;
      }
      case Graphic::Type::Particles:
      {
        KPE::ParticleSystem * particle = dynamic_cast<KPE::ParticleSystem *>(object);
        if (particle)
          if (particle->GetParent().IsActive())
            particle->draw();
        break;
      }
      default:
        break;
      }
    }

    depth_graphic_list_.clear();
  }

  void Renderer::drawUI()
  {
    for (UISprite * object : depth_ui_list_)
      object->draw();
    depth_ui_list_.clear();
  }

  void Renderer::drawBounds()
  {
    for (auto && graphic : bounds_list_)
    {
      ColliderSprite * spr = dynamic_cast<ColliderSprite *>(graphic);
      if (spr) spr->draw();
    }
    bounds_list_.clear();
  }
}

