/* This function gives the WMRA's base Jacobian Matrix based on the ground frame, given the Wheelchair orientation angle about the z axis.
Dimentions are as supplies, angles are in radians.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "WCD.h"
#include "matrix.h"
#include "Jga.h"
using namespace std;
using namespace math;

Matrix WMRA_Jga(int ind, float p, float X, float Y){

	Matrix Jga(6,2);
	Matrix L(1,1);
	// Reading the Wheelchair's constant dimentions, all dimentions are converted in millimeters:
	L=WMRA_WCD();
	int i;
	// Deciding if the motion is in reference to the arm base (1) or the wheel axle center (0):
	if (ind==0){
		for (i=1; i<4;i++){
			L(0,i)=0;
		}
	}
	
	// Calculating the Jacobian:
	Matrix Jtemp1(6,6),Jtemp2(6,2),Jtemp3(2,2);

	Jtemp1.Unit(6);
	Jtemp1(0,5)=-(X*sin(p)+Y*cos(p));
	Jtemp1(1,5)=X*cos(p)-Y*sin(p);

	Jtemp2.Null(6,2);
	Jtemp2(0,0)=cos(p)+2*(L(0,1)*sin(p)+L(0,2)*cos(p))/L(0,0);
	Jtemp2(0,1)=cos(p)-2*(L(0,1)*sin(p)+L(0,2)*cos(p))/L(0,0);
	Jtemp2(1,0)=sin(p)-2*(L(0,1)*cos(p)-L(0,2)*sin(p))/L(0,0);
	Jtemp2(1,1)=sin(p)+2*(L(0,1)*cos(p)-L(0,2)*sin(p))/L(0,0);
	Jtemp2(5,0)=-2/L(0,0);
	Jtemp2(5,1)=2/L(0,0);
	Jtemp2*=(L(0,4)/2);

	Jtemp3(0,0)=1;
	Jtemp3(0,1)=-L(0,0)/(2*L(0,4));
	Jtemp3(1,0)=1;
	Jtemp3(1,1)=L(0,0)/(2*L(0,4));

	Jga = Jtemp1 * Jtemp2 * Jtemp3;
	return Jga;
}