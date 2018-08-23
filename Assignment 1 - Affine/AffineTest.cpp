// AffineTest.cpp
// cs200 8/17

#include <iostream>
#include <iomanip>
#include <cmath>
#include "Affine.h"
using namespace std;


ostream& operator<<(ostream& s, const Hcoord& v) {
  return s << '[' << v.x << ',' << v.y << ',' << v.w << ']';
}


ostream& operator<<(ostream& s, const Affine& A) {
  s << '{';
  for (int i=0; i < 3; ++i) {
    s << '{';
    for (int j=0; j < 3; ++j)
      s << A[i][j] << ((j < 2) ? ',' : '}');
    s << ((i < 2) ? ',' : '}');
  }
  return s;
}


int main(void) {
  const Point O;
  const float pi = 4.0f*atan(1.0f);
  cout << fixed << setprecision(3);

  Vector v(4,-5);
  Affine A = translate(v) * scale(3);
  cout << A << endl;


  Point P(9,-2);
  Point Q = O + 3*(P-O) + v,
        R = A*P;
  cout << abs(R-Q) << endl;

  A = rotate(0.5f*pi) * translate(Vector(7,5));
  cout << A << endl;

  P = Point(1,4);
  Affine B = translate(P-O) * rotate(1.5f*pi) * translate(O-P);
  cout << B << endl;

  Q = Point(5,8);
  Vector u = Q - P;
  cout << dot(u,B*Q-P) << endl;
  
  B = Affine(Vector(5,2),Vector(1,4),Point(4,4));
  cout << B << endl;

  u = 3*P + 2*Q - 5*R;
  cout << u << endl;
  P = 3*P + 2*Q - 7*R; // this should fail ...

  return 0;
}

