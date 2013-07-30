/* This function gives the homogeneous transformation matrix, given the rotation angle about the Z axis.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "vector.h"
#include "Rotz.h"
using namespace std;
using namespace math;

Matrix WMRA_rotz(float t){
	
	Matrix T(4,4);

	float c, s;
	c=cos(t);
	s=sin(t);
	T.Unit(4);
	T(0,0)= c;
	T(0,1)= -s;
	T(1,0)= s;
	T(1,1)= c;

	return T;
}
