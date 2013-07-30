/* This function gives the wheelchair dimentions matrix to be used in the program. 
Modifying the dimentons on this file is sufficient to change these dimention in all related programs.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "WCD.h" 
using namespace std;
using namespace math;

Matrix WMRA_WCD(){

	Matrix L(1,5);
	int Ltemp[5] = {500,400,230,200,170};
	int i;
	for (i=0; i < 5; i++){
		L(0,i) = Ltemp[i];
	}

	// All dimentions are in millimeters.
	//L(0,0)=560;  // Distance between the two driving wheels.
	//L(0,1)=440;  // Horizontal distance between the wheels axix of rotation and the arm mounting position (along x).
	//L(0,2)=230;  // Horizontal distance between the middle point between the two driving wheels and the arm mounting position (along y).
	//L(0,3)=182;  // Vertical distance between the wheels axix of rotation and the arm mounting position (along z).
	//L(0,4)=168;  // Radius of the driving wheels.
		
	return L;
}
