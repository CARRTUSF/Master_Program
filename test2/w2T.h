/* This function gives the Transformation Matrix of the wheelchair with 2 DOF (Ground to WMRA base), given the previous transformation matrix and the required wheelchair's travel distance and angle.
Dimentions are as supplies, angles are in radians.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2007 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#ifndef _W2T_H // if not defined
#define _W2T_H // define MyHeader
#define PI 3.14159265
#define EPS 2.2204460492503131e-016 

#include "matrix.h"  
#include "vector.h"
#include "WCD.h"
using namespace std;
using namespace math;

#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif

Matrix WMRA_w2T(int ind, Matrix Tp, Matrix q);

#endif
