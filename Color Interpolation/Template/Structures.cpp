/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Structures.cpp
Purpose: This file contains the functions for overloaded
operators to handle Points as well as Identity, Translate,
Scale, and Rotate Matrices and operation involing Matrices.
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_7
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 04/18/2017
- End Header --------------------------------------------------------*/
#include "Structures.h"

Point operator - (const Point &p)
{
	return Point(-p.x, -p.y);
}

Point operator + (const Point &p1, const Point &p2)
{
	return Point(p1.x + p2.x, p1.y + p2.y);
}

Point operator - (const Point &p1, const Point &p2)
{
	return Point(p1.x - p2.x, p1.y - p2.y);
}

Point operator * (const Point &p1, const Point &p2)
{
	return Point(p1.x * p2.x, p1.y * p2.y);
}

Point operator * (const Point &p, const float &f)
{
	return Point(p.x * f, p.y * f);
}

Point operator * (const float &f, const Point &p)
{
	return (p * f);
}

float Dot(const Point &p1, const Point &p2)
{
	return (p1.x * p2.x + p1.y * p2.y);
}

float Length(const Point &p)
{
	return sqrtf(p.x * p.x + p.y * p.y);
}

/////

Point3D operator * (const Point3D &p1, const Point3D &p2)
{
	Point3D result;

	result.x = p1.y * p2.z - p1.z * p2.y;
	result.y = p1.z * p2.x - p1.x * p2.z;
	result.z = p1.x * p2.y - p1.y * p2.x;

	return result;
}

/////

Matrix Translate(const Point &p)
{
	Matrix m;

	m.m[0][0] = 1.0f;		m.m[0][1] = 0.0f;		m.m[0][2] = p.x;
	m.m[1][0] = 0.0f;		m.m[1][1] = 1.0f;		m.m[1][2] = p.y;
	m.m[2][0] = 0.0f;		m.m[2][1] = 0.0f;		m.m[2][2] = 1.0f;

	return m;
}

Matrix Rotate(const float &alpha)
{
	Matrix m;

	m.m[0][0] = cosf(alpha);		m.m[0][1] = -sinf(alpha);		m.m[0][2] = 0.0f;
	m.m[1][0] = sinf(alpha);		m.m[1][1] = cosf(alpha);		m.m[1][2] = 0.0f;
	m.m[2][0] = 0.0f;				m.m[2][1] = 0.0f;				m.m[2][2] = 1.0f;

	return m;
}

Matrix Scale(const Point &s)
{
	Matrix m;

	m.m[0][0] = s.x;		m.m[0][1] = 0.0f;		m.m[0][2] = 0.0f;
	m.m[1][0] = 0.0f;		m.m[1][1] = s.y;		m.m[1][2] = 0.0f;
	m.m[2][0] = 0.0f;		m.m[2][1] = 0.0f;		m.m[2][2] = 1.0f;

	return m;
}

Matrix operator * (const Matrix &m1, const Matrix &m2)
{
	Matrix result;

	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0];
	result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1];
	result.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2];

	result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0];
	result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1];
	result.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2];

	result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0];
	result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1];
	result.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2];

	return result;
}

Point operator * (const Matrix &m, const Point &p)
{
	Point result;

	result.x = m.m[0][0] * p.x + m.m[0][1] * p.y + m.m[0][2];
	result.y = m.m[1][0] * p.x + m.m[1][1] * p.y + m.m[1][2];
	float w = m.m[2][0] * p.x + m.m[2][1] * p.y + m.m[2][2];

	result.x /= w;
	result.y /= w;

	return result;
}