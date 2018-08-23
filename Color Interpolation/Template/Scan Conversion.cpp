/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Scan Conversion.cpp
Purpose: This file contains the functions for drawing various lines :
Vertical, Horizontal, Diagonal, and Naive Lines.
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_7
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 04/17/2017
- End Header --------------------------------------------------------*/
#include "Scan Conversion.h"

void DrawHorizontalLine(const Point &p1, const Point &p2, unsigned char r, unsigned char g, unsigned char b)
{
  int dx = (p2.x - p1.x >= 0) ? 1 : -1;
  int start = Round((dx == 1) ? p1.x : p2.x);
  int end = Round((dx == 1) ? p2.x : p1.x);
  int y = Round(p1.y);

  for (int x = start; x <= end; ++x)
    FrameBuffer::SetPixel(x, y, r, g, b);
}

void DrawVerticalLine(const Point &p1, const Point &p2, unsigned char r, unsigned char g, unsigned char b)
{
  int dy = (p2.y - p1.y >= 0) ? 1 : -1;
  int start = Round((dy == 1) ? p1.y : p2.y);
  int end = Round((dy == 1) ? p2.y : p1.y);
  int x = Round(p1.x);

  for (int y = start; y <= end; ++y)
    FrameBuffer::SetPixel(x, y, r, g, b);
}

void DrawDiagonalLine(const Point &p1, const Point &p2, unsigned char r, unsigned char g, unsigned char b)
{
  int x = Round(p1.x); //a rounded x-coordinate
  int tri_x = Round(p2.x) - x; //delta x
  int xstep = (tri_x >= 0) ? 1 : -1; // xstep
  tri_x = (tri_x >= 0) ? tri_x : -tri_x;

  int y = Round(p1.y);
  int tri_y = Round(p2.y) - y;
  int ystep = (tri_y >= 0) ? 1 : -1;
  tri_y = (tri_y >= 0) ? tri_y : -tri_y;

  //finds the slope of the line
  float m = (p2.y - p1.y) / (p2.x - p1.x);
  //finds the y_intercept
  float y_inter = p1.y - m * p1.x;

  //Compares x and y to use specific equations
  if (x > y)
  {
    FrameBuffer::SetPixel(x, Round(m*x + y_inter), r, g, b);
    while (tri_x--)
    {
      x += xstep;
      FrameBuffer::SetPixel(x, Round(m*x + y_inter), r, g, b);
    }
  }
  else
  {
    FrameBuffer::SetPixel(Round((y - y_inter) * 1 / m), y, r, g, b);
    while (tri_y--)
    {
      y += ystep;
      FrameBuffer::SetPixel(Round((y - y_inter) * 1 / m), y, r, g, b);
    }
  }
}

void DrawNaiveLine(const Point &p1, const Point &p2, unsigned char r, unsigned char g, unsigned char b)
{
  //This should happen first because both x-coordinates is 0, then 
  //it is best to prevent the chance of dividing by 0 when finding 
  //the slope by having this being checked first
  if (Round(p1.x) == Round(p2.x))
  {
    DrawVerticalLine(p1, p2, r, g, b);
    return;
  }

  //finds the slope of the line
  float m = (p2.y - p1.y) / (p2.x - p1.x);
  //finds the y_intercept
  float y_inter = p1.y - m * p1.x;

  int x = Round(p1.x); //a rounded x-coordinate
  int tri_x = Round(p2.x) - x; //delta x
  int xstep = (tri_x >= 0) ? 1 : -1; // xstep
  tri_x = (tri_x >= 0) ? tri_x : -tri_x;

  int y = Round(p1.y);
  int tri_y = Round(p2.y) - y;
  int ystep = (tri_y >= 0) ? 1 : -1;
  tri_y = (tri_y >= 0) ? tri_y : -tri_y;

  //checks if the slope is 0 then it is a horizontal line
  if (m == 0)
  {
    DrawHorizontalLine(p1, p2, r, g, b);
    return;
  }

  //checks if the absolute value of slope is 1
  //then it is a diagonal line
  else if (abs(m) == 1)
  {
    DrawDiagonalLine(p1, p2, r, g, b);
    return;
  }
  // in the chance it is none of the previous three then,
  //it draws a naive line
  FrameBuffer::SetPixel(x, Round(m*x + y_inter), r, g, b);
  while (tri_x--)
  {
    x += xstep;
    FrameBuffer::SetPixel(x, Round(m*x + y_inter), r, g, b);
  }
}

