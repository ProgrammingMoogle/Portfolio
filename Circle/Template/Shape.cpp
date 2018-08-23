/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Shape.cpp
Purpose: This file contains the functions for Shapes.
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_5
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 03/20/2017
- End Header --------------------------------------------------------*/
#include "Shape.h"

Shape::Shape()
{
}

void Shape::Update()
{
	Matrix tr = Translate(position);
	Matrix ro = Rotate(rotAngle * DEG_TO_RAD);
	Matrix sc = Scale(scale);

	Matrix m = tr * ro * sc;

	//PrintModelTransform(&tr.m[0][0], &ro.m[0][0], &sc.m[0][0], &m.m[0][0]);
	
	transformed.clear();
	for (unsigned int i=0; i<points.size(); ++i)
	{
		transformed.push_back(Pipeline::Update(m * points[i]));
	}
}

void Shape::Render()
{
	for (unsigned int i=0; i<transformed.size()-1; ++i)
		DrawNaiveLine(transformed[i], transformed[i+1], color.r, color.g, color.b);
	DrawNaiveLine(transformed[transformed.size()-1], transformed[0], color.r, color.g, color.b);
}