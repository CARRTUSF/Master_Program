/* This function gives the Roll, Pitch, Taw angles, given the transformation matrix.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "vector.h"
#include "T2rpy.h"
using namespace std;
using namespace math;

Matrix WMRA_T2rpy(Matrix T){

	Matrix rpy(3,1); 
	rpy.Null(3,1);
	float c, s;
	
	//Making sure there is no singularity:
	
	if (abs(T(0,0))<EPS && abs(T(1,0))<EPS){
		rpy(0,0)=0;
		rpy(1,0)=atan2(-T(2,0), T(0,0));
		rpy(2,0)=atan2(-T(1,2), T(1,1));
	}
	else{
		rpy(0,0)=atan2(T(1,0), T(0,0));
		s=sin(rpy(0,0));
		c=cos(rpy(0,0));
		rpy(1,0)=atan2(-T(2,0), c*T(0,0)+s*T(1,0));
		rpy(2,0)=atan2(s*T(0,2)-c*T(1,2), c*T(1,1)-s*T(0,1));
	}

	return rpy;
}

