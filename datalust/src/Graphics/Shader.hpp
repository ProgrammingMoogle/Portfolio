/**
  \file   Shader.hpp
  \author Samuel Cook

  \brief Shader wrapper interface

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <GL/glew.h>
#include <map>
#include <string>

namespace Graphics {
  class Shader {
    public:
      Shader(const char * shader_name = nullptr);
      bool loadFromFile(const char * filename);
      void useProgram();
      void setUniformMatrix4fv(const char * uname, const float * val);
      void setUniform4fv(const char * uname, const float * val);
      void setUniform1f(const char * uname, float val);
      void setUniform2fv(const char * uname, const float * val);
      void setUniform1i( const char * uname, int val );

      void setUniformArrayf(const char * uname, int size, float const* val);

      constexpr bool operator<(Shader const& rhs) const;

    private:
      bool Shader::loadVertShader(const char * filename);
      bool Shader::loadFragShader(const char * filename);
      bool Shader::linkProgram();
      
      static std::map<std::string, Shader> * loaded_shaders_;

    private:
#ifdef _DEBUG
      std::string name_;
#endif
      GLuint vertex_shader_ = 0;
      GLuint fragment_shader_ = 0;
      GLuint program_ = 0;
  };
}

#endif