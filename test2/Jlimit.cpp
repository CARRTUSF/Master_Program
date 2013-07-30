/* This function gives the joint limit vector to be used in the program. 
Modifying the parameters on this file is sufficient to change these limits in all related programs.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "Jlimit.h" 
using namespace std;
using namespace math;

void WMRA_Jlimit(Matrix& qmin, Matrix& qmax){
	
	//float qmintemp[7]= {-170*PI/180,-170*PI/180,-170*PI/180,-170*PI/180,-170*PI/180,90*PI/180,-200*PI/180};
	//float qmaxtemp[7] = {170*PI/180,170*PI/180,170*PI/180,170*PI/180,170*PI/180,270*PI/180,200*PI/180};

	float qmintemp[7]= {-170*PI/180,-45*PI/180,-170*PI/180,-170*PI/180,-170*PI/180,-79*PI/180,-200*PI/180};
	float qmaxtemp[7] = {170*PI/180,170*PI/180,170*PI/180,270*PI/180,170*PI/180,79*PI/180,200*PI/180};

	int i;
	for (i=0; i < 7; i++){
		qmin(0,i) = qmintemp[i];
		qmax(0,i) = qmaxtemp[i];
	}
}
