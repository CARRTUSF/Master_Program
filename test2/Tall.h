/* This function is for getting the transformations of the USF WMRA with 9 DOF.
q is for the 7 joints in radians, and dq is for the wheelchair only in millimeters and radians.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2007 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#ifndef _TALL_H // if not defined
#define _TALL_H // define MyHeader

#include "matrix.h"  
#include "vector.h"
#include "DH.h"
#include "Rotx.h"
#include "Rotz.h"
#include "Transl.h"
#include "w2T.h"
using namespace std;
using namespace math;

#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif

void WMRA_Tall(int i, Matrix q, Matrix dq, Matrix Twc, Matrix& T, Matrix& Ta, Matrix& Twco, Matrix& T1, Matrix& T2, Matrix& T3, Matrix& T4, Matrix& T5, Matrix& T6, Matrix& T7);

#endif
