/* This function gives the WMRA's errors from the current position to the required trajectory position.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%Modified By:Ana Catalina Torres%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "Delta.h" 
using namespace std;
using namespace math;

Matrix WMRA_delta(Matrix Tid, Matrix Tdd){

	Matrix delta(6,1), ep(3,1), eo(3,1);
	int i,j;
	for (i=0; i < 3; i++){
		ep(i,0) = Tdd(i,3)-Tid(i,3);
	}
	
	vector temp3 (0, 0, 0);
	for (j=0; j<3; j++){
		vector temp1 (Tid(0,j), Tid(1,j), Tid(2,j));
		vector temp2 (Tdd(0,j), Tdd(1,j), Tdd(2,j));
		vector crossed = crossProduct (temp1, temp2);
		temp3.x=temp3.x+crossed.x;
		temp3.y=temp3.y+crossed.y;
		temp3.z=temp3.z+crossed.z;
	}
	eo(0,0)=temp3.x;
	eo(1,0)=temp3.y;
	eo(2,0)=temp3.z;
	eo*=(0.5);

	// delta definition
	for (i=0;i<3;i++){
		delta(i,0)=ep(i,0); 
	}
	for (i=3;i<6;i++){
		delta(i,0)=eo(i-3,0);
	}

/*eo=0.5*( cross(Ti(1:3,1),Td(1:3,1)) + cross(Ti(1:3,2),Td(1:3,2)) + cross(Ti(1:3,3),Td(1:3,3)) );  
From equation 17 on page 189 of (Robot Motion Planning and Control) Book by Micheal Brady et al. Taken from the paper (Resolved-Acceleration Control of Mechanical Manipulators) By John Y. S. Luh et al.*/
	return delta;
}