// ClipTest.cpp
// -- simple test of clip class/functions
// cs200 11/17

#include <iostream>
#include <iomanip>
#include <vector>
#include "Clip.h"
using namespace std;


ostream& operator<<(ostream& s, const Point& P) {
  s << '(' << P.x << ',' << P.y << ')';
  return s;
}


unsigned smallest(const vector<Point>& pts) {
  unsigned imin = 0;
  for (unsigned i=1; i < pts.size(); ++i)
    if (pts[i].x < pts[imin].x
        || (Hcoord::near(pts[i].x,pts[imin].x)
            && pts[i].y < pts[imin].y))
      imin = i;
  return imin;
}


int main(void)
{
  cout << boolalpha;

  { // clip to the standard square default constructor call
    Clip clip;

    // clip a line segment operator()
    Point P(2,5),
          Q(-1,-3);
    bool is_clipped = clip(P,Q);
    cout << is_clipped << ": " << P << ' ' << Q << endl;

    // cull a line segment operator()
    Point R(-2,0),
          S(0,3);
    is_clipped = clip(R,S);
    cout << is_clipped << endl;

    // clip a polygon polygon
    vector<Point> polygon;
    polygon.push_back(Point(0,0));
    polygon.push_back(Point(-1,2));
    polygon.push_back(Point(-2,0));
    is_clipped = clip(polygon);
    cout << is_clipped << ": ";
    unsigned istart = smallest(polygon);
    for (unsigned i=0; i < polygon.size(); ++i)
      cout << polygon[(istart+i)%polygon.size()] << ' ';
    cout << endl;

    // cull a polygon
    polygon.clear();
    polygon.push_back(R);
    polygon.push_back(S);
    polygon.push_back(Point(-1,5));
    is_clipped = clip(polygon);
    cout << is_clipped << endl;
  }


  { // clip to a triangle
    Point V[3] = { Point(0,0), Point(2,0), Point(0,2) };
    Clip clip(vector<Point>(V,V+3));

    // clip a line segment
    Point P(-2,0),
          Q(2,1);
    bool is_clipped = clip(P,Q);
    cout << is_clipped << ": " << P << ' ' << Q << endl;

    // cull a line segment
    P = Point(-1,2);
    Q = Point(2,4);
    cout << clip(P,Q) << endl;

    // clip a triangle
    vector<Point> polygon;
    polygon.push_back(Point(1,-2));
    polygon.push_back(Point(2,2));
    polygon.push_back(Point(-2,1));
    is_clipped = clip(polygon);
    cout << is_clipped << ": ";
    unsigned istart = smallest(polygon);
    for (unsigned i=0; i < polygon.size(); ++i)
      cout << polygon[(istart+i)%polygon.size()] << ' ';
    cout << endl;

    // cull a quadrilateral
    polygon.clear();
    polygon.push_back(Point(3,2));
    polygon.push_back(Point(5,3));
    polygon.push_back(Point(5,0));
    polygon.push_back(Point(2,-1));
    cout << clip(polygon) << endl;
  }

  return 0;
}

