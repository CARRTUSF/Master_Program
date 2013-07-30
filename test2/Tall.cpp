/* This function is for getting the transformations of the USF WMRA with 9 DOF.
q is for the 7 joints in radians, and dq is for the wheelchair only in millimeters and radians.

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
#include "w2T.h"
#include "Tall.h"
using namespace std;
using namespace math;

void WMRA_Tall(int i, Matrix q, Matrix dq, Matrix Twc, Matrix& T, Matrix& Ta, Matrix& Twco, Matrix& T1, Matrix& T2, Matrix& T3, Matrix& T4, Matrix& T5, Matrix& T6, Matrix& T7){
	 
	// Inputting the D-H Parameters in a Matrix form:
	extern Matrix DH;
	
	if (i==1){
		DH=WMRA_DH(q);
		//Calculating the transformation matrices of each link:
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

		//Calculating the Transformation Matrix of the initial arm position:

		Ta=T1*T2*T3*T4*T5*T6*T7;
		
		//Calculating the Transformation Matrix of the initial WMRA system position:
		Twco=Twc;
		T=Twc*Ta;
	}
	else{
		
		//T1
		T1 = WMRA_rotx(DH(0,0))*WMRA_transl(DH(0,1),0,0)*WMRA_rotz(q(0,0))*WMRA_transl(0,0,DH(0,2));
		//T2
		T2 = WMRA_rotx(DH(1,0))*WMRA_transl(DH(1,1),0,0)*WMRA_rotz(q(1,0))*WMRA_transl(0,0,DH(1,2));
		//T3
		T3 = WMRA_rotx(DH(2,0))*WMRA_transl(DH(2,1),0,0)*WMRA_rotz(q(2,0))*WMRA_transl(0,0,DH(2,2));
		//T4
		T4 = WMRA_rotx(DH(3,0))*WMRA_transl(DH(3,1),0,0)*WMRA_rotz(q(3,0))*WMRA_transl(0,0,DH(3,2));
		//T5
		T5 = WMRA_rotx(DH(4,0))*WMRA_transl(DH(4,1),0,0)*WMRA_rotz(q(4,0))*WMRA_transl(0,0,DH(4,2));
		//T6
		T6 = WMRA_rotx(DH(5,0))*WMRA_transl(DH(5,1),0,0)*WMRA_rotz(q(5,0))*WMRA_transl(0,0,DH(5,2));
		//T7
		T7 = WMRA_rotx(DH(6,0))*WMRA_transl(DH(6,1),0,0)*WMRA_rotz(q(6,0))*WMRA_transl(0,0,DH(6,2));

		//Calculating the Transformation Matrix of the initial arm position:

		Ta=T1*T2*T3*T4*T5*T6*T7;
		Twc = WMRA_w2T(1, Twc, dq);
		Twco=Twc;
		T=Twc*Ta;
	}
}
