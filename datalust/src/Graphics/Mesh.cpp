/**
  \file   Mesh.cpp
  \author Samuel Cook

  \brief Encapsulates an OpenGL mesh


  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include <GL\glew.h>
#include <cstring>
#include <utility>
#include "Mesh.hpp"

namespace Graphics {
  GLuint SquareTexturedMesh::vert_buff_ = 0;
  GLuint SquareTexturedMesh::face_buff_ = 0;

  // vertices used by all meshes


  // index buffer used by all meshes


  SquareTexturedMesh::SquareTexturedMesh(const TexCoord* texcoords, float width, float height) : size_{ width, height } {
    // generate the vertex and face buffers if they haven't been generated yet
    if(!vert_buff_) {
      genVertBuffer();
    }
    if(!face_buff_) {
      genFaceBuffer();
    }
    // copy the uv texture coordinates into the object
    memcpy(texcoords_, texcoords, sizeof(texcoords_));

    glGenBuffers(1, &texcoord_buff_);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord_buff_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords_), texcoords_, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set up the vao
    //(vao_) glDeleteVertexArrays(1, &vao_);
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
      bindVertexData();
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(0);
      bindTexCoordData();
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(1);
      bindFaceData();
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  SquareTexturedMesh::SquareTexturedMesh(const SquareTexturedMesh & o) :
	  size_(o.size_)
  {
    // same as default constructor but for copying
	  if (!vert_buff_) {
		  genVertBuffer();
	  }
	  if (!face_buff_) {
		  genFaceBuffer();
	  }
	  memcpy(texcoords_, o.texcoords_, sizeof(texcoords_));
	  glGenBuffers(1, &texcoord_buff_);
	  glBindBuffer(GL_ARRAY_BUFFER, texcoord_buff_);
	  glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(TexCoord), texcoords_, GL_STATIC_DRAW);
	  glBindBuffer(GL_ARRAY_BUFFER, 0);

	  if (vao_) glDeleteVertexArrays(1, &vao_);
	  glGenVertexArrays(1, &vao_);
	  glBindVertexArray(vao_);
	  bindVertexData();
	  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(0);
	  bindTexCoordData();
	  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(1);
	  bindFaceData();
	  glBindVertexArray(0);
	  glBindBuffer(GL_ARRAY_BUFFER, 0);
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  SquareTexturedMesh::SquareTexturedMesh(SquareTexturedMesh && o) :
	  texcoord_buff_(std::move(o.texcoord_buff_)),
	  vao_(std::move(o.vao_)),
	  size_(std::move(o.size_))
  {
	  o.texcoord_buff_ = 0;
	  o.vao_ = 0;
	  memmove(texcoords_, o.texcoords_, sizeof(texcoords_));
  }
  SquareTexturedMesh::~SquareTexturedMesh() {
    if ( texcoord_buff_ )
      glDeleteBuffers( 1, &texcoord_buff_ );
    if ( vao_ )
      glDeleteVertexArrays( 1, &vao_ );
  }
  void SquareTexturedMesh::bindVertexData() {
    glBindBuffer(GL_ARRAY_BUFFER, vert_buff_);
  }
  void SquareTexturedMesh::bindFaceData() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_buff_);
  }
  void SquareTexturedMesh::bindVertexArray() const {
    glBindVertexArray(vao_);
  }
  void SquareTexturedMesh::bindTexCoordData() const {
    glBindBuffer(GL_ARRAY_BUFFER, texcoord_buff_);
  }

  void SquareTexturedMesh::regenerateBuffers()
  {
    genVertBuffer();
    genFaceBuffer();
  }

  void SquareTexturedMesh::ReInit()
  {
    glBindVertexArray(vao_);
    bindVertexData();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    bindTexCoordData();
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    bindFaceData();
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenBuffers(1, &texcoord_buff_);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord_buff_);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(TexCoord), texcoords_, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void SquareTexturedMesh::genVertBuffer() {
    // put the vertex data on the graphics card
    glGenBuffers(1, &vert_buff_);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buff_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts_), verts_, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void SquareTexturedMesh::genFaceBuffer() {
    // put the face data on the graphics card
    glGenBuffers(1, &face_buff_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_buff_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces_), faces_, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  void SquareTexturedMesh::getSize(Graphics::Size* size) const {
    if(size) {
      *size = size_;
    }
  }

  Graphics::Size const& SquareTexturedMesh::getSize() const {
    return size_;
  }
  void SquareTexturedMesh::setSize(const Graphics::Size& size) {
    size_ = size;
  }
  void SquareTexturedMesh::setUV(const TexCoord* texcoords) {
    // put the uv coordinates in the object and on the graphics card
    memcpy(texcoords_, texcoords, sizeof(texcoords_));
    glBindBuffer(GL_ARRAY_BUFFER, texcoord_buff_);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(TexCoord), texcoords_, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(!vao_) glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
      bindVertexData();
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
      glEnableVertexAttribArray(0);
      bindTexCoordData();
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TexCoord), 0);
      glEnableVertexAttribArray(1);
      bindFaceData();
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}