// TextureRender.h
// -- rendering textured 2D meshes
// cs200 9/17

#ifndef CS200_TEXTURERENDER_H
#define CS200_TEXTURERENDER_H


#include <GL/glew.h>
#include <GL/gl.h>
#include "TexturedMesh.h"
#include "Affine.h"


class TextureRender {
  public:
    TextureRender(void);
    ~TextureRender(void);
    void clearFrame(const Hcoord& c);
    void loadTexture(unsigned char *rgbdata, int width, int height);
    void unloadTexture(void);
    void setModelToWorld(const Affine &M);
    void loadMesh(TexturedMesh &m);
    void unloadMesh(void);
    void displayFaces(void);
  private:
    GLuint program,
           texture_buffer,
           vertex_buffer,
           texcoord_buffer,
           face_buffer;

    GLint utransform,
          aposition,
          atexcoord;
    int mesh_face_count;
};

#endif

