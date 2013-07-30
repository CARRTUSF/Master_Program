/* This function finds the trajectory points along a streight line, given the initial and final transformations. Single-angle rotation about a single axis is used
See Eqs. 1.73-1.103 pages 30-32 of Richard Paul's book " Robot Manipulators"

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "vector.h"
#include "Polynomial.h"
#include "Linear.h"
#include "BPolynomial.h"
#include "sign.h"
#include "Traj.h"
using namespace std;
using namespace math;

float ***WMRA_traj(int ind, Matrix Ti, Matrix Td, int n){

	float ***Tt;
	
	//Finding the rotation of the desired point based on the initial point:
	Matrix R(3,3), Titemp(3,3),Tdtemp(3,3);
	int i,j,m;
	for ( i=0 ; i < 3 ; i++ ) {
		for ( j = 0 ; j < 3 ; j++ ) {
			Titemp(i,j)=Ti(i,j);
			Tdtemp(i,j)=Td(i,j);
		}
	} 
	Titemp=~Titemp;
	R=Titemp*Tdtemp;
	
	//Initial single-angle representation of the rotation:
	float a, s, c, v;
	a=atan2(sqrt(pow((R(2,1)-R(1,2)),2)+pow((R(0,2)-R(2,0)),2)+pow((R(1,0)-R(0,1)),2)),(R(0,0)+R(1,1)+R(2,2)-1));
	s=sin(a);
	c=cos(a);
	v=1-c;
	
	//Finding the single-vector components for the rotation:
	float kx, ky, kz;
	if (a<0.001){
		kx=1;
		ky=0;
		kz=0;
	}
	else if (a<PI/2+0.001){
		kx=(R(2,1)-R(1,2))/(2*s);
		ky=(R(0,2)-R(2,0))/(2*s);
		kz=(R(1,0)-R(0,1))/(2*s);
	} 
	else {
		kx=sign((R(2,1)-R(1,2)))*sqrt((R(0,0)-c)/v);
		ky=sign((R(0,2)-R(2,0)))*sqrt((R(1,1)-c)/v);
		kz=sign((R(1,0)-R(0,1)))*sqrt((R(2,2)-c)/v);
		if (kx>ky && kx>kz){
			ky=(R(1,0)+R(0,1))/(2*kx*v);
			kz=(R(0,2)+R(2,0))/(2*kx*v);
		}
		else if (ky>kx && ky>kz){
			kx=(R(1,0)+R(0,1))/(2*ky*v);
			kz=(R(2,1)+R(1,2))/(2*ky*v);
		}
		else {
			kx=(R(0,2)+R(2,0))/(2*kz*v);
			ky=(R(2,1)+R(1,2))/(2*kz*v);
		}
	}
	
	// Running the desired trajectory method: 
	// 1 == Polynomial with Blending function,
	// 2 == Polynomial without Blending function,
	// 3 == Linear function.
	Matrix at(n,1), xt(n,1), yt(n,1), zt(n,1);
	Titemp=~Titemp;
	if (ind == 2){
		at=WMRA_Polynomial(0,a,n);
		xt=WMRA_Polynomial(Ti(0,3), Td(0,3), n);
		yt=WMRA_Polynomial(Ti(1,3), Td(1,3), n);
		zt=WMRA_Polynomial(Ti(2,3), Td(2,3), n);
	}
	else if (ind == 3) {
		at=WMRA_Linear(0,a,n);
		xt=WMRA_Linear(Ti(0,3), Td(0,3), n);
		yt=WMRA_Linear(Ti(1,3), Td(1,3), n);
		zt=WMRA_Linear(Ti(2,3), Td(2,3), n);
	}
	else {
		at=WMRA_BPolynomial(0,a,n);
		xt=WMRA_BPolynomial(Ti(0,3), Td(0,3), n);
		yt=WMRA_BPolynomial(Ti(1,3), Td(1,3), n);
		zt=WMRA_BPolynomial(Ti(2,3), Td(2,3), n);
	}
	
	Tt = new float**[n];
	for (i = 0; i < n; ++i) {
		Tt[i] = new float*[4];
		for (j = 0; j < 4; ++j){
			Tt[i][j] = new float[4];
		}
	}
	for ( i=0 ; i < 4 ; i++ ) {
		for ( j = 0 ; j < 4 ; j++ ) {
			Tt[0][i][j]=Ti(i,j);
		}
	}

	for (i=1; i<n; i++){
		// Single-angle Change:
		float da;
		da=at(i,0)-at(0,0);
		s=sin(da);
		c=cos(da);
		v=1-c;
		// Rotation and Position Change:
		
		Matrix dR(3,3);
		dR(0,0)=pow(kx,2)*v+c;
		dR(0,1)=kx*ky*v-kz*s;
		dR(0,2)=kx*kz*v+ky*s;
		dR(1,0)=kx*ky*v+kz*s;
		dR(1,1)=pow(ky,2)*v+c;
		dR(1,2)=ky*kz*v-kx*s;
		dR(2,0)=kx*kz*v-ky*s;
		dR(2,1)=ky*kz*v+kx*s;
		dR(2,2)=pow(kz,2)*v+c;
		
		//Finding the trajectory points along the trajectory line:
		Matrix Tti1(3,3),Tti(4,4);
		Tti1 = Titemp * dR;
		Tti.Unit(4);
		for ( m=0 ; m < 3 ; m++ ) {
			for ( j = 0 ; j < 3 ; j++ ) {
				Tti(m,j)=Tti1(m,j);
			}
		}
		Tti(0,3)=xt(i,0);
		Tti(1,3)=yt(i,0);
		Tti(2,3)=zt(i,0);
		
		for ( m=0 ; m < 4 ; m++ ) {
			for ( j = 0 ; j < 4 ; j++ ) {
				Tt[i][m][j]=Tti(m,j);
			}
		}
	}
	
	/*//Rotational Trajectory:
	// Single-angle Change:
	
	da=2*PI/(n-1);
	kx=1; 
	ky=0;
	kz=0;
	s=sin(da);
	c=cos(da);
	v=1-c;
	
	//Rotation and Position Change:

	Matrix dR(3,3);
		dR(0,0)=pow(kx,2)*v+c;
		dR(0,1)=kx*ky*v-kz*s;
		dR(0,2)=kx*kz*v+ky*s;
		dR(1,0)=kx*ky*v+kz*s;
		dR(1,1)=pow(ky,2)*v+c;
		dR(1,2)=ky*kz*v-kx*s;
		dR(2,0)=kx*kz*v-ky*s;
		dR(2,1)=ky*kz*v+kx*s;
		dR(2,2)=pow(kz,2)*v+c;
		
	// Finding the trajectory points along the trajectory line:
	Tt = new float**[n];
	for (i = 0; i < n; ++i) {
		Tt[i] = new float*[4];
		for (j = 0; j < 4; ++j){
			Tt[i][j] = new float[4];
		}
	}
	for ( i=0 ; i < 4 ; i++ ) {
		for ( j = 0 ; j < 4 ; j++ ) {
			Tt[0][i][j]=Ti(i,j);
		}
	}
	
	Matrix Tti1(3,3),Tti(4,4);
	for (i=1; i<n; i++){
		Titemp=~Titemp;
		dR= dR ^ (i)
		Tti1 = Titemp * dR;
		for ( m=0 ; m < 3 ; m++ ) {
			for ( j = 0 ; j < 3 ; j++ ) {
				Tti(m,j)=Tti1(m,j);
			}
		}
		Tti(0,3)=Ti(0,3)+2000*cos(i*da);
		Tti(1,3)=Ti(1,3)+2000*sin(i*da);
		Tti(2,3)=Ti(2,3);
		Tti(3,0)=0;
		Tti(3,1)=0;
		Tti(3,2)=0;
		Tti(3,3)=1;

		for ( m=0 ; m < 4 ; m++ ) {
			for ( j = 0 ; j < 4 ; j++ ) {
				Tt[i][m][j]=Tti(m,j);
			}
		}
	}
*/
	return Tt;
}