/* This "new USF WMRA" function SIMULATES the arm going from the ready position to any position with ANIMATION. All angles are in Radians.
 The ready position is assumed to be qd=[pi/2;pi/2;0;pi/2;pi/2;pi/2;0] (Radians).
 ini=1 --> initialize animation figures, ini=2 or any --> just update the figures, ini=3 --> close the figures.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres & Punya A. Basnayaka%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "ready2any.h" 
#include "ArmMotion.h" 
#include "DH.h" 
#include "q2T.h" 
#include <time.h>
using namespace std;
using namespace math;

void WMRA_ready2any(int ini, int vr, int ml, int arm, Matrix Tiwc, Matrix qd, Galil &g){
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
	float qi2 [7]= {PI/2, PI/2, 0, PI/2, PI/2, PI/2, 0}; // Initial joint angles (Ready Position).
	Matrix qi(7,1);
	float ts, dt, ddt;
	int n, j;
	for ( j = 0 ; j < 7 ; j++ ) {
		qi(j,0)=qi2[j];
	}
	ts=10;  // (5 or 10 or 20) Simulation time to move the arm from any position to the ready position.
	n=100;  // Number of time steps.
	dt=ts/n;  // The time step to move the arm from any position to the ready position.

	// Initializing the physical Arm:
	if (arm==1){
		qi.SetSize(10,1);
		WMRA_ARM_Motion(ini, 2, qi, dt, g);
		ddt=0;
		qi.SetSize(7,1);
	}

	// Initializing Virtual Reality Animation:
	if (vr==1){
		//qi.SetSize(9,1);
	//	WMRA_VR_Animation(ini, Tiwc, qi);
		//qi.SetSize(7,1);
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

	// Check for the shortest route:
	Matrix diff(7,1);
	for ( j = 0 ; j < 7 ; j++ ) {
		diff(j,0)=qd(j,0)-qi(j,0);
	}
	for ( j = 0 ; j < 7 ; j++ ) {
		if (diff(j,0) > PI) {
			diff(j,0)=diff(j,0)-2*PI;
		}
		else if (diff(j,0) < (-PI)) {
			diff(j,0)=diff(j,0)+2*PI;
		}
	}

	// Joint angle change at every time step.
	diff/=(n);
	Matrix dq(7,1);
	for ( j = 0 ; j < 7 ; j++ ) {
		dq(j,0)=diff(j,0);
	}
	
	// Initialization:
	Matrix qo(1,1);
	float tt;
	qo=qi;
	tt=0;

	Matrix qn(1,1);
	clock_t startt, endt, endf;
	double timedif; 
	Matrix T1a(1,1), T2a(1,1), T3a(1,1), T4a(1,1), T5a(1,1), T6a(1,1), T7a(1,1);

	while (tt <= (ts-dt)) {
		// Starting a timer:
		startt=0;
		startt=clock()/ CLOCKS_PER_SEC;
		
		
		// Calculating the new Joint Angles:
		qn.Null(7,1);
		qn=qo+dq;
		
		// Updating the physical Arm:
		if (arm==1) {
			float ddt2;
			ddt2=0;
			ddt2=ddt+dt;
			if (ddt>=0.5 || tt>=(ts-dt)){
				qn.SetSize(10,1);
				WMRA_ARM_Motion(2, 1, qn, ddt2, g);
				ddt=0;
				qn.SetSize(7,1);
			}
			ddt=0;
			ddt=ddt2;
		}
		
		// Updating Virtual Reality Animation:
		if (vr==1){
			//qn.SetSize(9,1);
			//WMRA_VR_Animation(2, Tiwc, qn);
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
		
		// Pausing for the speed sync:
		endt=0;
		endt=clock()/ CLOCKS_PER_SEC;
		timedif=0;
		timedif=endt-startt;
		wait((dt-timedif));
	}
}
