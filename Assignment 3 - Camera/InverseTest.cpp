// InverseTest.cpp
// -- simple test of the 'Inverse' function
// cs200 9/17


#include <iostream>
#include "Affine.h"
using namespace std;


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

  Affine A(Vector(1,3),Vector(2,4),Point(5,6)),
         iA = inverse(A);

  cout << "A = " << A << endl;
  cout << "A^-1 = " << iA << endl;

  return 0;
}

