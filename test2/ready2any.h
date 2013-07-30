/* This "new USF WMRA" function SIMULATES the arm going from the ready position to any position with ANIMATION. All angles are in Radians.
 The ready position is assumed to be qd=[pi/2;pi/2;0;pi/2;pi/2;pi/2;0] (Radians).
 ini=1 --> initialize animation figures, ini=2 or any --> just update the figures, ini=3 --> close the figures.

Function Declaration:*/

#ifndef _READY2ANY_H // if not defined
#define _READY2ANY_H // define MyHeader

#include "matrix.h"
#include "ArmMotion.h" 
#include "DH.h" 
#include "q2T.h" 
#include <time.h>
#include "Galil.h"
using namespace std;
using namespace math;
#define PI 3.14159265

#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif

void WMRA_ready2any(int ini, int vr, int ml, int arm, Matrix Tiwc, Matrix qd, Galil &g);

#endif
