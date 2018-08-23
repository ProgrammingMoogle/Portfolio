////////////////////
// Kacey Lei Yeo
// Assignment 9
// CS200
// Fall2017
////////////////////
#include "Clip.h"
#include "Affine.h"
#include "HalfPlane.h"

Clip::Clip(void)
{
  HalfPlane temp(1,1);
  half_planes.push_back(temp);

  temp = HalfPlane(-1,1);
  half_planes.push_back(temp);

  temp = HalfPlane(-1,-1);
  half_planes.push_back(temp);

  temp = HalfPlane(1,-1);
  half_planes.push_back(temp);

  
}

Clip::Clip(const std::vector<Point>& clip_obj_verts)
{
  for (int i = 0; i <= clip_obj_verts.size(); i++)
  {
    HalfPlane temp(clip_obj_verts[i].x, clip_obj_verts[i].y, clip_obj_verts[i].w);
    half_planes.push_back(temp);
  }
}

bool Clip::operator()(Point & P, Point & Q)
{
  Interval PQ;
  float dotP = 0;
  float dotQ = 0;
  float inter;
  int i = 1;
  PQ.bgn = 0;
  PQ.end = 1;

  while (i <= half_planes.size() && PQ.bgn < PQ.end)
  {
    dotP = dot(half_planes[i], P);
    dotQ = dot(half_planes[i], Q);
    if (dotP <= 0 && dotQ > 0)
    {
      Q.x = 0;
      Q.y = dotP / (dotP - dotQ);
    }
    else if (dotP > 0 && dotQ <= 0)
    {
      P.x = dotP / (dotP - dotQ);
      P.y = 1;
    }
    else if (dotP > 0 && dotQ > 0)
    {
      P.x = 0;
      Q.x = 0;
      P.y = 0;
      Q.y = 0;
    }
    else
    {
      Q = (1 - PQ.end)*P + PQ.end*Q;
      P = (1 - PQ.bgn)*P + PQ.bgn*Q;
    }
    i++;
  }
  return false;
}

bool Clip::operator()(std::vector<Point>& verts)
{
  Interval work;
  float inter = 0;
  float dotP = 0;
  float dotQ = 0;
  Point temp;

  for (int i = 0; i < half_planes.size(); i++)
  {
    for (int j = 0; j < verts.size(); j++)
    {
      work = intersect(half_planes[i], verts[j], verts[j + 1]);
      dotP = dot(half_planes[i], verts[j]);
      dotQ = dot(half_planes[i], verts[j + 1]);
      inter = dotP / (dotP - dotQ);
      temp.x = ((1 - inter)*verts[j].x) + (inter*verts[j + 1].x);
      temp.y = ((1 - inter)*verts[j].y) + (inter*verts[j + 1].y);
      half_planes[i].x = temp.x;
      half_planes[i].y = temp.y;
      if (!work.isEmpty())
      {
        return true;
      }
    }
    
  }
  return false;
}

bool Clip::clip_to_halfplane(const HalfPlane & h, std::vector<Point>& verts)
{
  Interval work;
  float inter = 0;
  float dotP = 0;
  float dotQ = 0;
  Point temp;

  for (int i = 0; i < verts.size(); i++)
  {
    work = intersect(h, verts[i], verts[i + 1]);
    if (!work.isEmpty())
    {
      dotP = dot(h, verts[i]);
      dotQ = dot(h, verts[i + 1]);
      inter = dotP / (dotP - dotQ);
      temp.x = ((1 - inter)*verts[i].x) + (inter*verts[i + 1].x);
      temp.y = ((1 - inter)*verts[i].y) + (inter*verts[i + 1].y);
      temp_vertices.push_back(temp);
      return true;
    }
  }
  return false;
}
