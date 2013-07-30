/* This function communicates with the physical USF WMRA system with 9 DOF to get the encoder readings and send the commands to be executed.
The (.H) file and the (.DLL) file that contains the used functions should be in the directory containing this program.
config=0: Set the current encoder readings to zeros.
config=1: Read the encoder readings from the configuration txt file.
config=2: Change the configuration file to the initial values provided by (qo), then read the encoder readings from the configuration txt file.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2007 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#ifndef _ARMMOTION2_H // if not defined
#define _ARMMOTION2_H // define MyHeader

#include "matrix.h"  
#include "WCD.h"  
#include "control.h"
#include "Galil.h"
#include <math.h>
using namespace std;
using namespace math;
//using namespace System;
#define PI 3.14159265

#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif

Matrix WMRA_ARM_Motion2(Matrix qo, float dt, int choice, Galil &g);

#endif
