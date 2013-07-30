/* This function gives the Transformation Matrix of the new USF WMRA with 7 DOF, given the joint angles in Radians.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "vector.h"
#include "DH.h"
#include "Rotx.h"
#include "Rotz.h"
#include "Transl.h"
#include "q2T.h"
#include <time.h>

using namespace std;
using namespace math;

void wait ( int seconds )
{
	clock_t endwait;
	endwait = clock () + seconds * CLOCKS_PER_SEC ;
	while (clock() < endwait) {}
}

Matrix WMRA_q2T(Matrix q){

	Matrix T(4,4);

	// Inputting the D-H Parameters in a Matrix form:
	Matrix DH(1,1);
	DH=WMRA_DH(q);	
	// Calculating the transformation matrices of each link:
	Matrix T1(4,4),T2(4,4),T3(4,4),T4(4,4),T5(4,4),T6(4,4),T7(4,4),Tttemp(4,4);
	//T1
	T1 = WMRA_rotx(DH(0,0))*WMRA_transl(DH(0,1),0,0)*WMRA_rotz(DH(0,3))*WMRA_transl(0,0,DH(0,2));
	//T2
	T2 = WMRA_rotx(DH(1,0))*WMRA_transl(DH(1,1),0,0)*WMRA_rotz(DH(1,3))*WMRA_transl(0,0,DH(1,2));
	//T3
	T3 = WMRA_rotx(DH(2,0))*WMRA_transl(DH(2,1),0,0)*WMRA_rotz(DH(2,3))*WMRA_transl(0,0,DH(2,2));
	//T4
	T4 = WMRA_rotx(DH(3,0))*WMRA_transl(DH(3,1),0,0)*WMRA_rotz(DH(3,3))*WMRA_transl(0,0,DH(3,2));
	//T5
	T5 = WMRA_rotx(DH(4,0))*WMRA_transl(DH(4,1),0,0)*WMRA_rotz(DH(4,3))*WMRA_transl(0,0,DH(4,2));
	//T6
	T6 = WMRA_rotx(DH(5,0))*WMRA_transl(DH(5,1),0,0)*WMRA_rotz(DH(5,3))*WMRA_transl(0,0,DH(5,2));
	//T7
	T7 = WMRA_rotx(DH(6,0))*WMRA_transl(DH(6,1),0,0)*WMRA_rotz(DH(6,3))*WMRA_transl(0,0,DH(6,2));
		
	//Calculating the total Transformation Matrix of the given arm position:
	T=T1*T2*T3*T4*T5*T6*T7;

	return T;
}
