/**
  \file   Background.hpp
  \author Samuel Cook

  \brief Components that represent mostly static environments.



  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include <cmath>
#include "Engine\Engine.h" // dt
#include "Transform\Transform.hpp"
#include "Background.hpp"
#include "GameObject/GameObject.h"
#include "Renderer.hpp"
#include <GL/glew.h>
#include <glm\gtc\matrix_transform.hpp>

namespace Graphics {
  namespace {
    // maps engine enum to opengl enum
    GLenum RepeatMode2GL(RepeatMode rm) {
      switch(rm) {
        case RepeatMode::Clamp:
          return GL_CLAMP_TO_EDGE;
        case RepeatMode::Mirror:
          return GL_MIRRORED_REPEAT;
        case RepeatMode::None:
          return GL_CLAMP_TO_BORDER;
        case RepeatMode::Repeat:
          return GL_REPEAT;
        default:
          return GL_REPEAT;
      }
    }

    // maps opengl enum to engine enum
    RepeatMode GL2RepeatMode(GLenum glenum) {
      switch(glenum) {
        case GL_CLAMP_TO_EDGE:
          return RepeatMode::Clamp;
        case GL_MIRRORED_REPEAT:
          return RepeatMode::Mirror;
        case GL_CLAMP_TO_BORDER:
          return RepeatMode::None;
        case GL_REPEAT:
          return RepeatMode::Repeat;
        default:
          return RepeatMode::Repeat;
      }
    }
  }

  BackgroundImage::BackgroundImage(GameObject& parent, const char *filename, float depth,
    float parallax_factor, float alpha,
    float r, float g, float b,
    float a, BlendMode mode) : Background(depth, parallax_factor, alpha, r, g, b, a, mode), Component(parent) {
    loadFromFile(filename);
  }

  BackgroundImage& BackgroundImage::loadFromFile(const char *filename) {
	  if (filename)
	  {
		  if (filename[0] != 0) {
			  getTexture().loadFromFile(filename);
			  calculateUVs();
		  }
	  }
    return *this;
  }
  BackgroundImage& BackgroundImage::setRepeat(RepeatMode horizontal, RepeatMode vertical) {
    getTexture().bindTexture();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, RepeatMode2GL(horizontal));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, RepeatMode2GL(vertical));
    glBindTexture(GL_TEXTURE_2D, 0);
    return *this;
  }
  BackgroundImage& BackgroundImage::getRepeat(RepeatMode *horizontal, RepeatMode *vertical) {
    getTexture().bindTexture();
    if(horizontal) {
      GLint s;
      glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &s);
      *horizontal = GL2RepeatMode(s);
    }
    if(vertical) {
      GLint t;
      glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &t);
      *vertical = GL2RepeatMode(t);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return *this;
  }

  BackgroundImage const& BackgroundImage::draw()
  {
    Renderer * render = Renderer::getCurrentRenderer();

    GameObject * parent = &(GetParent());
    parent->Find<Transform>()->Update(0);
    //world = parent->Find<Transform>()->getGLTransform();

    glm::vec2 pos(parent->Find<Transform>()->getPosition());
    glm::vec2 scale(parent->Find<Transform>()->getXScale(), parent->Find<Transform>()->getYScale());
    float angle = parent->Find<Transform>()->getOrientation();
    glm::mat4 world = glm::translate(glm::mat4(1), glm::vec3(pos, 0));
    world = world * glm::scale(glm::mat4(1), glm::vec3(scale, 1));

    glm::mat4 world2 = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 0, 1));

    calculateUVs();

    Size s = getTexture().getSize();
    s.x *= parent->Find<Transform>()->getXScale();
    s.y *= parent->Find<Transform>()->getYScale();

    glm::mat4 tex2world(1.0f);

    //tex2world = glm::translate(tex2world, glm::vec3(-0.5, -0.5, 0));
    tex2world = glm::scale(tex2world, glm::vec3(s.x, s.y, 1));

    glm::mat4 world2tex = glm::inverse(tex2world);

    render->drawTris(
      Renderer::RenderBackgroundParams(
        tex_,
        parallax_factor_,
        blend_mode_,
        blend_color_,
        0.5f * alpha_ * (std::abs(std::sin(0.5f * engine.runTime())) + 1.0f),
        world2tex * glm::inverse(world2) * glm::inverse(render->getView()) * glm::inverse(world) * tex2world,
        mesh_
      )
    );

    return *this;
  }

  BackgroundImage& BackgroundImage::calculateUVs() {
    Size size;
    size.x = 1280;
    size.y = 720;
    float halfwidth = (size.x / getTexture().getSize().x) * 0.5f;
    float halfheight = (size.y / getTexture().getSize().y) * 0.5f;
    // set the uv coordinates of the mesh such that the background tiles in the viewport
    TexCoord tc[4] = {
      TexCoord{-halfwidth,  halfheight},
      TexCoord{ halfwidth,  halfheight},
      TexCoord{ halfwidth, -halfheight},
      TexCoord{-halfwidth, -halfheight},
    };
    getMesh().setUV(tc);
    return *this;
  }

  ::Component* BackgroundImage::Clone_() const {
    return new BackgroundImage(*this);
  }
}