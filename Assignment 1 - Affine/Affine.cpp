#include "Affine.h"

Hcoord::Hcoord(float X, float Y, float W)
{
	Hcoord::x = X;
	Hcoord::y = Y;
	Hcoord::w = W;
}

Point::Point(float X, float Y)
{
	Point::x = X;
	Point::y = Y;
	Point::w = 1.0;
}

Vector::Vector(float X, float Y)
{
	Vector::x = X;
	Vector::y = Y;
	Vector::w = 0.0;
}

Vector normalize(const Vector & v)
{
  Vector norm;

  float normv;

  normv = float(sqrt(((v.x * v.x) + (v.y * v.y))));

  norm.x = (v.x / normv);
  norm.y = (v.y / normv);
  norm.w = 0;

  return norm;
}

Affine::Affine(void)
{
  row[0].x = 0.0;
  row[0].y = 0.0;
  row[0].w = 0.0;

  row[1].x = 0.0;
  row[1].y = 0.0;
  row[1].w = 0.0;

  row[2].x = 0.0;
  row[2].y = 0.0;
  row[2].w = 1.0;
}

Affine::Affine(const Vector & Lx, const Vector & Ly, const Point & disp)
{
  row[0].x = Lx.x;
  row[0].y = Ly.x;
  row[0].w = disp.x;

  row[1].x = Lx.y;
  row[1].y = Ly.y;
  row[1].w = disp.y;

  row[2].x = Lx.w;
  row[2].y = Ly.w;
  row[2].w = disp.w;
}

Hcoord operator+(const Hcoord & u, const Hcoord & v)
{
	Hcoord sum;

	sum.x = u.x + v.x;
	sum.y = u.y + v.y;
	sum.w = u.w + v.w;

	return sum;
}

Hcoord operator-(const Hcoord & u, const Hcoord & v)
{
	Hcoord diff;

	diff.x = u.x - v.x;
	diff.y = u.y - v.y;
	diff.w = u.w - v.w;

	return diff;
}

/*Negates the Hcoord*/
Hcoord operator-(const Hcoord & v)
{
	Hcoord diff = v;

	diff.x *= -1;
	diff.y *= -1;
  diff.w *= -1;

	return diff;
}

Hcoord operator*(float r, const Hcoord & v)
{
	Hcoord product;

	product.x = r * v.x;
	product.y = r * v.y;
	product.w = r * v.w;

	return product;
}

Hcoord operator*(const Affine & A, const Hcoord & v)
{
	Hcoord product;

  product.x = (A[0].x * v.x) + (A[0].y * v.y) + (A[0].w * v.w);
  product.y = (A[1].x * v.x) + (A[1].y * v.y) + (A[1].w * v.w);
  product.w = (A[2].x * v.x) + (A[2].y * v.y) + (A[2].w * v.w);

	return product;
}

float dot(const Vector & u, const Vector & v)
{
  float dotprod = (u.x * v.x) + (u.y * v.y) + (u.w * v.w);
  return dotprod;
}

Affine operator*(const Affine & A, const Affine & B)
{
	Affine product;

  //First Row
  product[0].x = (A[0].x * B[0].x) + (A[0].y * B[1].x) + (A[0].w * B[2].x);
  product[0].y = (A[0].x * B[0].y) + (A[0].y * B[1].y) + (A[0].w * B[2].y);
  product[0].w = (A[0].x * B[0].w) + (A[0].y * B[1].w) + (A[0].w * B[2].w);
   
  //Second Row
  product[1].x = (A[1].x * B[0].x) + (A[1].y * B[1].x) + (A[1].w * B[2].x);
  product[1].y = (A[1].x * B[0].y) + (A[1].y * B[1].y) + (A[1].w * B[2].y);
  product[1].w = (A[1].x * B[0].w) + (A[1].y * B[1].w) + (A[1].w * B[2].w);
    

  //Third Row
  product[2].x = (A[2].x * B[0].x) + (A[2].y * B[1].x) + (A[2].w * B[2].x);
  product[2].y = (A[2].x * B[0].y) + (A[2].y * B[1].y) + (A[2].w * B[2].y);
  product[2].w = (A[2].x * B[0].w) + (A[2].y * B[1].w) + (A[2].w * B[2].w);

	return product;
}



float abs(const Vector & v)
{
	float len = ((float)sqrt((v.x * v.x) + (v.y * v.y)));
	return len;
}

Affine rotate(float t)
{
	Affine rot;

	rot[0].x = float(cos(t));
	rot[0].y = float(-sin(t));
	rot[0].w = 0.0;

	rot[1].x = float(sin(t));
	rot[1].y = float(cos(t));
	rot[1].w = 0.0;

	rot[2].x = 0.0;
	rot[2].y = 0.0;
	rot[2].w = 1.0;

	return rot;
}

Affine translate(const Vector & v)
{
	Affine trans;

  trans[0].x = 1.0;
  trans[0].w = v.x;
  
  trans[1].y = 1.0;
  trans[1].w = v.y;
  trans[2].w = 1.0;

	return trans;
}


Affine scale(float r)
{
	Affine scalar;

	scalar[0][0] = r;
	scalar[0][1] = 0.0;
	scalar[0][2] = 0.0;

	scalar[1][0] = 0.0;
	scalar[1][1] = r;
	scalar[1][2] = 0.0;

	scalar[2][0] = 0.0;
	scalar[2][1] = 0.0;
	scalar[2][2] = 1.0;

	return scalar;
}

Affine scale(float rx, float ry)
{
	Affine scalar;

	scalar[0][0] = rx;
	scalar[0][1] = 0.0;
	scalar[0][2] = 0.0;

	scalar[1][0] = 0.0;
	scalar[1][1] = ry;
	scalar[1][2] = 0.0;

	scalar[2][0] = 0.0;
	scalar[2][1] = 0.0;
	scalar[2][2] = 1.0;

	return scalar;
}


