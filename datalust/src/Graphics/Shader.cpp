/**
  \file   Shader.cpp
  \author Samuel Cook

  \brief Shader wrapper implementation

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Shader.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Engine\Engine.h>
#include <Trace\Trace.hpp>

namespace Graphics {

  constexpr bool Shader::operator<(Shader const& rhs) const
  {
    return   vertex_shader_ < rhs.vertex_shader_   ||
           fragment_shader_ < rhs.fragment_shader_ ||
                   program_ < rhs.program_;
  }

  std::map<std::string, Shader> *Shader::loaded_shaders_ = nullptr;

  Shader::Shader(const char * shader_name)
#ifdef  _DEBUG
    : name_(shader_name)
#endif 
  {
    if (shader_name == nullptr)
      return;

    if (loaded_shaders_ == nullptr)
    {
      try
      {
        loaded_shaders_ = new std::map<std::string, Shader>{};
      }
      catch (std::exception & e)
      {
        throw std::runtime_error(e.what());
      }
    }

    if (!loaded_shaders_->empty())
    {
      auto find_result = loaded_shaders_->find(shader_name);
      if (find_result != loaded_shaders_->cend())
      {
        *this = find_result->second;
        return;
      }
    }

    if (!loadFromFile(shader_name)) 
    {
      glDeleteShader(vertex_shader_);
      glDeleteShader(fragment_shader_);
      glDeleteProgram(program_);

      throw std::runtime_error("Failed to Compile/Link Shader");
    }


    loaded_shaders_->emplace(shader_name, *this);
  }

  bool Shader::loadFromFile(const char * filename) {
    return
      loadVertShader(filename) &&
      loadFragShader(filename) &&
      linkProgram();
  }

  void Shader::useProgram() {
    glUseProgram(program_);
  }

  void Shader::setUniformMatrix4fv(const char * uname, const float * val) {
    glUseProgram(program_);
    glUniformMatrix4fv(glGetUniformLocation(program_, uname), 1, false, val);
  }

  void Shader::setUniform4fv(const char * uname, const float * val) {
    glUseProgram(program_);
    glUniform4fv(glGetUniformLocation(program_, uname), 1, val);
  }


  void Shader::setUniform1f(const char * uname, float val) {
    glUseProgram(program_);
    glUniform1f(glGetUniformLocation(program_, uname), val);
  }
  void Shader::setUniform2fv(const char * uname, const float * val) {
    glUseProgram(program_);
    glUniform2fv(glGetUniformLocation(program_, uname), 1, val);
  }

  void Shader::setUniform1i(const char * uname, int val) {
    glUseProgram(program_);
    glUniform1i(glGetUniformLocation(program_, uname), val);
  }

  void Shader::setUniformArrayf(const char * uname, int size, float const* val)
  {
    glUseProgram(program_);
    glUniform1fv(glGetUniformLocation(program_, uname), size, val);
  }

  bool Shader::loadVertShader(const char * filename) {
    const std::string vs_path{ std::string{ "data/glsl/" } + filename + ".vert" };

    std::ifstream in(vs_path.c_str());
    if (in.is_open()) {
      // read in the shader
      if (vertex_shader_) glDeleteShader(vertex_shader_);
      vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
      std::stringstream ss;
      ss << in.rdbuf();
      char * cstr = new char[ss.str().length() + 1];
      strcpy(cstr, ss.str().c_str());
      // compile the shader
      glShaderSource(vertex_shader_, 1, &cstr, 0);
      glCompileShader(vertex_shader_);
      delete[] cstr;
      int  success;
      char infoLog[512];
      glGetShaderiv(vertex_shader_, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(vertex_shader_, 512, NULL, infoLog);
        engine.Find<Trace::TraceHandler>()->TraceMessage(TraceLevel::ERROR, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
        return false;
      }
      return true;
    }
    engine.Find<Trace::TraceHandler>()->TraceMessage(TraceLevel::ERROR, "ERROR: Failed to open %s", vs_path);
    return false;
  }

  bool Shader::loadFragShader(const char * filename) {
    const std::string fs_path{ std::string{"data/glsl/"} + filename + ".frag" };

    std::ifstream in(fs_path.c_str());
    if (in.is_open()) {
      // read in the shader
      if (fragment_shader_) glDeleteShader(fragment_shader_);
      fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
      std::stringstream ss;
      ss << in.rdbuf();
      char * cstr = new char[ss.str().length() + 1];
      strcpy(cstr, ss.str().c_str());
      // compile the shader
      glShaderSource(fragment_shader_, 1, &cstr, 0);
      glCompileShader(fragment_shader_);
      delete[] cstr;
      int  success;
      char infoLog[512];
      glGetShaderiv(fragment_shader_, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(fragment_shader_, 512, NULL, infoLog);
        engine.Find<Trace::TraceHandler>()->TraceMessage(TraceLevel::ERROR, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
        return false;
      }
      return true;
    }
    engine.Find<Trace::TraceHandler>()->TraceMessage(TraceLevel::ERROR, "ERROR: Failed to open %s", fs_path);
    return false;
  }

  bool Shader::linkProgram() {
    program_ = glCreateProgram();
    // link the shaders together into a program
    glAttachShader(program_, vertex_shader_);
    glAttachShader(program_, fragment_shader_);
    glLinkProgram(program_);
    int  success;
    char infoLog[512];
    glGetProgramiv(program_, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(program_, 512, NULL, infoLog);
      engine.Find<Trace::TraceHandler>()->TraceMessage(TraceLevel::ERROR, "ERROR::SHADER::PROGRAM::LINK_FAILED\n%s", infoLog);
      return false;
    }
    return true;
  }
}