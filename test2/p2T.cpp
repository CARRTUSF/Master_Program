/* This function gives the Transformation Matrix of the WMRA's base on the wheelchair with 2 DOF, given the desired x,y position and z rotation angle.
Dimentions are as supplies, angles are in radians.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "vector.h"
#include "WCD.h"
#include "p2T.h" 
using namespace std;
using namespace math;

Matrix WMRA_p2T(float x, float y, float a){

	Matrix T(4,4);

	//Reading the Wheelchair's constant dimentions, all dimentions are converted in millimeters:
	Matrix L(1,5);
	L=WMRA_WCD();
	//Defining the Transformation Matrix:

	T.Unit(4);	
	T(0,0)= cos(a);
	T(0,1)= -sin(a);
	T(0,3)= x; 
	T(1,0)= sin(a);
	T(1,1)= cos(a);
	T(1,3)= y;
	T(2,3)= L(0,3)+L(0,4);

	return T;
}