/* This "new USF WMRA" function SIMULATES the arm going from the ready position to the parking position with ANIMATION. All angles are in Radians.
The parking position is assumed to be qi=[0;pi/2;0;pi;0;0;0] (Radians), the ready position is assumed to be qd=[pi/2;pi/2;0;pi/2;pi/2;pi/2;0]] (Radians).
ini=1 --> initialize animation figures, ini=2 or any --> just update the figures, ini=3 --> close the figures.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres & Punya A. Basnayaka%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "ready2park.h" 
#include "ArmMotion.h" 
#include "DH.h" 
#include "q2T.h" 
#include <time.h>
using namespace std;
using namespace math;

void WMRA_ready2park(int ini, int vr, int ml, int arm, Matrix Tiwc, Matrix qiwc, Galil &g){

Matrix zero(1,1);
zero.Null(1,1);
	if (ini==3){
		if (arm==1){
			try {
            WMRA_ARM_Motion(ini, 0, zero, 0, g);
			}
			catch (...) {
				cout << "Exception 1 occurred";
			}
		}
		if (vr==1){
			/*try {
				WMRA_VR_Animation(ini, 0, 0);
			}
			catch (...) {
				cout << "Exception 2 occurred";
			}*/
		}
		if (ml==1){
			/*try {
				WMRA_ML_Animation(ini, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			}
			catch (...) {
				cout << "Exception 3 occurred";
			}*/
		}
		return;
	}

	// Defining the used conditions:
	float qi2 [7]= {PI/2, PI/2, PI/2, PI/2, PI/2, PI/2, 0}; // Initial joint angles (Parking Position).
	float qd2 [7]= {0, PI/2, 0, PI, 0, 0, 0}; // Final joint angles (Ready Position).
	Matrix qd(7,1), qi(7,1);
	float ts, dt, ddt;
	int n, j;
	for ( j = 0 ; j < 7 ; j++ ) {
		qi(j,0)=qi2[j];
		qd(j,0)=qd2[j];
	}
	ts=20;  // (5 or 10 or 20) Simulation time to move the arm from any position to the ready position.
	n=100;  // Number of time steps.
	dt=ts/n;  // The time step to move the arm from any position to the ready position.
	Matrix dq(1,1);
	dq=qd-qi;
	dq/=(0.5*n+5);  // Joint angle change at every time step.

	FILE * fil;
	fil = fopen("Ready2park.txt","w");
	fprintf(fil," qi is: \n\n");
	int k;
	for (j=0;j<qi.RowNo();j++){
		for (k=0;k<qi.ColNo();k++){
			fprintf(fil," %4.2f ",  qi(j,k));
		}
		fprintf(fil," \n");
	}
	fprintf(fil," \n\n\n");
	fprintf(fil," qd is: \n\n");
	for (j=0;j<qd.RowNo();j++){
		for (k=0;k<qd.ColNo();k++){
			fprintf(fil," %4.2f ",  qd(j,k));
		}
		fprintf(fil," \n");
	}
	fprintf(fil," \n\n\n");
	fprintf(fil," ts is %4.2f ",  ts);
	fprintf(fil," \n\n\n");
	fprintf(fil," n is %d ",  n);
	fprintf(fil," \n\n\n");
	fprintf(fil," dt is %4.2f ",  dt);
	fprintf(fil," \n\n\n");
	fprintf(fil," dq is: \n\n");
	for (j=0;j<dq.RowNo();j++){
		for (k=0;k<dq.ColNo();k++){
			fprintf(fil," %4.2f ",  dq(j,k));
		}
		fprintf(fil," \n");
	}
	fprintf(fil," \n\n\n");

	// Initializing the physical Arm:
	if (arm==1){
		zero.Null(10,1);
		for ( j = 0 ; j < 7 ; j++ ) {
			zero(j,0)=qi(j,0);
		}
		zero(7,0)=qiwc(0,0);
		zero(8,0)=qiwc(1,0);
		WMRA_ARM_Motion(ini, 2, zero, dt, g);
		ddt=0;
	}

	fprintf(fil," qi is: \n\n");
	for (j=0;j<qi.RowNo();j++){
		for (k=0;k<qi.ColNo();k++){
			fprintf(fil," %4.2f ",  qi(j,k));
		}
		fprintf(fil," \n");
	}
	fprintf(fil," \n\n\n");
	fprintf(fil," qiwc is: \n\n");
	for (j=0;j<qiwc.RowNo();j++){
		for (k=0;k<qiwc.ColNo();k++){
			fprintf(fil," %4.2f ",  qiwc(j,k));
		}
		fprintf(fil," \n");
	}
	fprintf(fil," \n\n\n");

	// Initializing Virtual Reality Animation:
	if (vr==1){
		/*zeros.Null(9,1);
		for ( j = 0 ; j < 7 ; j++ ) {
			zeros(j,0)=qi(j,0);
		}
		zeros(7,0)=qiwc(0,0);
		zeros(8,0)=qiwc(1,0);*/
	//	WMRA_VR_Animation(ini, Tiwc, qi);
	}

	// Initializing Robot Animation in Matlab Graphics:
	Matrix DH(1,1);
	Matrix T01(4,4), T12(4,4), T23(4,4), T34(4,4), T45(4,4), T56(4,4), T67(4,4);
	Matrix Ti(4,4), Td(4,4);
		
	if (ml==1){
		// Inputting the D-H Parameters in a Matrix form:
		DH=WMRA_DH(qi);
		
		// Calculating the transformation matrices of each link:		
		T01 = WMRA_rotx(DH(0,0))*WMRA_transl(DH(0,1),0,0)*WMRA_rotz(DH(0,3))*WMRA_transl(0,0,DH(0,2));
		//T2
		T12 = WMRA_rotx(DH(1,0))*WMRA_transl(DH(1,1),0,0)*WMRA_rotz(DH(1,3))*WMRA_transl(0,0,DH(1,2));
		//T3
		T23 = WMRA_rotx(DH(2,0))*WMRA_transl(DH(2,1),0,0)*WMRA_rotz(DH(2,3))*WMRA_transl(0,0,DH(2,2));
		//T4
		T34 = WMRA_rotx(DH(3,0))*WMRA_transl(DH(3,1),0,0)*WMRA_rotz(DH(3,3))*WMRA_transl(0,0,DH(3,2));
		//T5
		T45 = WMRA_rotx(DH(4,0))*WMRA_transl(DH(4,1),0,0)*WMRA_rotz(DH(4,3))*WMRA_transl(0,0,DH(4,2));
		//T6
		T56 = WMRA_rotx(DH(5,0))*WMRA_transl(DH(5,1),0,0)*WMRA_rotz(DH(5,3))*WMRA_transl(0,0,DH(5,2));
		//T7
		T67 = WMRA_rotx(DH(6,0))*WMRA_transl(DH(6,1),0,0)*WMRA_rotz(DH(6,3))*WMRA_transl(0,0,DH(6,2));
		// Calculating the Transformation Matrix of the initial and desired arm positions:
		Ti=Tiwc*T01*T12*T23*T34*T45*T56*T67;
		Td=Tiwc*WMRA_q2T(qd);
		//WMRA_ML_Animation(ini, Ti, Td, Tiwc, T01, T12, T23, T34, T45, T56, T67);
	}

	// Initialization:
	Matrix qo(1,1);
	float tt;
	qo=qi;
	tt=0;

	fprintf(fil," qo is: \n\n");
	for (j=0;j<qo.RowNo();j++){
		for (k=0;k<qo.ColNo();k++){
			fprintf(fil," %4.2f ",  qo(j,k));
		}
		fprintf(fil," \n");
	}
	fprintf(fil," \n\n\n");
	fprintf(fil," tt is %4.2f ",  tt);
	fprintf(fil," \n\n\n");
	
	Matrix qn(1,1);
	clock_t startt, endt, endf;
	double timedif; 
	Matrix T1a(1,1), T2a(1,1), T3a(1,1), T4a(1,1), T5a(1,1), T6a(1,1), T7a(1,1);
	
	qn.Null(7,1);
	while (tt <= ts) {
		// Starting a timer:
		startt=0;
		startt=clock()/ CLOCKS_PER_SEC;
		
		
		// Calculating the new Joint Angles:
		
		if (tt==0){
			qn=qo;
		}
		else if (tt < (dt*(0.5*n-5))){
			for (j=1;j<7;j++){
				qn(j,0)=qo(j,0)+dq(j,0);
			}			
		}
		else if (tt < (dt*(0.5*n+5))){
			qn=qo+dq;
		}
		else if (tt < (dt*(n-1))){
			qn(0,0)=qo(0,0)+dq(0,0);
		}

		fprintf(fil," qn is: \n\n");
		for (j=0;j<qn.RowNo();j++){
			for (k=0;k<qn.ColNo();k++){
				fprintf(fil," %4.2f ",  qn(j,k));
			}
			fprintf(fil," \n");
		}
		fprintf(fil," \n\n\n");
		fprintf(fil," ddt is %4.2f ",  ddt);
		fprintf(fil," \n\n\n");
		
		// Updating the physical Arm:
		if (arm==1) {
			float ddt2=0;
			ddt2=ddt+dt;
			if (ddt>=0.5 || tt>=ts){
				zero.Null(10,1);
				for ( j = 0 ; j < 7 ; j++ ) {
					zero(j,0)=qn(j,0);
				}
				zero(7,0)=qiwc(0,0);
				zero(8,0)=qiwc(1,0);
				WMRA_ARM_Motion(2, 1, zero, ddt2, g);
				ddt2=0;
			}
			ddt=0;
			ddt=ddt2;
		}
		
		// Updating Virtual Reality Animation:
		if (vr==1){
			/*zero.Null(9,1);
			for ( j = 0 ; j < 7 ; j++ ) {
				zero(j,0)=qn(j,0);
			}
			zero(7,0)=qiwc(0,0);
			zero(8,0)=qiwc(1,0);*/
			//WMRA_VR_Animation(2, Tiwc, zero);
		}
		
		// Updating Matlab Animation:
		if (ml==1){
			// Calculating the new Transformation Matrix:
			T1a = WMRA_rotx(DH(0,0))*WMRA_transl(DH(0,1),0,0)*WMRA_rotz(qn(0,0))*WMRA_transl(0,0,DH(0,2));
		//T2
		T2a = WMRA_rotx(DH(1,0))*WMRA_transl(DH(1,1),0,0)*WMRA_rotz(qn(1,0))*WMRA_transl(0,0,DH(1,2));
		//T3
		T3a = WMRA_rotx(DH(2,0))*WMRA_transl(DH(2,1),0,0)*WMRA_rotz(qn(2,0))*WMRA_transl(0,0,DH(2,2));
		//T4
		T4a = WMRA_rotx(DH(3,0))*WMRA_transl(DH(3,1),0,0)*WMRA_rotz(qn(3,0))*WMRA_transl(0,0,DH(3,2));
		//T5
		T5a = WMRA_rotx(DH(4,0))*WMRA_transl(DH(4,1),0,0)*WMRA_rotz(qn(4,0))*WMRA_transl(0,0,DH(4,2));
		//T6
		T6a = WMRA_rotx(DH(5,0))*WMRA_transl(DH(5,1),0,0)*WMRA_rotz(qn(5,0))*WMRA_transl(0,0,DH(5,2));
		//T7
		T7a = WMRA_rotx(DH(6,0))*WMRA_transl(DH(6,1),0,0)*WMRA_rotz(qn(6,0))*WMRA_transl(0,0,DH(6,2));
		
		//WMRA_ML_Animation(2, Ti, Td, Tiwc, T1a, T2a, T3a, T4a, T5a, T6a, T7a);
		}

		// Updating the old values with the new values for the next iteration:
		
		qo.Null(7,1);
		qo=qn;
		tt=tt+dt;

		fprintf(fil," qo is: \n\n");
		for (j=0;j<qo.RowNo();j++){
			for (k=0;k<qo.ColNo();k++){
				fprintf(fil," %4.2f ",  qo(j,k));
			}
			fprintf(fil," \n");
		}
		fprintf(fil," \n\n\n");
		fprintf(fil," tt is %4.2f ",  tt);
		fprintf(fil," \n\n\n");
		
		// Pausing for the speed sync:
		endt=0;
		endt=clock()/ CLOCKS_PER_SEC;
		timedif=0;
		timedif=endt-startt;
		wait((dt-timedif));
	}
	fclose(fil);
}
