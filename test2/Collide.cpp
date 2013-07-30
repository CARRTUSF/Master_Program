/* This function is to stop the arm if it is moving towards a collision with itself, the wheelchair, or the human user.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%Modified By:Ana Catalina Torres%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "Collide.h" 
#include "WCD.h"  
using namespace std;
using namespace math;

Matrix WMRA_collide(Matrix dqi, Matrix T01, Matrix T12, Matrix T23, Matrix T34, Matrix T45, Matrix T56, Matrix T67){

	// Reading the Wheelchair's constant dimentions, all dimentions are converted in millimeters:
	Matrix L(1,1);
	L=WMRA_WCD();

	// Collision Conditions:
	int gr;
	Matrix dq(1,1);
	gr=100-L(0,3)-L(0,4); // The ground buffer surface.
	dq=dqi;

	// 1- Collision of frame 3 using T03:
	Matrix T03(1,1);
	T03=T01*T12*T23;
	// Collision with the ground:
	if (T03(2,3) <= gr){
		dq=dqi;
		dq*=(-0.01);
	}
	// Collision with the wheelchair's front left side:
	if (T03(0,3) >= 450 && T03(1,3) <= -150){
		dq=dqi;
		dq*=(-0.01);
	}
	// Collision with the wheelchair's rear left side:
	if (T03(0,3) <= 450 && T03(1,3) <= 100){
		dq=dqi;
		dq*=(-0.01);
	}
	// Collision with the wheelchair's rear left wheel:
	if (T03(0,3) <= 0 && T03(1,3) <= 100 && T03(2,3) <= 120){
		dq=dqi;
		dq*=(-0.01);
	}

	// 2- Collision of frame 4 using T04:
	Matrix T04(1,1);
	T04=T03*T34;
	// Collision with the ground:
	if (T04(2,3) <= gr){
		dq=dqi;
		dq*=(-0.01);
	}
	// Collision with the wheelchair's front left side:
	if (T04(0,3) <= 450 && T04(0,3) >= -100 && T04(1,3) <= 0){
		dq=dqi;
		dq*=(-0.01);
	}
	// Collision with the wheelchair's rear left side:
	if (T04(0,3) <= -100 && T04(1,3) <= 100){
		dq=dqi;
		dq*=(-0.01);
	}
	// Collision with the wheelchair's rear left wheel:
	if (T04(0,3) <= -100 && T04(1,3) <= 100 && T04(2,3) <= 120){
		dq=dqi;
		dq*=(-0.01);
	}

	// 3- Collision of frame 5 using T05:
	Matrix T05(1,1);
	T05=T04*T45;
	// Collision with the ground:
	if (T05(2,3) <= gr){
		dq=dqi;
		dq*=(-0.01);
	}
	// Collision with the wheelchair driver's left shoulder:
	if (T05(0,3) <= -100 && T05(0,3) >= -550 && T05(1,3) <= 150){
		dq=dqi;
		dq*=(-0.01);
	}
	// Collision with the wheelchair driver's lap:
	if (T05(0,3) <= 400 && T05(0,3) >= -100 && T05(1,3) <= 0 && T05(2,3) <= 470){
		dq=dqi;
		dq*=(-0.01);
	}
	// Collision with the wheelchair's battery pack:
	if (T05(0,3) <= -430 && T05(0,3) >= -630 && T05(1,3) <= 100 && T05(2,3) <= 50){
		dq=dqi;
		dq*=(-0.01);
	}

	// 4- Collision of frame 7 using T07:
	Matrix T07(1,1);
	T07=T05*T56*T67;
	// Collision with the ground:
	if (T07(2,3) <= gr){
		dq=dqi;
		dq*=(-0.01);
	}
	// Collision with the wheelchair driver's left shoulder:
	if (T07(0,3) <= -50 && T07(0,3) >= -600 && T07(1,3) <= 200){
		dq=dqi;
		dq*=(-0.01);
	}
	// Collision with the wheelchair driver's lap:
	if (T07(0,3) <= 450 && T07(0,3) >= -50 && T07(1,3) <= 50 && T07(2,3) <= 520){
		dq=dqi;
		dq*=(-0.01);
	}
	// Collision with the wheelchair's battery pack:
	if (T07(0,3) <= -480 && T07(0,3) >= -680 && T07(1,3) <= 50 && T07(2,3) <= 100){
		dq=dqi;
		dq*=(-0.01);
	}

	// 5- Collision of the arm and itself using T37:
	Matrix T37(1,1);
	T37=T34*T45*T56*T67;
	// Collision between the forearm and the upper arm:
	if (T37(0,3) <= 170 && T37(0,3) >= -170 && T37(1,3) >= -100 && T37(2,3) <= 0){
		dq=dqi;
		dq*=(-0.01);
	}

	return dq;
}