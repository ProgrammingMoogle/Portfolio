

#include "TextureRender.h"
#include <stdexcept>

TextureRender::TextureRender(void)
{
  GLint value;

  const char *fragment_shader_text =
    "#version 130\n\
     uniform sampler2D usamp;\
     in vec2 vtexcoord;\
     out vec4 frag_color;\
     void main(void) {\
       frag_color = texture(usamp,vtexcoord);\
     }";
  GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fshader, 1, &fragment_shader_text, 0);
  try
  {
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &value);
  }
  catch (const std::runtime_error& e)
  {
    char buffer[1024];
    glGetShaderInfoLog(fshader, 1024, 0, buffer);
    throw e;
  }
  
  const char *vertex_shader_text =
    "#version 130\n\
     attribute vec3 position;\
     attribute vec2 texcoord;\
     uniform mat3 transform;\
     out vec2 vtexcoord;\
     void main() {\
       vec3 v = transform * position;\
       gl_Position = vec4(v.x,v.y,0,1);\
       vtexcoord = texcoord;\
     }";
  GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vshader, 1, &vertex_shader_text, 0);
  try
  {
    glCompileShader(vshader);
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &value);
  }
  catch (const std::runtime_error& e)
  {
    char buffer[1024];
    glGetShaderInfoLog(vshader, 1024, 0, buffer);
    throw e;
  }
  
  //link the shaders
  program = glCreateProgram();
  glAttachShader(program, fshader);
  glAttachShader(program, vshader);
  try
  {
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &value);
  }
  catch (const std::runtime_error& e)
  {
    char buffer[1024];
    glGetProgramInfoLog(program, 1024, 0, buffer);
    throw e;
  }

  // shader parameter locations
  aposition = glGetAttribLocation(program, "position");
  atexcoord = glGetAttribLocation(program, "texcoord");
  utransform = glGetUniformLocation(program, "transform");

  // enable the use of the position and texcoord shader attributes
  glEnableVertexAttribArray(aposition);
  glEnableVertexAttribArray(atexcoord);
}

TextureRender::~TextureRender(void)
{
  glUseProgram(0);
  glDeleteProgram(program);
}

void TextureRender::clearFrame(const Hcoord & c)
{
  glClearColor(c.x, c.y, c.w, 1);
  glClear(GL_COLOR_BUFFER_BIT);
}

void TextureRender::loadTexture(unsigned char * rgbdata, int width, int height)
{
  glGenTextures(1, &texture_buffer);
  glBindTexture(GL_TEXTURE_2D, texture_buffer);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
    0, GL_RGB, GL_UNSIGNED_BYTE, rgbdata);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void TextureRender::unloadTexture(void)
{
  glDeleteBuffers(1, &texture_buffer);
}

void TextureRender::setModelToWorld(const Affine & M)
{
  glUseProgram(program);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glVertexAttribPointer(aposition, 3, GL_FLOAT, false, 0, 0);

  float temp[9] = { M[0].x,M[0].y,M[0].w,
    M[1].x,M[1].y,M[1].w,
    M[2].x,M[2].y,M[2].w };

  glUniformMatrix3fv(utransform, 1, true, temp);

  //Binds the the texture buffer and sets the attributes
  glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
  glVertexAttribPointer(atexcoord, 3, GL_FLOAT, false, 0, 0);
}

void TextureRender::loadMesh(TexturedMesh & m)
{
  mesh_face_count = m.faceCount();

  // vertex buffer for standard square
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, m.vertexCount() * sizeof(Point), m.vertexArray(), GL_STATIC_DRAW);

  // texture coordinate buffer
  glGenBuffers(1, &texcoord_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
  glBufferData(GL_ARRAY_BUFFER, m.vertexCount() * sizeof(Point), m.texcoordArray(), GL_STATIC_DRAW);

  // face list buffer
  glGenBuffers(1, &face_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Mesh::Face) * m.faceCount(), m.faceArray(), GL_STATIC_DRAW);

  
}

void TextureRender::unloadMesh(void)
{
  glDeleteBuffers(1, &face_buffer);
  glDeleteBuffers(1, &vertex_buffer);
  glDeleteBuffers(1, &texcoord_buffer);
}

void TextureRender::displayFaces(void)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_buffer);
  glDrawElements(GL_TRIANGLES, mesh_face_count * 3, GL_UNSIGNED_INT, 0);

  
}
