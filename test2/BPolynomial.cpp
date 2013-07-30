/* This function uses a 3rd order Polynomial with a Blending factor to find a smooth trajectory points of a variable "q" along a streight line, given the initial and final variable values and the number of trajectory points.
The output is the variable position.
See Eq. 7.18 page 210 of Craig Book

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%Modified By:Ana Catalina Torres%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/
#include "matrix.h" 
#include "BPolynomial.h" 
using namespace std;
using namespace math;

Matrix WMRA_BPolynomial(float qi, float qf, float n){
	Matrix qtb(2,1);
	// Blending Factor:
	int b;
	b=5;

	// Initializing the time:
	float tt, tf, dt, qddb, tb, qdb, qb;
	float a01, a11, a21, a31, a41, a51, a02, a12, a22, a32, a42, a52;
	int i;
	tt=0;
	tf=abs(qf-qi);
	dt=tf/(n-1);
	
	if (tf > 0.001){
		// Blending procedure:
		// Time, position, velocity, and acceleration of the variable at the first blending point:
		qddb=b*4*(qf-qi)/pow(tf,2);
		tb=tf/2-sqrt(pow(qddb,2)*pow(tf,2)-4*qddb*(qf-qi))/abs(2*qddb);
		qdb=qddb*tb;
		qb=qi+qddb*pow(tb,2)/2;
		// Calculating the polynomial factors at the first blending point: From Eq.7.18 page 210 of Craig Book
		a01=qi;
		a11=0;
		a21=0.5*qddb;
		a31=(20*(qb-qi)-8*qdb*tb-2*qddb*pow(tb,2))/(2*pow(tb,3));
		// a41=(30*(qi-qb)+14*qdb*tb+qddb*pow(tb,2))/(2*pow(tb,4)); % Uncomment for 5th order polynomial.
		// a51=(12*(qb-qi)-6*qdb*tb)/(2*pow(tb,5)); % Uncomment for 5th order polynomial.
		// Calculating the polynomial factors at the second blending point: From Eq.7.18 page 210 of Craig Book
		a02=qb+qdb*(tf-2*tb);
		a12=qdb;
		a22=-0.5*qddb;
		a32=(20*(qf-a02)-12*a12*tb+2*qddb*pow(tb,2))/(2*pow(tb,3));
		// a42=(30*(a02-qf)+16*a12*tb-qddb*pow(tb,2))/(2*pow(tb,4)); % Uncomment for 5th order polynomial
		// a52=(12*(qf-a02)-6*a12*tb)/(2*pow(tb,5)); % Uncomment for 5th order polynomial.
	}
	
	// Calculating the intermediate joint angles along the trajectory from the initial to the final position:
	float *qttemp;
	qttemp = new float[n];
	for (i=0; i<n; i++){
		if (tf<=0.001){
			qttemp[i]=qi;
		}
		else if (tt<=tb){
			qttemp[i]=a01+a11*tt+a21*pow(tt,2)+a31*pow(tt,3); //+a41*pow(tt,4)+a51*pow(tt,5); // Uncomment before "+a41" for 5th order polynomial.
		}
		else if (tt>=(tf-tb)){
			qttemp[i]=a02+a12*(tt+tb-tf)+a22*pow((tt+tb-tf),2)+a32*pow((tt+tb-tf),3); //+a42*pow((tt+tb-tf),4)+a52*pow((tt+tb-tf),5); // Uncomment before "+42" for 5th order polynomial.
		}
		else {
			qttemp[i]=qb-qdb*(tb-tt);
		}
		tt = tt + dt;
	}    
	qtb.SetSize(n,1);
	for (i=0; i < n; i++){
		qtb(i,0) = qttemp[i];
	}
	delete [] qttemp; 
	return qtb;
}