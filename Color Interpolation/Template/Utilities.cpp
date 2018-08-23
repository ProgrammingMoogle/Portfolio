/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Utilities.h
Purpose: This file contains function prototypes of the functions
inside of the Utilities.cpp
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_7
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 04/18/2017
- End Header --------------------------------------------------------*/
#include "Utilities.h"

unsigned char *FrameBuffer::buffer = NULL;
int FrameBuffer::width = 0;
int FrameBuffer::height = 0;

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
}

//Clears the frame buffer to the given (r, g, b) color
void FrameBuffer::Clear(const unsigned char &r, const unsigned char &g, const unsigned char &b)
{
	for (int i=0; i<width; ++i)
	{
		for (int j=0; j<height; ++j)
		{
			buffer[i * 3 + j * width * 3] = r;
			buffer[i * 3 + j * width * 3 + 1] = g;
			buffer[i * 3 + j * width * 3 + 2] = b;
		}
	}
}

//Initializes the frame buffer
void FrameBuffer::Init(const unsigned int &w, const unsigned int &h)
{
	width = w;
	height = h;
	buffer = new unsigned char [width * height * 3];

	Clear(255, 255, 255);
}

//Set the pixel's color at (x,y)
void FrameBuffer::SetPixel(const int &x, const int &y, const unsigned char &r, const unsigned char &g, const unsigned char &b)
{
	int _y = height - y;

	if (x < 0 || x > width || _y < 0 || _y > height)
		return;

	buffer[x * 3 + _y * width * 3] = r;
	buffer[x * 3 + _y * width * 3 + 1] = g;
	buffer[x * 3 + _y * width * 3 + 2] = b;
}

//Stores the color of the given (x,y) pixel in r, g & b
void FrameBuffer::GetPixel(const int &x, const int &y, unsigned char &r, unsigned char &g, unsigned char &b)
{
	if (x < 0 || x > width || y < 0 || y > height)
		return;

	r = buffer[x * 3 + y * width * 3];
	g = buffer[x * 3 + y * width * 3 + 1];
	b = buffer[x * 3 + y * width * 3 + 2];
}

/////

int Ceiling(const float &p)
{
	return ceil(p);
}

int Floor(const float &p)
{
	return floor(p);
}

int Truncate(const float &p) 
{ 
     return Floor(p);
}

int Round(const float &p)
{
	return Floor(p + 0.5f);
}

unsigned char Clamp(float f)
{
	if (f < 0.0f)
		return 0;
	if (f > 255.0f)
		return 255;

	return (unsigned char)f;
}

/////

unsigned char *CreateChessBoardImage()
{
	unsigned char * chessBoard = (unsigned char *)::operator new(128 * 128 * 4);
	bool black = false;

	for(int j = 0; j < 128; j++)
	{
		if(j % 32 == 0)
			black = !black;

		for(int i = 0; i < 128; i++)
		{
			if(i % 32 == 0)
                black = !black;
					           
			*(chessBoard + (j * 128 * 4) + (i * 4) +  0) = black ? 0 : 255;
			*(chessBoard + (j * 128 * 4) + (i * 4) +  1) = black ? 0 : 255;
			*(chessBoard + (j * 128 * 4) + (i * 4) +  2) = black ? 0 : 255;
			*(chessBoard + (j * 128 * 4) + (i * 4) +  3) = 128;
		}
	}

	return chessBoard;
};