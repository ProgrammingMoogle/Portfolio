/**
  \file   Mesh.hpp
  \author Samuel Cook

  \brief Encapsulates an OpenGL Mesh.

  

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _MESH_HPP_
#define _MESH_HPP_

#include "GraphicsTypes.hpp"

typedef unsigned int GLuint;

namespace Graphics {
  struct Face
  {
    constexpr Face(unsigned i1, unsigned i2, unsigned i3) : ind1(i1), ind2(i2), ind3(i3) {}
    unsigned ind1, ind2, ind3;
  };

  /**
    \brief The 
  */  
  class SquareTexturedMesh {
    public:
      SquareTexturedMesh(const TexCoord* texcoords, float width = 1, float height = 1);
	  SquareTexturedMesh(const SquareTexturedMesh& o);
	  SquareTexturedMesh(SquareTexturedMesh&& o);
      ~SquareTexturedMesh();
      static void bindVertexData();
      static void bindFaceData();
      void bindVertexArray() const;
      void bindTexCoordData() const;
      void getSize(Graphics::Size* size) const;
      Graphics::Size const& getSize() const;
      void setSize(const Graphics::Size& size);
      void setUV(const TexCoord* texcoords);
      static void regenerateBuffers();
      void ReInit();

    private:
      static GLuint vert_buff_;
      static GLuint face_buff_;
      GLuint texcoord_buff_ = 0;

      static constexpr Vertex verts_[4] = {
        { -0.5f,  0.5f,  0.0f },
        { 0.5f,  0.5f,  0.0f },
        { 0.5f, -0.5f,  0.0f },
        { -0.5f, -0.5f,  0.0f }
      };

      static constexpr Face faces_[2] = {
        { 0, 1, 2 },
        { 2, 3, 0 }
      };

      GLuint vao_ = 0;

      TexCoord texcoords_[4];
      Graphics::Size size_;


    private:
      static void genVertBuffer();
      static void genFaceBuffer();
  };
}

#endif