void DrawMidpointLine(const Point &p1, const Point &p2, unsigned char r, unsigned char g, unsigned char b)
{
  int x = Round(p1.x);
  int y = Round(p1.y);
  int hor = 0;
  int dia = 0;
  int dp = 0;
  FrameBuffer::SetPixel(x, y, r, g, b);
  //FrameBuffer::SetPixel(Round(p2.x), Round(p2.y), r, g, b);
  //the delta of x and y
  int tri_x = Round(p2.x) - Round(p1.x);
  int tri_y = Round(p2.y) - Round(p1.y);



  //Steps of x and y
  int xstep = (tri_x >= 0) ? 1 : -1;
  int ystep = (tri_y >= 0) ? 1 : -1;

  //makes it positive if delta x or delta y are negative
  if (xstep < 1)
  {
    tri_x *= xstep;
  }
  if (ystep < 1)
  {
    tri_y *= ystep;
  }
  //determines if it is horizontal or a diagonal line
  int count = 0;
  // m is greater than 1
  if (tri_x < tri_y)
  {
    count = tri_y;
    hor = (2 * tri_x);
    dia = (2 * tri_x) - (2 * tri_y);
    dp = (2 * tri_x) - tri_y;
    while (count--)
    {
      //go up
      if (dp < 0)
      {
        y += ystep;
        dp += hor;
      }
      //go diagonal
      else
      {
        x += xstep;
        y += ystep;
        dp += dia;
      }
      FrameBuffer::SetPixel(x, y, r, g, b);
    }
  }
  // m is less than 1
  else
  {
    count = tri_x;
    hor = (2 * tri_y);
    dia = (2 * tri_y) - (2 * tri_x);
    dp = (2 * tri_y) - tri_x;
    while (count--)
    {
      //go up
      if (dp < 0)
      {
        x += xstep;
        dp += hor;
      }
      //go diagonal
      else
      {
        x += xstep;
        y += ystep;
        dp += dia;
      }
      FrameBuffer::SetPixel(x, y, r, g, b);
    }
  }
}

/////

void DrawCircleEightWaySymmetric(const int &x, const int &y, const int &xC, const int &yC, unsigned char r, unsigned char g, unsigned char b)
{
  FrameBuffer::SetPixel(Round(x + xC), Round(-y + yC), r, g, b);
  FrameBuffer::SetPixel(Round(x + xC), Round(y + yC), r, g, b);
  FrameBuffer::SetPixel(Round(-x + xC), Round(-y + yC), r, g, b);
  FrameBuffer::SetPixel(Round(-x + xC), Round(y + yC), r, g, b);
  FrameBuffer::SetPixel(Round(y + xC), Round(-x + yC), r, g, b);
  FrameBuffer::SetPixel(Round(y + xC), Round(x + yC), r, g, b);
  FrameBuffer::SetPixel(Round(-y + xC), Round(-x + yC), r, g, b);
  FrameBuffer::SetPixel(Round(-y + xC), Round(x + yC), r, g, b);
}

