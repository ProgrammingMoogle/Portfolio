#pragma once
#include <algorithm>
#include <vector>
#include <cmath>
#include <numeric>
#include <glm\gtc\matrix_transform.hpp>

#include "Transform/Transform.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/TextSprite.hpp"

namespace Graphics
{
  // the max number of digits the type can contain in base10
  const size_t Text::maxSize = 
  static_cast<size_t>(
    std::ceil(
      std::log10(
        std::numeric_limits<int>::max()
    )));

  Text::Text(int toDraw)
  {
    digits_.reserve(maxSize);
    setDisplayNumber(toDraw);
  }

  void Text::Draw(TextSprite & text, glm::mat4x4 && transform, glm::mat4x4 && xOffset)
  {
    for (char const & digit : digits_)
    {
      // tell our renderer to draw the next letter
      text.owner_->drawTris(
        Renderer::RenderParams
        {
          text.geometry(),
          text.getTexture(),
          text.blend_mode_,
          text.blend_color_,
          text.alpha_,
          calcOffset(digit),
          transform,
          text.mesh_
        }
      );

      // move to the next spot to draw
      transform = xOffset * transform;
    }
  }

  void Text::DrawWorld(TextSprite & Sprite, glm::mat4x4 && transform, glm::mat4x4 && xOffset)
  {
    const glm::mat4 & toNDC = Sprite.owner_->getProj() * Sprite.owner_->getView();
    for (char const & digit : digits_)
    {
      // tell our renderer to draw the next letter
      Sprite.owner_->drawTris(
        Renderer::RenderParams
        {
          Sprite.geometry(),
          Sprite.getTexture(),
          Sprite.blend_mode_,
          Sprite.blend_color_,
          Sprite.alpha_,
          calcOffset(digit),
          toNDC *transform,
          Sprite.mesh_
        }
      );

      // move to the next spot to draw
      transform = xOffset * transform;
    }
  }
  /*
  TextSprite::TextSprite(
    GameObject& parent, const char * filename, const Size& size,
    float depth, size_t frame, float alpha, const Color& color, BlendMode mode) :
  
  {

  }
  */

  TextSprite::TextSprite(GameObject & parent, const char * filename, const Size & size, float depth, size_t frame, float alpha, const Color & color, BlendMode mode)
    : UISprite(parent, filename, size, depth, frame, alpha, color, mode), text{-1}
  {
    mesh_.setUV(
      std::array<TexCoord, 4>{ {
        { 0.0f, 1.0f },
        { 0.1f, 1.0f },
        { 0.1f, 0.0f },
        { 0.0f, 0.0f }
        }}.data());
  }
  void TextSprite::setDisplayNumber(int number)
  {
    text.setDisplayNumber(number);
  }
  void TextSprite::draw()
  {
    Transform *transform = GetParent().Find<Transform>();
    if (transform == nullptr)
      return;

    // Considerations: Support for centered and right-aligned text
    // Some initial offset from the GameObject

    // TODO, get a starting position and the scale...

    text.Draw(*this, getTransform(*transform), getShiftAmount(*transform));
  }

  Graphic::Type TextSprite::graphicType() const
  {
    return Graphic::Type::UISprite;
  }

  void Text::setDisplayNumber(int display)
  {
    // avoid redundant computations
    if (toDisplay_ == display)
      return;

    digits_.clear();
    toDisplay_ = display;

    // modulus is undefined for negative numbers
    if (display < 0)
      return;

    int radix = display;
    do {
      // get least-significant digit
      digits_.emplace_back(static_cast<char>(radix % base));
      // shove out least-significant digit
      radix /= base;
    } while (radix > 0);

    // algorithm produces digits in *reverse* order
    std::reverse(digits_.begin(), digits_.end());
  }

  size_t Text::getCount() const
  {
    return digits_.size();
  }

  constexpr TexCoord Text::calcOffset(char digit)
  {
    // TODO: use texture size to find uv's
    return TexCoord{ static_cast<float>(digit) / static_cast<float>(base), 0.0f };
  }

  glm::mat4 TextSprite::getShiftAmount(Transform const& transform)
  {
    return glm::translate(glm::mat4{ 1 }, glm::vec3{ transform.getXScale(), 0.0f, 0.0f });
  }

  glm::mat4 TextSprite::getTransform(Transform const& transform)
  {
    return transform.getGLTransform();
  }

  void WorldTextSprite::setFontSize(float size)
  {
    fontSize = std::abs(size);
  }

  glm::vec2 const & WorldTextSprite::getOffset() const
  {
    return offset;
  }

  glm::mat4 WorldTextSprite::getModel(Transform const& transform) const
  {
    return
      glm::translate(
        glm::mat4{ 1 }
        , glm::vec3{
          transform.getPosition().x + offset.x - (fontSize * (text.getCount() - 1) * 0.5f),
          transform.getPosition().y + offset.y,
          0.0f }) *
          glm::scale(glm::mat4{ 1 }, glm::vec3{ glm::vec2{ fontSize }, 0.0f });
  }


  glm::mat4 WorldTextSprite::getOffset(Transform const &) const
  {
    return glm::translate(glm::mat4{ 1 }, glm::vec3{ fontSize, 0.0f, 0.0f });
  }

  WorldTextSprite::WorldTextSprite(GameObject & parent, const char * filename, const Size & size, float depth, size_t frame, float alpha, const Color & color, BlendMode mode) :
    TextSprite{ parent, filename, size, depth, frame, alpha, color, mode }, offset{ 0 }, fontSize{ 50 }
  {
    tex_.loadFromFile("data/img/bg/digits_black.png");
  }

  void WorldTextSprite::draw()
  {
    Transform *transform = GetParent().Find<Transform>();
    if (transform == nullptr)
      return;

    text.DrawWorld(*this, getModel(*transform), getOffset(*transform));
  }

  void WorldTextSprite::setOffset(glm::vec2 && _offset)
  {
    offset = std::move(_offset);
  }

  void WorldTextSprite::setOffset(glm::vec2 const& _offset)
  {
    offset = _offset;
  }

}