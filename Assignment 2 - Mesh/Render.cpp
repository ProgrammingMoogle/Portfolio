////////////////////
// Kacey Lei Yeo
// Assignment 2
// CS200
// Fall2017
////////////////////
#include "Render.h"
#include <stdexcept>

Render::Render(void)
{
  GLint value;

  //Fragment Shader compilation
  const char *fragment_shader_text =
    "#version 130\n\
     uniform vec3 color;\
     out vec4 frag_color;\
     void main(void) {\
       frag_color = vec4(color,1);\
     }";
  GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fshader, 1, &fragment_shader_text, 0);
  try
  {
    
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &value);

  }
  catch (std::runtime_error& e)
  {
    char buff[1024];
    glGetShaderInfoLog(fshader, 1024, 0, buff);
    throw e;
  }
  
  //Vertex Shader compilation
  const char *vertex_shader_text =
    "#version 130\n\
     attribute vec3 position;\
     uniform mat3 transform;\
     void main() {\
       vec3 v = transform * position;\
       gl_Position = vec4(v.x,v.y,0,1);\
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

  // link shader program
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
  utransform = glGetUniformLocation(program, "transform");
  ucolor = glGetUniformLocation(program, "color");

  // enable the use of the position shader attribute
  glEnableVertexAttribArray(aposition);
}

Render::~Render(void)
{
  // delete shaders and shader program
  glUseProgram(0);
  GLuint shaders[2];
  GLsizei count;
  glGetAttachedShaders(program, 2, &count, shaders);
  for (int i = 0; i < count; ++i)
    glDeleteShader(shaders[i]);
  glDeleteProgram(program);
}

void Render::clearFrame(const Hcoord & c)
{
  // clear background
  glClearColor(c.x, c.y, c.w, 1);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Render::setModelToWorld(const Affine & M)
{
  glUseProgram(program);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glVertexAttribPointer(aposition, 3, GL_FLOAT, false, 0, 0);

  float temp[9] = { M[0].x,M[0].y,M[0].w, 
                    M[1].x,M[1].y,M[1].w,
                    M[2].x,M[2].y,M[2].w };

  

  glUniformMatrix3fv(utransform, 1,true, temp);
}

void Render::loadMesh(Mesh & m)
{
  mesh_edge_count = m.edgeCount();
  mesh_face_count = m.faceCount();

  // vertex buffer for standard square (no. elements and the size of the type)
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, m.vertexCount()* sizeof(Point), m.vertexArray(), GL_STATIC_DRAW);



  // edge list buffer
  glGenBuffers(1, &edge_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edge_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Mesh::Edge) * m.edgeCount(), m.edgeArray(), GL_STATIC_DRAW);


  // face list buffer
  glGenBuffers(1, &face_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Mesh::Face) * m.faceCount(), m.faceArray(), GL_STATIC_DRAW);

}

void Render::unloadMesh(void)
{
  // delete GPU buffers used
  glDeleteBuffers(1, &face_buffer);
  glDeleteBuffers(1, &edge_buffer);
  glDeleteBuffers(1, &vertex_buffer);
}

void Render::displayEdges(const Hcoord & c)
{

  // draw edges
  glUniform3f(ucolor, c.x, c.y, c.w);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edge_buffer);
  glDrawElements(GL_LINES, mesh_edge_count * 2, GL_UNSIGNED_INT, 0);
}

void Render::displayFaces(const Hcoord & c)
{
  // draw faces
  glUniform3f(ucolor, c.x, c.y, c.w);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_buffer);
  glDrawElements(GL_TRIANGLES, mesh_face_count * 3, GL_UNSIGNED_INT, 0);
}
