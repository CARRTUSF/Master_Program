/*  This function uses a 3rd order Polynomial with no Blending factor to find a smooth trajectory points of a variable "q" along a streight line, given the initial and final variable values and the number of trajectory points.
The output is the variable position.
See Eqs. 7.3 and 7.6 page 204,205 of Craig Book

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "vector.h"
#include "Polynomial.h"
using namespace std;
using namespace math;

#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif

Matrix WMRA_Polynomial(float qi, float qf, float n){
	
	Matrix qtp(2,1);

	float tt, tf, dt;
	int i;
	tt=0;
	tf=abs(qf-qi);
	dt=tf/(n-1);
	
	float *qttemp;
	qttemp = new float[n];

	for (i=0; i<n; i++){
		if (tf<=0.001){
			qttemp[i]=qi;
		}
		else {
			qttemp[i]=qi+(qf-qi)*3*pow(tt,2)/pow(tf,2)-(qf-qi)*2*pow(tt,3)/pow(tf,3); //From Eq.7.3 and 7.6 page 204,205 of Craig Book
		}
		tt = tt + dt;
	}
	qtp.SetSize(n,1);
	for (i=0; i < n; i++){
		qtp(i,0) = qttemp[i];
	}
	delete [] qttemp; 

	return qtp;
}