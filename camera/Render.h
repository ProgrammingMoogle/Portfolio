// Render.h
// -- simple rendering using OpenGL
// cs250 1/18

#ifndef CS250_RENDER_H
#define CS250_RENDER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "Affine.h"


class Render {
  public:
    Render(void);
    ~Render(void);
    void clearBuffers(const Vector& c);
    void setColor(const Vector& c);
    void drawLine(const Hcoord& P, const Hcoord& Q);
    void fillTriangle(const Hcoord& P, const Hcoord& Q, const Hcoord& R);
  private:
    GLint program;
    GLint ucolor, aposition;
};


#endif

