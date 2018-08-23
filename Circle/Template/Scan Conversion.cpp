/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Scan Conversion.cpp
Purpose: This file contains the functions for drawing various lines :
Vertical, Horizontal, Diagonal, and Naive Lines.
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_5
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 03/20/2017
- End Header --------------------------------------------------------*/
#include "Scan Conversion.h"

//Draw Horizontal Line
void DrawHorizontalLine(const Point &p1, const Point &p2, unsigned char r, unsigned char g, unsigned char b)
{
  //compares if the p1.x is further along than p2.x
  if (Round(p1.x) > Round(p2.x))
  {
    //if so it loops starting from p1.x to p2.x 
    //it sets the pixels as it goes
    for (float i = Round(p1.x); i > Round(p2.x); i--)
    {
      FrameBuffer::SetPixel(i, Round(p1.y), r, g, b);
    }
  }
  //compares if the p2.x is further along than p1.x
  else if (p2.x > Round(p1.x))
  {
    //if so it loops starting from p2.x to p1.x 
    //it sets the pixels as it goes
    for (float i = Round(p2.x); i > Round(p1.x); i--)
    {
      FrameBuffer::SetPixel(i, Round(p1.y), r, g, b);
    }
  }
}

//Draw Vertical Lines
void DrawVerticalLine(const Point &p1, const Point &p2, unsigned char r, unsigned char g, unsigned char b)
{
  //compares if the p1.y is further along than p2.y
  if (p1.y > p2.y)
  {
    //if so it loops starting from p1.y to p2.y
    //it sets the pixels as it goes
    for (int i = Round(p1.y); i > Round(p2.y); i--)
    {
      FrameBuffer::SetPixel(Round(p1.x), i, r, g, b);
    }
  }
  //compares if the p2.y is further along than p1.y
  else if (p2.y > p1.y)
  {
    //if so it loops starting from p2.y to p1.y
    //it sets the pixels as it goes
    for (float i = Round(p2.y); i > Round(p1.y); i--)
    {
      FrameBuffer::SetPixel(Round(p1.x), i, r, g, b);
    }
  }
}

//Draw Diagonal Line
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

//Draw Naive Lines
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

//Draw Midpoint Lines
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
//Draws a circle in a symmetrically accurate way
void DrawCircleEightWaySymmetric(const int &x, const int &y, const int &xC, const int &yC, unsigned char r, unsigned char g, unsigned char b)
{
  FrameBuffer::SetPixel(Round(x + xC), Round(-y + yC), r, g, b);
  FrameBuffer::SetPixel(Round(x + xC), Round(y + yC), r, g, b);
  FrameBuffer::SetPixel(Round(-x + xC), Round(-y + yC), r, g, b);
  FrameBuffer::SetPixel(Round(-x + xC), Round(y + yC), r, g, b);
  FrameBuffer::SetPixel(Round(y + xC), Round(-x + yC), r, g, b);
  FrameBuffer::SetPixel(Round(y + xC), Round(x + yC), r, g, b);
  FrameBuffer::SetPixel(Round(-y + xC), Round(-x + yC), r, g, b);
  FrameBuffer::SetPixel(Round(-y + xC), Round(x + yC), r,  g, b);
}

//Draws a circle using a midpoint and its radius
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
  int xdir = Round(y * sqrt(2)/2);
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

//Draw a section of ellipse with four points
void DrawEllipseFourWaySymmetric(const int &x, const int &y, const int &xC, const int &yC, unsigned char r, unsigned char g, unsigned char b)
{
  FrameBuffer::SetPixel(x + xC, y + yC, r, g, b);
  FrameBuffer::SetPixel(-x + xC, y + yC, r, g, b);
  FrameBuffer::SetPixel(x + xC, -y + yC, r, g, b);
  FrameBuffer::SetPixel(-x + xC, -y + yC, r, g, b);
}

//Draw Ellipse using a center point a major and minor
void DrawMidpointEllipse(const Point &p, const float &A, const float &B, unsigned char r, unsigned char g, unsigned char b)
{
  int x = 0;
  int y = B;

  int dpX = -((A*A) * B) + B*B + ((A*A)/4);
  int dpXS = (B*B)*(2*x + 3);
  int dpXSE = (B*B)*(2*x + 3) + 2*(A*A)*(-y+1);
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

  DrawEllipseFourWaySymmetric(x,y, p.x,p.y,r,g,b);

  for (x; x <= xdirection; ++x)
  {
    //E
    if(dpX < 0)
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