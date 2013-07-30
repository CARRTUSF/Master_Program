/* This function uses a Linear function to find an equally-spaced trajectory points of a variable "q" along a streight line, given the initial and final variable values and the number of trajectory points.
The output is the variable position.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "Linear.h"
using namespace std;
using namespace math;


Matrix WMRA_Linear(float qi, float qf, float n){
	
	Matrix qt(2,1);
	int i;
	float dq;
	dq =(qf-qi)/(n-1);

	float *qttemp;
	qttemp = new float[n];
	for (i=1; i<n+1; i++){
		qttemp[i-1]=qi+dq*(i-1);
	}

	qt.SetSize(n,1);
	for (i=0; i < n; i++){
		qt(i,0) = qttemp[i];
	}
	delete [] qttemp; 

	return qt;
}