void DrawMidpointCircle(const Point &p, const float &R, unsigned char r, unsigned char g, unsigned char b)
{
  int x = 0;
  int y = R;
  //decision parameter
  int dp = 1 - R;
  //decision parameter straight
  int dps = 3;
  //decision parameter diagonal
  int dpD = -2 * y + 5;
  //how far the x direction will go
  //the one comes from sqrt of 2 / 2 since if rounded properly would be 1
  int xdir = Round(y * sqrt(2) / 2);
  //goes straight
  int straight = 2;
  //goes straight-diagonal
  int strdia = 2;
  //goes diagonal-straight
  int diastr = 2;
  //goes diagonal-diagonal
  int diadia = 4;

  DrawCircleEightWaySymmetric(x, y, p.x, p.y, r, g, b);

  for (x = 0; x < xdir; x)
  {
    // forward
    if (dp < 0)
    {
      DrawCircleEightWaySymmetric(x += 1, y, p.x, p.y, r, g, b);
      dp += dps;
      dps += straight;
      dpD += diastr;
    }
    // Diagonal
    else if (dp > 0)
    {
      DrawCircleEightWaySymmetric(x += 1, y -= 1, p.x, p.y, r, g, b);
      dp += dpD;
      dps += strdia;
      dpD += diadia;
    }
    //Go straight other wise
    else if (dp == 0)
    {
      DrawCircleEightWaySymmetric(x += 1, y, p.x, p.y, r, g, b);
      dp += dps;
      dps += straight;
      dpD += diastr;
    }
  }
}

void DrawEllipseFourWaySymmetric(const int &x, const int &y, const int &xC, const int &yC, unsigned char r, unsigned char g, unsigned char b)
{
  FrameBuffer::SetPixel(x + xC, y + yC, r, g, b);
  FrameBuffer::SetPixel(-x + xC, y + yC, r, g, b);
  FrameBuffer::SetPixel(x + xC, -y + yC, r, g, b);
  FrameBuffer::SetPixel(-x + xC, -y + yC, r, g, b);
}

void DrawMidpointEllipse(const Point &p, const float &A, const float &B, unsigned char r, unsigned char g, unsigned char b)
{
  int x = 0;
  int y = B;

  int dpX = -((A*A) * B) + B*B + ((A*A) / 4);
  int dpXS = (B*B)*(2 * x + 3);
  int dpXSE = (B*B)*(2 * x + 3) + 2 * (A*A)*(-y + 1);
  int xdirection = (A*A) / sqrt((A*A) + (B*B));

  //handles when S is chosen in section 1
  //used to update dpXS and dpXSE
  //it's only used to update dpXS when SE is chosen
  int deltaSA = 2 * (B*B);

  //used for when SE is chosen in either section 1 or section 2
  //used to update dpXSE or dpYSE only when SE is chosen
  int deltaSE = 2 * (B*B) + 2 * (A*A);

  //handles when S is chosen in section 2
  int deltaSB = 2 * (A*A);

  DrawEllipseFourWaySymmetric(x, y, p.x, p.y, r, g, b);

  for (x; x <= xdirection; ++x)
  {
    //E
    if (dpX < 0)
    {
      DrawEllipseFourWaySymmetric(x, y, p.x, p.y, r, g, b);
      dpX += dpXS;
      dpXS += deltaSA;
      dpXSE += deltaSA;
    }
    //SE
    else
    {
      DrawEllipseFourWaySymmetric(x, y -= 1, p.x, p.y, r, g, b);
      dpX += dpXSE;
      dpXS += deltaSA;
      dpXSE += deltaSE;
    }
  }


  int ydirection = (B*B) / sqrt((A*A) + (B*B));

  //Realligns the circle
  y--;
  x--;

  int dpY = (B*B)*x - 2 * ((A*A)*y) + (A*A) + ((B*B) / 4);
  int dpYS = A*A*(-2 * y + 3);
  int dpYSE = 2 * (B*B) *(x + 1) + A*A*(-2 * y + 3);

  for (y; y >= 0; --y)
  {
    //S
    if (dpY > 0)
    {
      DrawEllipseFourWaySymmetric(x, y, p.x, p.y, r, g, b);
      dpY += dpYS;
      dpYS += deltaSB;
      dpYSE += deltaSB;
    }
    //SE
    else
    {
      DrawEllipseFourWaySymmetric(x += 1, y, p.x, p.y, r, g, b);
      dpY += dpYSE;
      dpYS += deltaSB;
      dpYSE += deltaSE;
    }
  }
}

