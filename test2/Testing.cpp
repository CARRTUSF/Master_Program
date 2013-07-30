/* This "new USF WMRA" script SIMULATES the WMRA system with ANIMATION and plots for 9 DOF. All angles are in Radians.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2007 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "matrix.h"
#include "var_included.h"
#include "ArmMotion.h"
#include "BPolynomial.h"
#include "Collide.h"
#include "Delta.h"
#include "DH.h"
#include "J07.h"
#include "Jga.h"
#include "Jlimit.h"
#include "Linear.h"
#include "Opt.h"
#include "p2T.h"
#include "Polynomial.h"
#include "q2T.h"
#include "Rotx.h"
#include "Roty.h"
#include "Rotz.h"
#include "sign.h"
#include "T2rpy.h"
#include "Tall.h"
#include "Traj.h"
#include "Transl.h"
#include "w2T.h"
#include "WCD.h"
#include <math.h>
#include <time.h>
using namespace std;
using namespace math;
using namespace System;
#define PI 3.14159265
#define d2r PI/180 //Conversions from Degrees to Radians.
#define r2d 180/PI //Conversions from Radians to Degrees.

// Declaring the global variables to be used for the touch screen control:
//global VAR_DX
//global VAR_SCREENOPN

int main(){
	
	Matrix L(1,1);
	// Reading the Wheelchair's constant dimentions, all dimentions are converted in millimeters:
	L=WMRA_WCD();
	
	// User input prompts:
	int choice000, choice00000, choice0000, choice5, choice50, choice500, choice0, choice00, choice1, choice10, choice2, choice3, choice4, choice6, choice7, choice8;
	int WCA, coord, cart, optim, JLA, JLO, cont, trajf, vr, ml, arm, ini, plt;
	int j, k;
	int port1, ts;
	float v;
	Matrix Td(4,4), Vd(3,1), qi(7,1), WCi(3,1);
	int Tdte[4][4]={{0, 0, 1, 1455},{-1, 0, 0, 369},{0, -1, 0, 999},{ 0, 0, 0, 1}};
	for (j=0;j<4;j++){
		for (k=0;k<4;k++){
			Td(j,k)=Tdte[j][k];
		}
	}
topchoice:;			
	WCA=1;
top8:;
	coord=1;
	cont=1;
	cart=1;
	optim=1;
	JLA=1;
	JLO=1;
	ini=0;
topchoice2:;
	v=50;
top13:;
	trajf=1;
	vr=0; ml=0;

	arm=0;
	float qi2 [7]= {90, 90, 0, 90, 90, 90, 0};
		for ( j = 0 ; j < 7 ; j++ ) {
			qi(j,0)=qi2[j]*d2r;
		}
		WCi.Null(3,1);
	plt=1;
	
	// Calculating the Transformation Matrix of the initial position of the WMRA's base:
	Matrix Tiwc(4,4), qiwc(2,1);
	Tiwc = WMRA_p2T(WCi(0,0),WCi(1,0),WCi(2,0));
	cout<<"Tiwc\n"<<Tiwc<<"\n\n";
	
	//Calculating the initial Wheelchair Variables:
	qiwc(0,0)=sqrt(pow(WCi(0,0),2)+pow(WCi(1,0),2));
	qiwc(1,0)=WCi(2,0);
	cout<<"qiwc\n"<<qiwc<<"\n\n";

	//Calculating the initial transformation matrices:
	Matrix dq(1,1), Ti(4,4), Tia(4,4), Tiwco(4,4), T01(4,4), T12(4,4), T23(4,4), T34(4,4), T45(4,4), T56(4,4), T67(4,4), Unit(1,1);
	dq.Null(2,1);
	WMRA_Tall(1, qi, dq, Tiwc, Ti, Tia, Tiwco, T01, T12, T23, T34, T45, T56, T67);
	Tiwc=Tiwco;
	cout<<"\nTi\n\n"<<Ti<<"\n\n";
	cout<<"\nTia\n\n"<<Tia<<"\n\n";
	cout<<"\nTiwc\n\n"<<Tiwc<<"\n\n";
	cout<<"\nT01\n\n"<<T01<<"\n\n";
	cout<<"\nT12\n\n"<<T12<<"\n\n";
	cout<<"\nT23\n\n"<<T23<<"\n\n";
	cout<<"\nT34\n\n"<<T34<<"\n\n";
	cout<<"\nT45\n\n"<<T45<<"\n\n";
	cout<<"\nT56\n\n"<<T56<<"\n\n";
	cout<<"\nT67\n\n"<<T67<<"\n\n";

	float D, dt, total_time, n, dg;
	Matrix dx(1,1);
	D=sqrt(pow(Td(0,3)-Ti(0,3),2) + pow(Td(1,3)-Ti(1,3),2) + pow(Td(2,3)-Ti(2,3),2));
	cout<<"\nD\n"<<D<<"\n\n";
	//Calculating the number of iteration and the time increment (delta t) if the linear step increment of the tip is 1 mm:
		dt=0.05;    // Time increment in seconds.
		total_time=D/v;     // Total time of animation.
		n=Math::Round(total_time/dt); // Number of iterations rounded up.
		dt=total_time/n;    // Adjusted time increment in seconds.
		cout<<"\nTotal time\n"<<total_time<<"\n\n";
		cout<<"\nn\n"<<n<<"\n\n";
		cout<<"\ndt\n"<<dt<<"\n\n";
		float ***Tt;
		Tt = new float**[n];
		for (j = 0; j < n; ++j) {
			Tt[j] = new float*[4];
			for (k = 0; k < 4; ++k){
				Tt[j][k] = new float[4];
			}
		}
		
		// Calculating the Trajectory of the end effector, and once the trajectory is calculated, we should redefine "Td" based on the ground frame:
		int i;
		Tt=WMRA_traj(trajf,Ti,Td,n);
		FILE * fid;
			fid = fopen("traj.txt","a");
			for (i=0;i<n;i++){
			for (j=0;j<4;j++){
				for (k=0;k<4;k++){
					fprintf(fid," %4.f ",  Tt[i][j][k]);
				}
				fprintf(fid," \n");
			}
				fprintf(fid," \n\n\n");
			}
			fprintf(fid," \n\n\n");
			fclose (fid);

		// Initializing the joint angles, the Transformation Matrix, and time:
	Matrix qo(9,1), To(4,4), Towc(4,4), Toa(4,4), Jo(1,1);
	float tt, ddt;
	
	dq.Null(9,1);
	cout<<"\n\ndq\n\n"<<dq<<"\n\n";
	dg=0;
	for (j=0; j<7; j++){
		qo(j,0)=qi(j,0);
	}
	for (j=7; j<9; j++){
		qo(j,0)=qiwc(j-7,0);
	}
	cout<<"\n\nqo\n\n"<<qo<<"\n\n";
	To=Ti;
	cout<<"\n\nTo\n\n"<<To<<"\n\n";
	Toa=Tia;
	cout<<"\n\nToa\n\n"<<Toa<<"\n\n";
	Towc=Tiwc;
	cout<<"\n\nTowc\n\n"<<Towc<<"\n\n";
	tt=0;
	i=0;
	dHo.Null(7,1);
	
			// Re-Drawing the Animation:
		Matrix Joa(6,7), Jowc(1,1), Jowctemp(1,1), Joatemp(1,1);
		float detJoa, detJo, phi;
		
		// Starting a timer:
		clock_t tic, toc;
		tic=clock();
		
		float ***Ttnew;
		Ttnew = new float**[n];
		for (j = 0; j < n; ++j) {
			Ttnew[j] = new float*[4];
			for (k = 0; k < 4; ++k){
				Ttnew[j][k] = new float[4];
			}
		}

		// Starting the Iteration Loop:
		while (i<1){
			cout<<"\n\ni\n\n"<<i;
			// Calculating the 6X7 Jacobian of the arm in frame 0:
			WMRA_J07(T01, T12, T23, T34, T45, T56, T67, Joa, detJoa);
			FILE * fid;
			fid = fopen("tryJ.txt","a");
			for (j=0;j<6;j++){
				for (k=0;k<7;k++){
					fprintf(fid," %4.f ",  Joa(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," %g",  detJoa);
			fprintf(fid," \n\n\n");
			fclose (fid);
			// Calculating the 6X2 Jacobian based on the WMRA's base in the ground frame:
			phi=atan2(Towc(1,0),Towc(0,0));
			cout<<"\n\nphi\n\n"<<phi;
			Jowc=WMRA_Jga(1, phi, Toa(0,3), Toa(1,3));
			cout<<"\n\nJowc\n\n"<<Jowc;
			// Changing the Jacobian reference frame based on the choice of which coordinates frame are referenced in the Cartesian control:
			// coord=1 for Ground Coordinates Control.
			// coord=2 for Wheelchair Coordinates Control.
			// coord=3 for Gripper Coordinates Control.
			Joatemp.Null(6,6);
				for (j=0; j<3; j++){
					for (k=0; k<3; k++){
						Joatemp(j,k)=Towc(j,k);
					}
				}
				for (j=3; j<6; j++){
					for (k=3; k<6; k++){
						Joatemp(j,k)=Towc(j-3,k-3);
					}
				}
				Joatemp=Joatemp*Joa;
				Joa=Joatemp;
				cout<<"\n\nJoa\n\n"<<Joa;
				Jowc=Jowc;
				cout<<"\n\nJowc\n\n"<<Jowc;
			
				Jo.SetSize(6,9);
				for (j=0; j<6; j++){
					for (k=0; k<7; k++){
						Jo(j,k)=Joa(j,k);
					}
					for (k=7; k<9; k++){
						Jo(j,k)=Jowc(j,k-7);
					}
				}
				
				Joatemp=~Jo;
				Joatemp=Jo*Joatemp;
				detJo=sqrt(Joatemp.Det());	
				cout<<"\n\nJo\n\n"<<Jo;
				cout<<"\n\ndetJo\n\n"<<detJo;
				
			
			// Finding the Cartesian errors of the end effector:
			Matrix invTowc(1,1), invTo(1,1), Towctemp(3,3), Towctemp2(3,1), Tttemp(4,4), Ttnew1(4,4);
				for (j=0; j<4; j++){
						for (k=0; k<4; k++){
							Tttemp(j,k)=Tt[i][j][k];	
						}
					}	
					dx=WMRA_delta(To, Tttemp);
					cout<<"\n\ndx\n\n"<<dx;

				dq=WMRA_Opt(optim, JLA, JLO, Jo, detJo, dq, dx, dt, qo);
				cout << "\n\ndq is \n \n" << dq  ;
			
						
			Matrix qn(1,1), Tn(4,4), Tna(4,4), Tnwc(4,4);
			// Calculating the new Joint Angles:
			qn=qo+dq;
			cout<<"\n\nqn\n\n"<<qn;
			// Calculating the new Transformation Matrices:
			Matrix dqtemp(2,1);
			dqtemp(0,0)=dq(7,0);
			dqtemp(1,0)=dq(8,0);
			WMRA_Tall(2, qn, dqtemp, Towc, Tn, Tna, Tnwc, T01, T12, T23, T34, T45, T56, T67);
			cout<<"\n\nTn\n\n"<<Tn;
			cout<<"\n\nTna\n\n"<<Tna;
			cout<<"\n\nTnwc\n\n"<<Tnwc;

			// A safety condition function to stop the joints that may cause colision of the arm with itself, the wheelchair, or the human user:
			if (JLO==1 && WCA!=3){
				Matrix dqtemp(2,1);
				dqtemp(0,0)=dq(7,0);
				dqtemp(1,0)=dq(8,0);
				dq.SetSize(7,1);
				dq=WMRA_collide(dq, T01, T12, T23, T34, T45, T56, T67);
				// Re-calculating the new Joint Angles:
				dq.SetSize(9,1);
				cout<<"\n\ndq\n\n"<<dq;
				qn=qo+dq;
				cout<<"\n\nqn\n\n"<<qn;
				dqtemp(0,0)=dq(7,0);
				dqtemp(1,0)=dq(8,0);
				// Re-calculating the new Transformation Matrices:				
				WMRA_Tall(2, qn, dqtemp, Towc, Tn, Tna, Tnwc, T01, T12, T23, T34, T45, T56, T67);
				
			}
			
			// Saving the plot data in case plots are required:
			
			// Updating the old values with the new values for the next iteration:
			qo=qn;
			To=Tn;
			Toa=Tna;
			Towc=Tnwc;
			tt=tt+dt;
			i=i+1;
			cout<<"\n\nTo\n\n"<<To;
			cout<<"\n\nToa\n\n"<<Toa;
			cout<<"\n\nTowc\n\n"<<Towc;
						
			// Delay to comply with the required speed:
			if (tic < tt){
			//	Sleep(tt-tic); 
			}
		}
		
		

		
		// Reading the elapsed time and printing it with the simulation time:
		if (cont==1 || cont==2){
			printf("\nSimula. time is %6.6f seconds.\n", total_time);
		}
		toc=clock()-tic / ((double)CLOCKS_PER_SEC);
		printf("\nElapsed time is %6.6f seconds.\n", toc);
						
		// Plotting:
		
  		return 0;
}
