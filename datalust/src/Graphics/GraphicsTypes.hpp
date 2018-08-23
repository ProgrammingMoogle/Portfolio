/**
  \file   GraphicsTypes.hpp
  \author Samuel Cook

  \brief Types used by the Graphics Engine.

  A collection of enums and POD-structs that are used in the Graphics engine's interface and by the
  private implementation.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _GRAPHICS_TYPES_HPP_
#define _GRAPHICS_TYPES_HPP_

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

namespace Graphics {
  /**
    \brief How a texture should handle texels with uv coordinates outside the range [0, 1].
  */
  enum class RepeatMode {
    None,   ///< A solid background color will be used for texels outside the range [0, 1].
    Repeat, ///< The texture will repeat when texels fall outside the range [0, 1].
    Mirror, ///< Every other repeat will be flipped along the repeating axis.
    Clamp,  ///< The colors at the edge of the texture will be extended.
  };

  /**
    \brief How to handle alpha blending between two colors.
  */
  enum class BlendMode {
    None,     ///< No alpha blending, only the top color will show.
    Mix,      ///< The top and bottom colors will be mixed based on alpha.
    Multiply, ///< The top and bottom colors will be multiplied together, darkens.
    Screen,   ///< The colors will be inverted, multiplied and inverted again, lightens.
    Overlay,  ///< a combination of Multiply and Screen, darkens darks and lightens lights.
    Additive,
  };

  /**
    \brief A color in RGBA floating-point representation.
  */
  typedef glm::vec4 Color;

  /**
    \brief The vertex representation used by the graphics engine.
  */
  struct Vertex
  {
    constexpr Vertex(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
    float x, y, z;
  };

  /**
    \brief The texture coordinate representation used by the graphics engine.
  */
  struct TexCoord
  {
    constexpr TexCoord(float _u = 0, float _v = 0) : u(_u), v(_v) {}
    float u, v;
  };
  
  typedef glm::vec2 Size;
}

#endif