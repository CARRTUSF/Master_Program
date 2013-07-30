/* This function gives the Transformation Matrix of the wheelchair with 2 DOF (Ground to WMRA base), given the previous transformation matrix and the required wheelchair's travel distance and angle.
Dimentions are as supplies, angles are in radians.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "vector.h"
#include "WCD.h"
#include "w2T.h"
using namespace std;
using namespace math;

Matrix WMRA_w2T(int ind, Matrix Tp, Matrix q){

	Matrix T(4,4);
	Matrix L(1,1);
	// Reading the Wheelchair's constant dimentions, all dimentions are converted in millimeters:
	L=WMRA_WCD();
	// Deciding if the motion is in reference to the arm base (1) or the wheel axle center (0):
	int i;	
	if (ind==0){
		for (i=1; i<4; i++){
			L(0,i) = 0;
		}
	}
	
	// Defining the inverse of Transformation Matrix between the wheelchair center and the WMRA's base:
	Matrix Twa(4,4), Twainv(4,4);
	Twa.Unit(4);
	for (i=0; i<3; i++){
		Twa(i,3) = L(0,i+1);
	}
	
	// The previous transformation matrix from the ground to the wheelchair center:
	Twainv = !Twa;
	Tp = Tp * Twainv;

	// Defining the Transformation Matrix between the ground and the wheelchair center and WMRA's base:
	if (abs(q(1,0))<=EPS){          // Streight line motion.
		for (i=0; i<2; i++){
			Tp(i,3)= Tp(i,3) + q(0,0)*Tp(i,0);
		}
		T = Tp * Twa;
	}
	else {
		float po, p, r;
		po=atan2(Tp(1,0),Tp(0,0));
		p=q(1,0);
		r=q(0,0)/p-L(0,0)/2;
		Matrix Tgw(4,4);
		Tgw.Unit(4);
		Tgw(0,0)= cos(po+p);
		Tgw(0,1)= -sin(po+p);
		Tgw(0,3)= Tp(0,3)+sin(PI/2+po+p/2)*(r+L(0,0)/2)*sin(p)/cos(p/2);
		Tgw(1,0)= sin(po+p);
		Tgw(1,1)= cos(po+p);
		Tgw(1,3)= Tp(1,3)-cos(PI/2+po+p/2)*(r+L(0,0)/2)*sin(p)/cos(p/2);
		Tgw(2,3)= Tp(2,3);

		T= Tgw * Twa;
	}

	return T;
}
