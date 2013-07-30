/* This function gives the DH-Parameters matrix to be used in the program. 
Modifying the parameters on this file is sufficient to change these dimention in all related programs.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%Modified By:Ana Catalina Torres%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "DH.h"  

using namespace math;
using namespace std;

Matrix WMRA_DH(Matrix q){
	
	Matrix DH1(7,4);

//Inputting the D-H Parameters in a Matrix form, dimensions are in millimeters and radians:

// Dimentions based on the actual physical arm:
	float DHtemp[7][4]={{-PI/2, 0,	102,  q(0,0)},
					{ PI/2, 0,	133,  q(1,0)},  
					{-PI/2, 0,  502,  q(2,0)},
					{ PI/2, 0,  90,  q(3,0)},
					{-PI/2, 0,  375,  q(4,0)},
					{ PI/2, 0,   0,   q(5,0)},
					{-PI/2, 0, 161+143, q(6,0)}};
			     
// Dimentions based on the Virtual Reality arm model:
/*  float DH[7][4]={{-PI/2, 0, 109.72, q(0,0)},
					{ PI/2, 0, 118.66, q(1,0)},  
					{-PI/2, 0, 499.67, q(2,0)},
					{ PI/2, 0, 121.78, q(3,0)},
					{-PI/2, 0, 235.67, q(4,0)},
					{ PI/2, 0,   0,    q(5,0)},
					{-PI/2, 0, 276.68, q(6,0)}};
*/
	int i,j;
	for (j=0; j < 4; j++){
		for (i=0; i < 7; i++){
			DH1(i,j) = DHtemp[i][j];				
		}
	}
	return DH1;
}