void DrawTriangleTopLeftFillRule(const Point &p1, const Point &p2, const Point &p3, unsigned char *texture, int mode)
{
  //P0 - TOP
  Point TOP = p1;
  //P1 - MIDDLE
  Point MID = p2;
  //P2 - BOTTOM
  Point BOT = p3;
  bool isLeft = false;
  int LeftEdge = 0;
  int RightEdge = 0;

  //int edge[2] = {0};

  //CASES 4,5,6
  if (p1.y < p2.y)
  {
    // CASE 4
    if (p3.y < p1.y)
    {
      TOP = p3;
      MID = p1;
      BOT = p2;
      isLeft = true;
      LeftEdge = 1;
      RightEdge = 0;
    }
    else
    {
      //CASE 5
      if (p2.y < p3.y)
      {
        TOP = p1;
        MID = p2;
        BOT = p3;
        isLeft = true;
        LeftEdge = 1;
        RightEdge = 0;
      }
      //CASE 6
      else
      {
        TOP = p1;
        MID = p3;
        BOT = p2;
        isLeft = false;
        LeftEdge = 0;
        RightEdge = 1;
      }
    }
  }
  else
  {
    //CASE 2
    if (p3.y < p2.y)
    {
      TOP = p3;
      MID = p2;
      BOT = p1;
      isLeft = false;
      LeftEdge = 0;
      RightEdge = 1;
    }
    else
    {
      //CASE 3
      if (p1.y < p3.y)
      {
        TOP = p2;
        MID = p1;
        BOT = p3;
        isLeft = false;
        LeftEdge = 0;
        RightEdge = 1;
      }
      //CASE 1
      else
      {
        TOP = p2;
        MID = p3;
        BOT = p1;
        isLeft = true;
        LeftEdge = 1;
        RightEdge = 0;
      }
    }
  }

  //[0] Top to Bottom
  //[1] Top to Middle
  //[2] Middle to Bottom
  float InSlope[3] = { 0 };

  //InSlope Top to Bot
  float dy = (BOT.y - TOP.y);
  if (dy != 0)
  {
    InSlope[0] = (BOT.x - TOP.x) / dy;
  }

  //Top to Mid
  dy = (MID.y - TOP.y);
  if (dy != 0)
  {
    InSlope[1] = (MID.x - TOP.x) / dy;
  }

  //Mid to Bot
  dy = (BOT.y - MID.y);
  if (dy != 0)
  {
    InSlope[2] = (BOT.x - MID.x) / dy;
  }

  float MaxX = TOP.x;
  float MinX = TOP.x;
  int j = 0;
  int i = 0;

  //VECTORS - NORMALIZES TOP TO MIDDLE 
  Vector v1;
  v1.x = (MID.x - TOP.x);
  v1.y = (MID.y - TOP.y);
  v1.color.r = (MID.color.r - TOP.color.r);
  v1.color.g = (MID.color.g - TOP.color.g);
  v1.color.b = (MID.color.b - TOP.color.b);
  v1.u = MID.u - TOP.u;
  v1.v = MID.v - TOP.v;

  //NORMALIZES TOP TO BOTTOM
  Vector v2;
  v2.x = (BOT.x - TOP.x);
  v2.y = (BOT.y - TOP.y);
  v2.color.r = (BOT.color.r - TOP.color.r);
  v2.color.g = (BOT.color.g - TOP.color.g);
  v2.color.b = (BOT.color.b - TOP.color.b);
  v2.u = BOT.u - TOP.u;
  v2.v = BOT.v - TOP.v;

  //RED - Normalize
  float aR = -((v1.y * v2.color.r) - (v1.color.r * v2.y));
  float bR = -((v1.color.r * v2.x) - (v1.x * v2.color.r));
  float cR = (v1.x * v2.y) - (v1.y * v2.x);

  //GREEN - Normalize
  float aG = -((v1.y * v2.color.g) - (v1.color.g * v2.y));
  float bG = -((v1.color.g * v2.x) - (v1.x * v2.color.g));
  float cG = (v1.x * v2.y) - (v1.y * v2.x);

  //BLUE - Normalize
  float aB = -((v1.y * v2.color.b) - (v1.color.b * v2.y));
  float bB = -((v1.color.b * v2.x) - (v1.x * v2.color.b));
  float cB = (v1.x * v2.y) - (v1.y * v2.x);

  //U - Normal
  float aU = -((v1.y * v2.u) - (v1.u * v2.y)); // goes across x by canceling out the y
  float bU = -((v1.u * v2.x) - (v1.x * v2.u)); // goes across y by canceling out the x
  float cU = (v1.x * v2.y) - (v1.y * v2.x);

  //V - Normal
  float aV = -((v1.y * v2.v) - (v1.v * v2.y)); // goes across x by canceling out the y
  float bV = -((v1.v * v2.x) - (v1.x * v2.v)); // goes across y by canceling out the x
  float cV = (v1.x * v2.y) - (v1.y * v2.x);

  //Bottom Left Triangle
  for (i = ceil(TOP.y); i < ceil(MID.y); i++)
  {
    MinX += InSlope[LeftEdge];
    MaxX += InSlope[RightEdge];

    Color rgb = TOP.color; // in order to ensure that TOP.color still has it has its starting value rgb will be in used in place of TOP.color
    Point temp = TOP; // same purpose as rgb
    Color rgbTexture; // same purpose as rgb
    Color rgbText;
    Color rgbPercent;
    float Alpha = 0;

    if (mode != 3)
    {
      //Updates color along the x and y
      TOP.color.r = (TOP.color.r + (bR / cR)) + ((aR / cR) * InSlope[LeftEdge]);
      TOP.color.g = (TOP.color.g + (bG / cG)) + ((aG / cG) * InSlope[LeftEdge]);
      TOP.color.b = (TOP.color.b + (bB / cB)) + ((aB / cB) * InSlope[LeftEdge]);
    }    
    if (mode != 0)
    {
      //Updates u and v along the y
      TOP.u = (TOP.u + (bU / cU)) + ((aU / cU) * InSlope[LeftEdge]);
      TOP.v = (TOP.v + (bV / cV)) + ((aV / cV) * InSlope[LeftEdge]);
    }
    for (j = ceil(MinX); j < ceil(MaxX); j++)
    {
      //Make sure that this is an int by Rounding
      int index = (Round(temp.v * 127) * 128 + Round(temp.u * 127)) * 4;
      temp.v += (aV / cV); //updating TOP.v along the x
      temp.u += (aU / cU); //updating TOP.u along the x

      texture[index]; //R
      texture[index + 1]; // G
      texture[index + 2]; // B
      texture[index + 3]; // Alpha

      //Interpolated
      rgb.r += ((aR / cR));
      rgb.g += ((aG / cG));
      rgb.b += ((aB / cB));

      Alpha = (texture[index + 3] / 255.0f);
      rgbPercent.r = (texture[index] / 255.0f);
      rgbPercent.g = (texture[index + 1] / 255.0f);
      rgbPercent.b = (texture[index + 2] / 255.0f);

      //Combined Surface and Texture Color
      rgbText.r = rgb.r + (Alpha * (texture[index] - (rgb.r)));
      rgbText.g = rgb.g + (Alpha * (texture[index+1] - (rgb.g)));
      rgbText.b = rgb.b + (Alpha * (texture[index+2] - (rgb.b)));

      rgbTexture.r = rgb.r * rgbPercent.r;
      rgbTexture.g = rgb.g * rgbPercent.g;
      rgbTexture.b = rgb.b * rgbPercent.b;

      if (mode == 0)
      {
        //Interpulates the rgb and draws the pixel
        FrameBuffer::SetPixel(j, i, rgb.r, rgb.g, rgb.b);
      }
      if (mode == 1)
      {
        FrameBuffer::SetPixel(j, i, rgbText.r, rgbText.g, rgbText.b);
      }
      if (mode == 2)
      {
        FrameBuffer::SetPixel(j, i, rgbTexture.r, rgbTexture.g, rgbTexture.b);
      }
      if (mode == 3)
      {
        FrameBuffer::SetPixel(j, i, texture[index], texture[index+1], texture[index+2]);
      }
    }
  }

  if (isLeft == true)
  {
    MinX = MID.x;
    LeftEdge = 2;
  }
  else
  {
    MaxX = MID.x;
    RightEdge = 2;
  }

  //Top Right Triangle
  for (i = ceil(MID.y); i < ceil(BOT.y); i++)
  {
    MinX += InSlope[LeftEdge];
    MaxX += InSlope[RightEdge];

    Color rgb1 = TOP.color;
    Point temp = TOP; // same purpose as rgb
    Color rgbTexture; // same purpose as rgb
    Color rgbText;
    Color rgbPercent;
    float Alpha = 0;

    if (isLeft == true)
    {
      temp.u = MID.u;
      temp.v = MID.v;
    }

    if (mode != 3)
    {
      if (isLeft == true)
      {
        MID.color.r = (MID.color.r + (bR / cR)) + ((aR / cR) * InSlope[LeftEdge]);
        MID.color.g = (MID.color.g + (bG / cG)) + ((aG / cG) * InSlope[LeftEdge]);
        MID.color.b = (MID.color.b + (bB / cB)) + ((aB / cB) * InSlope[LeftEdge]);
      }
      //Updates color along the x and y
      TOP.color.r = (TOP.color.r + (bR / cR)) + ((aR / cR) * InSlope[LeftEdge]);
      TOP.color.g = (TOP.color.g + (bG / cG)) + ((aG / cG) * InSlope[LeftEdge]);
      TOP.color.b = (TOP.color.b + (bB / cB)) + ((aB / cB) * InSlope[LeftEdge]);
      
    }
    if (mode != 0)
    {
      //Updates u and v along the y
      if (isLeft == true)
      {
        MID.u = (MID.u + (bU / cU)) + ((aU / cU) * InSlope[LeftEdge]);
        MID.v = (MID.v + (bV / cV)) + ((aV / cV) * InSlope[LeftEdge]);
      }
      TOP.u = (TOP.u + (bU / cU)) + ((aU / cU) * InSlope[LeftEdge]);
      TOP.v = (TOP.v + (bV / cV)) + ((aV / cV) * InSlope[LeftEdge]);
    }
    

   

    for (j = ceil(MinX); j < ceil(MaxX); j++)
    {
      int temp1 = Round(temp.v * 127);
      int temp2 = Round(temp.u * 127);

      int index = (temp1 * 128 + temp2) * 4;

      temp.v += (aV / cV); //updating TOP.v along the x
      temp.u += (aU / cU); //updating TOP.u along the x

      texture[index]; //R
      texture[index + 1]; // G
      texture[index + 2]; // B
      texture[index + 3]; // Alpha

                          //Interpolated
      rgb1.r += ((aR / cR));
      rgb1.g += ((aG / cG));
      rgb1.b += ((aB / cB));

      Alpha = (texture[index + 3] / 255.0f);
      rgbPercent.r = (texture[index] / 255.0f);
      rgbPercent.g = (texture[index + 1] / 255.0f);
      rgbPercent.b = (texture[index + 2] / 255.0f);

      //Combined Surface and Texture Color
      rgbText.r = rgb1.r + (Alpha * (texture[index] - (rgb1.r)));
      rgbText.g = rgb1.g + (Alpha * (texture[index + 1] - (rgb1.g)));
      rgbText.b = rgb1.b + (Alpha * (texture[index + 2] - (rgb1.b)));

      rgbTexture.r = rgb1.r * rgbPercent.r;
      rgbTexture.g = rgb1.g * rgbPercent.g;
      rgbTexture.b = rgb1.b * rgbPercent.b;
      if (mode == 0)
      {
        FrameBuffer::SetPixel(j, i, rgb1.r, rgb1.g, rgb1.b);
      }
      if (mode == 1)
      {
        FrameBuffer::SetPixel(j, i, rgbText.r, rgbText.g, rgbText.b);
      }
      if (mode == 2)
      {
        FrameBuffer::SetPixel(j, i, rgbTexture.r, rgbTexture.g, rgbTexture.b);
      }
      if (mode == 3)
      {
        FrameBuffer::SetPixel(j, i, texture[index], texture[index + 1], texture[index + 2]);
      }
    }
  }

}
