// Global Variables

#ifndef VAR_INCLUDED
#define VAR_INCLUDED

#include "matrix.h"  

using namespace std;
using namespace math;

#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif

Matrix dHo(7,1);
Matrix DH(7,4);
//long * ptr;
float e2r1, e2r2, e2r3, e2r4, e2r5, e2r6, e2r7, e2r8, e2r9, e2d;
Matrix timep(1,1), q1(1,1), q2(1,1), q3(1,1), q4(1,1), q5(1,1), q6(1,1), q7(1,1), qll(1,1), qrr(1,1), qd1(1,1), qd2(1,1), qd3(1,1), qd4(1,1), qd5(1,1), qd6(1,1), qd7(1,1), qdl(1,1), qdr(1,1), x(1,1), y(1,1), z(1,1), roll(1,1), pitch(1,1), yaw(1,1), xc(1,1), yc(1,1), zc(1,1), rollc(1,1), pitchc(1,1), yawc(1,1), detJoap(1,1), detJop(1,1); 
int varscreenopn;
int exitvar;

#endif