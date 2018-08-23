#pragma once
#include "Graphics/Sprite.hpp"

class Transform;
namespace Graphics
{
  class TextSprite;
  class Text
  {
  public:
    Text(int toDraw);
    void Draw(TextSprite & Sprite, glm::mat4x4 && transform, glm::mat4x4 && xOffset);
    void DrawWorld(TextSprite & Sprite, glm::mat4x4 && transform, glm::mat4x4 && xOffset);

    void setDisplayNumber(int display);

    size_t getCount() const;

  private:
    int toDisplay_;
    std::vector<char> digits_;
    static constexpr size_t base = 10;
    static const size_t maxSize;


    static constexpr TexCoord calcOffset(char digit);
  };

  class TextSprite : public UISprite
  {
    // methods
  public:
    TextSprite(
      GameObject& parent, 
      const char* filename = NULL, 
      const Size& size = { 1,1 }, 
      float depth = 0.0f, 
      size_t frame = 0,
      float alpha = 1.0f,
      const Color& color = { 0, 0, 0, 0 },
      BlendMode mode = BlendMode::Mix
    );


    void setDisplayNumber(int number);
    virtual void draw();
    virtual Graphic::Type graphicType() const;
    friend void Text::DrawWorld(TextSprite & Sprite, glm::mat4x4 && transform, glm::mat4x4 && xOffset);
    friend void Text::Draw(TextSprite & TextSprite, glm::mat4x4 && transform, glm::mat4x4 && xOffset);
    // vars
  protected:
    Text text;


    // methods
  private:
    static glm::mat4 getShiftAmount(Transform const& transform);
    static glm::mat4  getTransform(Transform const& transform);

  };


  class WorldTextSprite : public TextSprite
  {
  public:
    WorldTextSprite(
      GameObject& parent,
      const char* filename = NULL,
      const Size& size = { 1,1 },
      float depth = 0.0f,
      size_t frame = 0,
      float alpha = 1.0f,
      const Color& color = { 1, 1, 1, 1 },
      BlendMode mode = BlendMode::Mix
    );


    virtual void draw();
    void setOffset(glm::vec2 && offset);
    void setOffset(glm::vec2 const& offset);
    void setFontSize(float size);
    
    glm::vec2 const& getOffset() const;
  private:

    glm::mat4 getModel(Transform const&) const;
    glm::mat4 getOffset(Transform const&) const;
    glm::vec2 offset;
    float fontSize;
  };
}