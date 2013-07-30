/* This function is for the resolved rate and optimization solution of the USF WMRA with 9 DOF.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2007 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#ifndef _OPT_H // if not defined
#define _OPT_H // define MyHeader

#include "matrix.h"  
#include "vector.h"
#include "WCD.h"
#include "Jlimit.h"
#include "sign.h"
#include <limits>

using namespace std;
using namespace math;

#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif

Matrix WMRA_Opt(int i, float JLA, float JLO, Matrix Jo, float detJo, Matrix dq, Matrix dx, float dt, Matrix q);
   
#endif