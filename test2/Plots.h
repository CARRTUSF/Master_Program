/* This function plots different animation variables for USF WMRA with 9 DOF.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2007 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#ifndef _PLOTS_H // if not defined
#define _PLOTS_H // define MyHeader

#include "matrix.h"  
#include "T2rpy.h"
#include <math.h>
#include <stdio.h>
using namespace std;
using namespace math;

#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif

#define PI 3.14159265
#define r2d 180/PI //Conversions from Radians to Degrees.
void WMRA_Plots(int st, Matrix L, float dt, int i, float tt, Matrix qn, Matrix dq, Matrix Tn, Matrix Tnwc, float detjoa, float detjo);

#endif