// Affine.h
// -- 2D point/vector/transformation package
// cs200 8/17

#ifndef CS200_AFFINE_H
#define CS200_AFFINE_H

#include <cmath>
#include <cassert>	


struct Hcoord {
  float x, y, w;
  explicit Hcoord(float X=0, float Y=0, float W=0);
  float& operator[](int i) { return *(&x+i); }
  float operator[](int i) const { return *(&x+i); }
  static bool near(float x, float y) { return std::abs(x-y) < 1e-5f; }
};


struct Point : Hcoord {
  explicit Point(float X=0, float Y=0);
  Point(const Hcoord& v) : Hcoord(v) { assert(near(w,1)); }
};


struct Vector : Hcoord {
  explicit Vector(float X=0, float Y=0);
  Vector(const Hcoord& v) : Hcoord(v) { assert(near(w,0)); }
};


struct Affine {
  Hcoord row[3];
  Affine(void);
  Affine(const Vector& Lx, const Vector& Ly, const Point& disp);
  Hcoord& operator[](int i) { return row[i]; }
  const Hcoord& operator[](int i) const { return row[i]; }
};


Hcoord operator+(const Hcoord& u, const Hcoord& v);
Hcoord operator-(const Hcoord& u, const Hcoord& v);
Hcoord operator-(const Hcoord& v);
Hcoord operator*(float r, const Hcoord& v);
Hcoord operator*(const Affine& A, const Hcoord& v);
Affine operator*(const Affine& A, const Affine& B);
float dot(const Vector& u, const Vector& v);
float abs(const Vector& v);
Vector normalize(const Vector& v);
Affine rotate(float t);
Affine translate(const Vector& v);
Affine scale(float r);
Affine scale(float rx, float ry);

// Note that this function is NOT implemented in assignment #1:
Affine inverse(const Affine& A);


#endif

