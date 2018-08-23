/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Scan Conversion.h
Purpose: This file contains function prototypes of the functions
inside of the Scan Conversion.cpp
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_7
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 04/18/2017
- End Header --------------------------------------------------------*/
#pragma once

#include "Structures.h"

void DrawHorizontalLine(const Point &p1, const Point &p2, unsigned char r, unsigned char g, unsigned char b);
void DrawVerticalLine(const Point &p1, const Point &p2, unsigned char r, unsigned char g, unsigned char b);
void DrawDiagonalLine(const Point &p1, const Point &p2, unsigned char r, unsigned char g, unsigned char b);
void DrawNaiveLine(const Point &p1, const Point &p2, unsigned char r, unsigned char g, unsigned char b);

void DrawMidpointLine(const Point &p1, const Point &p2, unsigned char r, unsigned char g, unsigned char b);

void DrawCircleEightWaySymmetric(const int &x, const int &y, const int &xC, const int &yC, unsigned char r, unsigned char g, unsigned char b);
void DrawMidpointCircle(const Point &p, const float &R, unsigned char r, unsigned char g, unsigned char b);
void DrawEllipseFourWaySymmetric(const int &x, const int &y, const int &xC, const int &yC, unsigned char r, unsigned char g, unsigned char b);
void DrawMidpointEllipse(const Point &p, const float &A, const float &B, unsigned char r, unsigned char g, unsigned char b);

void DrawTriangleTopLeftFillRule(const Point &p1, const Point &p2, const Point &p3, unsigned char *texture = NULL, int mode = 0);
