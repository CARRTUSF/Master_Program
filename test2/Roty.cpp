/* This function gives the homogeneous transformation matrix, given the rotation angle about the Y axis.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2007 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "vector.h"
#include "Roty.h"
using namespace std;
using namespace math;

Matrix WMRA_roty(float t){
	
	Matrix T(4,4);

	float c, s;
	c=cos(t);
	s=sin(t);
	T.Unit(4);
	T(0,0)= c;
	T(0,2)= s;
	T(2,0)= -s;
	T(2,2)= c;

	return T;
}
