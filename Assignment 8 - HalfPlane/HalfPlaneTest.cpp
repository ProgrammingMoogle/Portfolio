// HalfPlaneTest.cpp
// -- simple test of the plane utility functions
// cs200 11/17

#include <iostream>
#include <iomanip>
#include "HalfPlane.h"
using namespace std;


ostream& operator<<(ostream& s, const Interval I) {
  s << '[' << I.bgn << ',' << I.end << ']';
  return s;
}


int main(void) {

  { // half-space/point dot product 2
    HalfPlane h(2,-1,4);
    Point P(-5,-8);
    cout << dot(h,P) << endl;
  }
   
  { // half-space from normal+point 
    Vector n(8,-3);
    Point C(2,9);
    HalfPlane h = HalfPlane(n,C);
    cout << boolalpha;
    cout << Hcoord::near(dot(h,C),0)
         << ' ' << (dot(h,C+n) > 0)
         << ' ' << (dot(h,C-n) < 0) << endl;
  }

  { // half-space from 3 points 
    Point A(28,44), // m.x = 33 m.y = 9
          B(61,53),
          P(54,21); 
    HalfPlane h = HalfPlane(A,B,P);
    cout << boolalpha;
    cout << Hcoord::near(dot(h,A),0)
         << ' ' << Hcoord::near(dot(h,B),0)
         << ' ' << (dot(h,P) < 0) << endl;
  }

  { // non-trivial intersection interval of half-space/segment
    HalfPlane h(-3,1,7);
    Point P(11,5),
          Q(1,10);
    Interval I = intersect(h,P,Q);
    cout << I << endl;
  }

  { // trivial intersection interval of half-space/segment
    HalfPlane h(1,2,5);
    Point P(-1,1),
          Q(2,-3);
    Interval I = intersect(h,P,Q);
    cout << I.bgn << endl;
    cout << I.end << endl;
    cout << I.isEmpty() << endl;
  }

  return 0;
}

