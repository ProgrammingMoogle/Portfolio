/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Structures.h
Purpose: This file contains function prototypes of the functions
inside of the Structures.cpp
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_7
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 04/18/2017
- End Header --------------------------------------------------------*/
#pragma once

#include "Utilities.h"

struct Color
{
	float r, g, b;

	Color (float _r = 0.0f, float _g = 0.0f, float _b = 0.0f)
	{
		r = _r;
		g = _g;
		b = _b;
	}
};

/////

typedef struct Point
{
	float x, y;
	Color color;
	float u, v;

	Point (float _x = 0.0f, float _y = 0.0f, Color _c = Color(), float _u = 0.0f, float _v = 0.0f)
	{
		x = _x;
		y = _y;
		color = _c;
		u = _u;
		v = _v;
	}
} Vector;

Point operator - (const Point &p);
Point operator + (const Point &p1, const Point &p2);
Point operator - (const Point &p1, const Point &p2);
Point operator * (const Point &p1, const Point &p2);
Point operator * (const Point &p, const float &f);
Point operator * (const float &f, const Point &p);

float Dot(const Point &p1, const Point &p2);
float Length(const Point &p);

/////

struct Point3D
{
	float x, y, z;

	Point3D (float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

Point3D operator * (const Point3D &p1, const Point3D &p2);

/////

struct Matrix
{
	float m[3][3];

	Matrix()
	{
		m[0][0] = 1;	m[0][1] = 0;	m[0][2] = 0;
		m[1][0] = 0;	m[1][1] = 1;	m[1][2] = 0;
		m[2][0] = 0;	m[2][1] = 0;	m[2][2] = 1;
	}
};

Matrix Translate(const Point &p);
Matrix Rotate(const float &alpha);
Matrix Scale(const Point &s);

Matrix operator * (const Matrix &m1, const Matrix &m2);
Point operator * (const Matrix &m, const Point &p);

/////

struct Line
{
	Point p1;
	Point p2;
};

/////

struct Circle
{
	Point c;
	float r;
};

/////

struct Ellipses
{
	Point c;
	float a;
	float b;
};

/////

struct Triangle
{
	Point p1;
	Point p2;
	Point p3;
};

/////

struct Quad
{
	Point p1;
	Point p2;
	Point p3;
	Point p4;
};