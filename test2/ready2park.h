/* This "new USF WMRA" function SIMULATES the arm going from the ready position to the parking position with ANIMATION. All angles are in Radians.
The parking position is assumed to be qi=[0;pi/2;0;pi;0;0;0] (Radians), the ready position is assumed to be qd=[pi/2;pi/2;0;pi/2;pi/2;pi/2;0]] (Radians).
ini=1 --> initialize animation figures, ini=2 or any --> just update the figures, ini=3 --> close the figures.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2007 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#ifndef _READY2PARK_H // if not defined
#define _READY2PARK_H // define MyHeader

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

void WMRA_ready2park(int ini, int vr, int ml, int arm, Matrix Tiwc, Matrix qiwc, Galil &g);

#endif
