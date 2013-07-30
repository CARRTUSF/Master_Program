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

topchoice:;			
	cout << "\n Choose what to control: \n   For combined Wheelchair and Arm control, press '1',\n   For Arm only control, press '2',\n   For Wheelchair only control, press '3'. \n\n ";
	cin >> choice000;
	if (choice000==2){
		WCA=2;
top1:;
		cout << "\n Choose whose frame to base the control on: \n   For Ground Frame, press '1', \n   For Wheelchair Frame, press '2', \n   For Gripper Frame, press '3'. \n\n ";
		cin >> choice00000;
		if (choice00000==2){
			coord=2;
		}
		else if (choice00000==3){
			coord=3;
		}
		else if (choice00000==1){
			coord=1;
		}
		else {
			cout << "\n Invalid choice. Try again. \n\n ";
			goto top1;
		}
top2:;
		cout << "\n Choose the cartesian coordinates to be controlled: \n   For Position and Orientation, press '1', \n   For Position only, press '2'. \n\n ";
		cin >> choice0000;
		if (choice0000==2){
			cart=2;
		}
		else if (choice0000==1) {
			cart=1;
		}
		else {
			cout << "\n Invalid choice. Try again. \n\n ";
			goto top2;
		}
top3:;
		cout << "\n Please enter the desired optimization method: (1= SR-I & WLN, 2= P-I & WLN, 3= SR-I & ENE, 4= P-I & ENE) \n\n ";
		cin >> choice5;
		if (choice5==2){
			optim=2;
		}
		else if (choice5==3){
			optim=3;
		}
		else if (choice5==4){
			optim=4;
		}
		else if (choice5==1){
			optim=1;
		}
		else {
			cout << "\n Invalid choice. Try again. \n\n ";
			goto top3;
		}
top4:;
		cout << "\n Do you want to include Joint Limit Avoidance? (1= Yes, 2= No) \n\n ";
		cin >> choice50;
		if (choice50==2){
			JLA=0;
		}
		else if (choice50==1){
			JLA=1;
		}
		else {
			cout << "\n Invalid choice. Try again. \n\n ";
			goto top4;
		}
top5:;
		cout << "\n Do you want to include Joint Limit/Velocity and Obstacle Safety Stop? (1= Yes, 2= No) \n\n ";
		cin >> choice500;
		if (choice500==2){
			JLO=0;
		}
		else if (choice500==1){
			JLO=1;
		}
		else {
			cout << "\n Invalid choice. Try again. \n\n ";
			goto top5;
		}
	}
	else if (choice000==3){
		WCA=3;
top6:;
		cout << "\n Choose whose frame to base the control on: \n   For Ground Frame, press '1', \n   For Wheelchair Frame, press '2'. \n\n ";
		cin >> choice00000;
		if (choice00000==2){
			coord=2;
		}
		else if (choice00000==1){
			coord=1;
		}
		else {
			cout << "\n Invalid choice. Try again. \n\n ";
			goto top6;
		}
top7:;
		cout << "\n Do you want to include Joint Limit/Velocity and Obstacle Safety Stop? (1= Yes, 2= No) \n\n ";
		cin >> choice500;
		if (choice500==2){
			JLO=0;
		}
		else if (choice500==1){
			JLO=1;
		}
		else {
			cout << "\n Invalid choice. Try again. \n\n ";
			goto top7;
		}
		
		cart=2;
		optim=0;
		JLA=0;
	}
	else if (choice000==1) {
		WCA=1;
top8:;
		cout << "\n Choose whose frame to base the control on: \n   For Ground Frame, press '1', \n   For Wheelchair Frame, press '2', \n   For Gripper Frame, press '3'. \n\n ";
		cin >> choice00000;
		if (choice00000==2){
			coord=2;
		}
		else if (choice00000==3){
			coord=3;
		}
		else if (choice00000==1){
			coord=1;
		}
		else {
			cout << "\n Invalid choice. Try again. \n\n ";
			goto top8;
		}
top9:;
		cout << "\n Choose the cartesian coordinates to be controlled: \n   For Position and Orientation, press '1', \n   For Position only, press '2'. \n\n ";
		cin >> choice0000;
		if (choice0000==2){
			cart=2;
		}
		else if (choice0000==1){
			cart=1;
		}
		else {
			cout << "\n Invalid choice. Try again. \n\n ";
			goto top9;
		}
top10:;
		cout << "\n Please enter the desired optimization method: (1= SR-I & WLN, 2= P-I & WLN, 3= SR-I & ENE, 4= P-I & ENE) \n\n ";
		cin >> choice5;
		if (choice5==2){
			optim=2;
		}
		else if (choice5==3){
			optim=3;
		}
		else if (choice5==4){
			optim=4;
		}
		else if (choice5==1){
			optim=1;
		}
		else {
			cout << "\n Invalid choice. Try again. \n\n ";
			goto top10;
		}
top11:;
		cout << "\n Do you want to include Joint Limit Avoidance? (1= Yes, 2= No) \n\n ";
		cin >> choice50;
		if (choice50==2){
			JLA=0;
		}
		else if (choice50==1){
			JLA=1;
		}
		else {
			cout << "\n Invalid choice. Try again. \n\n ";
			goto top11;
		}
top12:;
		cout << "\n Do you want to include Joint Limit/Velocity and Obstacle Safety Stop? (1= Yes, 2= No) \n\n ";
		cin >> choice500;
		if (choice500==2){
			JLO=0;
		}
		else if (choice500==1){
			JLO=1;
		}
		else {
			cout << "\n Invalid choice. Try again. \n\n ";
			goto top12;
		}
	}
	else {
		cout << "\n Invalid choice. Try again. \n\n ";
		goto topchoice;
	}

topchoice2:;
	cout << "\n Choose the control mode: \n For position control, press '1', \n For velocity control, press '2', \n For SpaceBall control, press '3', \n For Psychology Mask control, press '4', \n For Touch Screen control, press '5'. \n\n ";
	cin >> choice0;
	if (choice0==1){
		cont=1;
		printf ("\n Please enter the transformation matrix of the desired position and orientation from the control-based frame \n (e.g. [0 0 1 1455;-1 0 0 369;0 -1 0 999; 0 0 0 1])\n\n");
		cin >> Td;
		cout << "\n Please enter the desired linear velocity of the gripper in mm/s (e.g. 50) \n\n ";
		cin >> v;
top13:;
		cout << "\n Chose the Trajectory generation function: \n Press '1' for a Polynomial function with Blending, or \n press '2' for a Polynomial function without Blending, or \n press '3' for a Linear function. \n\n ";
		cin >> choice00;
		if (choice00==2) {
			trajf=2;
		}
		else if (choice00==3){
			trajf=3;
		}
		else if (choice00==1){
			trajf=1;
		}
		else {
		cout << "\n Invalid choice. Try again. \n\n ";
		goto top13;
		}
	}
	else if (choice0==2){
		cont=2;
		cout << "\n Please enter the desired simulation time in seconds (e.g. 2) \n\n ";
		cin >> ts;
		if (cart==2){
			cout << "\n Please enter the desired 3x1 cartesian velocity vector of the gripper (in mm/s) (e.g. [70;70;-70]) \n\n ";
			cin >> Vd;
		}
		else {
			cout << "\n Please enter the desired 6x1 cartesian velocity vector of the gripper (in mm/s, radians/s) (e.g. [70;70;-70;0.001;0.001;0.001]) \n\n ";
			Vd.SetSize(6,1);
			cin >> Vd;
		}
	}
	else if (choice0==3){
		cont=3;
		//Space Ball will be used for control.
		cout << "\n Please enter the desired linear velocity of the gripper in mm/s (e.g. 50) \n\n ";
		cin >> v;
	}
	else if (choice0==4){
		cont=4;
		//BCI 2000 Psychology Mask will be used for control.
		cout << "\n Please enter the desired linear velocity of the gripper in mm/s (e.g. 50) \n\n ";
		cin >> v;
		cout << "\n Please enter the desired port number (e.g. 19711) \n\n ";
		cin >> port1;
	}		
	else if (choice0==5){
		cont=5;
		//Touch Screen will be used for control.
		cout << "\n Please enter the desired linear velocity of the gripper in mm/s (e.g. 50) \n\n ";
		cin >> v; 
	}
	else {
		cout << "\n Invalid choice. Try again. \n\n ";
		goto topchoice2;
	}

topchoice3:;
	cout << "\n Choose animation type or no animation: \n For Virtual Reality Animation, press '1', \n For Matlab Graphics Animation, press '2', \n For BOTH Animations, press '3', \n For NO Animation, press '4'. \n\n ";
	cin >> choice1;
	if (choice1==2) {
		vr=0; ml=1;
	}
	else if (choice1==3){
		vr=1; ml=1;
	}
	else if (choice1==4){
		vr=0; ml=0;
	}
	else if (choice1==1){
		vr=1; ml=0;
	}
	else {
		cout << "\n Invalid choice. Try again. \n\n ";
		goto topchoice3;
	}

topchoice4:;
	cout << "\n Would you like to run the actual WMRA? \n For yes, press '1', \n For no, press '2'.\n\n";
	cin >> choice10;
	if (choice10==1) {
		arm=1;
	}
	else if (choice10==2){
		arm=0;
	}
	else {
		cout << "\n Invalid choice. Try again. \n\n ";
		goto topchoice4;
	}

topchoice5:;
	cout <<"\n Press '1' if you want to start at the 'ready' position, \n or press '2' if you want to enter the initial joint angles. \n\n ";
	cin >> choice2;
	if (choice2==2) {
		cout <<"\n Please enter the arms initial angles vector in radians (e.g. [PI/2;PI/2;0;PI/2;PI/2;PI/2;0])\n\n ";
		cin >> qi;
		cout <<"\n  Please enter the initial x,y position and z orientation of the WMRA base from the ground base in millimeters and radians (e.g. [200;500;0.3])\n\n ";
		cin >> WCi;
		ini = 0;
	}
	else if (choice2==1){
		float qi2 [7]= {90, 90, 0, 90, 90, 90, 0};
		for ( j = 0 ; j < 7 ; j++ ) {
			qi(j,0)=qi2[j]*d2r;
		}
		WCi.Null(3,1);
		if (vr==1 && ml==1 && arm==1) {
top14:;	
			cout <<"\n Press '1' if you want to include 'park' to 'ready' motion, \n or press '2' if not.\n\n ";
			cin >> choice3;
			if (choice3==2){
				ini = 0;
			}
			else if (choice3==1){
				ini = 1;
			}
			else {
				cout << "\n Invalid choice. Try again. \n\n ";
				goto top14;
			}
		}
	}
	else {
		cout << "\n Invalid choice. Try again. \n\n ";
		goto topchoice5;
	}

topchoice6:;
	cout <<"\n Press '1' if you do NOT want to plot the simulation results, \n or press '2' if do.\n\n ";
	cin >> choice4;
	if (choice4==2) {
		plt=2;
	}
	else if (choice4==1) {
		plt=1;
	}
	else {
		cout << "\n Invalid choice. Try again. \n\n ";
		goto topchoice6;
	}
	
	
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
	cout<<"Tiwc\n"<<Tiwc<<"\n\n";
	cout<<"Ti\n"<<Ti<<"\n\n";
	cout<<"Tia\n"<<Tia<<"\n\n";
	cout<<"Tiwco\n"<<Tiwco<<"\n\n";
	cout<<"T01\n"<<T01<<"\n\n";
	cout<<"T12\n"<<T12<<"\n\n";
	cout<<"T23\n"<<T23<<"\n\n";
	cout<<"T34\n"<<T34<<"\n\n";
	cout<<"T45\n"<<T45<<"\n\n";
	cout<<"T56\n"<<T56<<"\n\n";
	cout<<"T67\n"<<T67<<"\n\n";

	float D, dt, total_time, n, dg;
	Matrix dx(1,1);
	float ***Tt;
	if (cont==1){
		//Calculating the linear distance from the initial position to the desired position and the linear velocity:
		if (coord==2){
			D=sqrt(pow(Td(0,3)-Tia(0,3),2) + pow(Td(1,3)-Tia(1,3),2) + pow(Td(2,3)-Tia(2,3),2));
			cout<<"D\n"<<D<<"\n\n";
		}
		else if (coord==3){
			D=sqrt(pow(Td(0,3),2) + pow(Td(1,3),2) + pow(Td(2,3),2));
			cout<<"D\n"<<D<<"\n\n";
		}
		else {
			D=sqrt(pow(Td(0,3)-Ti(0,3),2) + pow(Td(1,3)-Ti(1,3),2) + pow(Td(2,3)-Ti(2,3),2));
			cout<<"D\n"<<D<<"\n\n";
		}
		//Calculating the number of iteration and the time increment (delta t) if the linear step increment of the tip is 1 mm:
		dt=0.05;    // Time increment in seconds.
		total_time=D/v;     // Total time of animation.
		n=Math::Round(total_time/dt); // Number of iterations rounded up.
		dt=total_time/n;    // Adjusted time increment in seconds.
		cout<<"Total time\n"<<total_time<<"\n\n";
		cout<<"n\n"<<n<<"\n\n";
		cout<<"dt\n"<<dt<<"\n\n";
		n=2;
		
		// Calculating the Trajectory of the end effector, and once the trajectory is calculated, we should redefine "Td" based on the ground frame:
		if (coord==2){
			Tt=WMRA_traj(trajf,Tia,Td,n+2);
			Td=Tiwc*Td;
			cout<<"Tt\n\n";
		}
		else if (coord==3){
			Unit.Unit(4);
			Tt=WMRA_traj(trajf,Unit,Td,n+2);
			Td=Ti*Td;
			cout<<"Tt\n\n";
		}
		else {
			Tt=WMRA_traj(trajf,Ti,Td,n+2);
			cout<<"Tt\n\n";
		}
	}
	else if (cont==2){
		// Calculating the number of iterations and the time increment (delta t) if the linear step increment of the gripper is 1 mm:
		dt=0.05;    // Time increment in seconds.
		total_time=ts;     // Total time of animation.
		n=Math::Round(total_time/dt); // Number of iterations rounded up.
		dt=total_time/n;    // Adjusted time increment in seconds.
		dx=Vd;
		dx*=(dt);
		Td=Ti;
		cout<<"Total time\n"<<total_time<<"\n\n";
		cout<<"n\n"<<n<<"\n\n";
		cout<<"dt\n"<<dt<<"\n\n";
		cout<<"dx\n"<<dx<<"\n\n";
		cout<<"Td\n"<<Td<<"\n\n";
		n=2;
	}
	else if (cont==3){
		//WMRA_exit(); // This is to stop the simulation in SpaceBall control when the user presses the exit key.
		//Matrix spdata(6,1);
		//spdata(0,0)=spdata1(3)/20;
		//spdata(1,0)=-spdata1(1)/40;
		//spdata(2,0)=spdata1(2)/30;
		//spdata(3,0)=spdata1(6)/1500;
		//spdata(4,0)=-spdata1(4)/900;
		//spdata(5,0)=spdata1(5)/1300;
		//dt=0.05;
		//dx=spdata;
		//dx*=(v*dt);
		//dg=spdata1(7);
		//Td=Ti;
		//n=1;
	}
	else if (cont==4){
		//WMRA_exit(); // This is to stop the simulation in Psychology Mask control when the user presses the exit key.
		//dt=0.05;
		//dx=WMRA_psy(port1);
		//dx*=(v*dt);
		//dg=dx(7);
		//dx.SetSize(6,1);
		//Td=Ti;
		//n=1;
	}
	else {
		//WMRA_screen('0');   // This is to start the screen controls. Argument: '0'=BACK button disabled, '1'=BACK button enabled.
		//dt=0.05;
		//dx=VAR_DX(1:6);
		//dx*=(v*dt);
		//dg=VAR_DX(7);
		//Td=Ti;
		//n=1;
	}
	
	// Initializing the joint angles, the Transformation Matrix, and time:
	Matrix qo(9,1), To(4,4), Towc(4,4), Toa(4,4), Jo(1,1);
	float tt, ddt;
	int i;
	dq.Null(9,1);
	cout<<"dq"<<dq<<"\n\n";
	dg=0;
	cout<<"qi"<<qi<<"\n\n";
	cout<<"qiwc"<<qiwc<<"\n\n";
	for (j=0; j<7; j++){
		qo(j,0)=qi(j,0);
	}
	for (j=7; j<9; j++){
		qo(j,0)=qiwc(j-7,0);
	}
	cout<<"qo\n"<<qo<<"\n\n";
	To=Ti;
	cout<<"To\n"<<To<<"\n\n";
	Toa=Tia;
	cout<<"Toa\n"<<Toa<<"\n\n";
	Towc=Tiwc;
	cout<<"Towc\n"<<Towc<<"\n\n";
	tt=0;
	i=0;
	dHo.Null(7,1);
	
	// Initializing the WMRA:
	if (ini==0){   // When no "park" to "ready" motion required.
		// Initializing Virtual Reality Animation:
		if (vr==1){
			//WMRA_VR_Animation(1, Towc, qo);
		}
		// Initializing Robot Animation in Matlab Graphics:
		if (ml==1){
			//WMRA_ML_Animation(1, To, Td, Towc, T01, T12, T23, T34, T45, T56, T67);
		}
		// Initializing the Physical Arm:
		if (arm==1){
			Matrix qotemp(1,1);
			qotemp.Null(10,1);
			for (j=0; j<9; j++){
				qotemp(j,0)=qo(j,0);
			}
			WMRA_ARM_Motion(1, 2, qotemp, 0);
			ddt=0;
		}
		else if (ini==1 && (vr==1 || ml==1 || arm==1)){	 // When "park" to "ready" motion is required.
			Matrix qotemp(2,1);
			qotemp(0,0)=qo(7,0);
			qotemp(1,0)=qo(8,0);
			//WMRA_park2ready(1, vr, ml, arm, Towc, qotemp);
			if (arm==1){
				ddt=0;
			}
		}
	}

		// Re-Drawing the Animation:
		Matrix Joa(6,7), Jowc(1,1), Jowctemp(1,1), Joatemp(1,1);
		float detJoa, detJo, phi;
		if (vr==1 || ml==1){
			//drawnow;
		}
		// Starting a timer:
		clock_t tic, toc;
		tic=clock();
		cout<<"tic"<<tic<<"\n\n";
		cout<<"i"<<i<<"\n\n";

		float ***Ttnew;
		Ttnew = new float**[n];
		for (j = 0; j < n; ++j) {
			Ttnew[j] = new float*[4];
			for (k = 0; k < 4; ++k){
				Tt[j][k] = new float[4];
			}
		}

		// Starting the Iteration Loop:
		while (i<n){
			// Calculating the 6X7 Jacobian of the arm in frame 0:
			WMRA_J07(T01, T12, T23, T34, T45, T56, T67, Joa, detJoa);
			// Calculating the 6X2 Jacobian based on the WMRA's base in the ground frame:
			phi=atan2(Towc(1,0),Towc(0,0));
			Jowc=WMRA_Jga(1, phi, Toa(0,3), Toa(1,3));
			// Changing the Jacobian reference frame based on the choice of which coordinates frame are referenced in the Cartesian control:
			// coord=1 for Ground Coordinates Control.
			// coord=2 for Wheelchair Coordinates Control.
			// coord=3 for Gripper Coordinates Control.
			if (coord==2){
				Joa=Joa;
				Jowctemp.Null(6,6);
				for (j=0; j<3; j++){
					for (k=0; k<3; k++){
						Jowctemp(j,k)=Towc(k,j);
					}
				}
				for (j=3; j<6; j++){
					for (k=3; k<6; k++){
						Jowctemp(j,k)=Towc(k-3,j-3);
					}
				}
				Jowctemp=Jowctemp*Jowc;
				Jowc=Jowctemp;
			}
			else if (coord==3){
				Joatemp.Null(6,6);
				for (j=0; j<3; j++){
					for (k=0; k<3; k++){
						Joatemp(j,k)=Toa(k,j);
					}
				}
				for (j=3; j<6; j++){
					for (k=3; k<6; k++){
						Joatemp(j,k)=Toa(k-3,j-3);
					}
				}
				Joatemp=Joatemp*Joa;
				Joa=Joatemp;
				Jowctemp.Null(6,6);
				for (j=0; j<3; j++){
					for (k=0; k<3; k++){
						Jowctemp(j,k)=To(k,j);
					}
				}
				for (j=3; j<6; j++){
					for (k=3; k<6; k++){
						Jowctemp(j,k)=To(k-3,j-3);
					}
				}
				Jowctemp=Jowctemp*Jowc;
				Jowc=Jowctemp;
			}
			else if (coord==1){
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
				Jowc=Jowc;
			}

			//Calculating the 3X9 or 6X9 augmented Jacobian of the WMRA system based on the ground frame:
			if (cart==2){
				Joa.SetSize(3,7);
				Joatemp=~Joa;
				Joatemp=Joa*Joatemp;
				detJoa=sqrt(Joatemp.Det());
				Jowc.SetSize(3,2);
				Jo.SetSize(3,9);
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
			}
			else {
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
			}
			
			// Finding the Cartesian errors of the end effector:
			Matrix invTowc(1,1), invTo(1,1), Towctemp(3,3), Towctemp2(3,1), Tttemp(4,4), Ttnew1(4,4);
			if (cont==1){
				// Calculating the Position and Orientation errors of the end effector, and the rates of motion of the end effector:            
				if (coord==2){
					invTowc.Unit(4);
					for (j=0; j<3; j++){
						for (k=0; k<3; k++){
							invTowc(j,k)=Towc(k,j);
							Towctemp=(j,k)=Towc(k,j);
							Towctemp2(j,0)=Towc(j,3);
						}
					}
					Towctemp*=(-1);
					Towctemp=Towctemp*Towctemp2;
					for (j=0; j<3; j++){
						invTowc(j,3)=Towctemp(j,0);
					}	
					for (j=0; j<4; j++){
						for (k=0; k<4; k++){
							Tttemp(j,k)=Tt[i][j][k];							
						}
					}
					Tttemp=invTowc*Tiwc*Tttemp;
					Ttnew1=Tttemp;
					for (j=0; j<4; j++){
						for (k=0; k<4; k++){
							Ttnew[i][j][k]=Tttemp(j,k);	
						}
					}
					dx=WMRA_delta(Toa, Ttnew1);
				}
				else if (coord==3){
					invTo.Unit(4);
					for (j=0; j<3; j++){
						for (k=0; k<3; k++){
							invTo(j,k)=To(k,j);
							Towctemp=(j,k)=-To(k,j);
							Towctemp2(j,0)=To(j,3);
						}
					}
					Towctemp=Towctemp*Towctemp2;
					for (j=0; j<3; j++){
						invTo(j,3)=Towctemp(j,0);
					}	
					for (j=0; j<4; j++){
						for (k=0; k<4; k++){
							Tttemp(j,k)=Tt[i][j][k];							
						}
					}
					Tttemp=invTo*Ti*Tttemp;
					Ttnew1=Tttemp;
					for (j=0; j<4; j++){
						for (k=0; k<4; k++){
							Ttnew[i][j][k]=Tttemp(j,k);	
						}
					}
					Unit.Unit(4);
					dx=WMRA_delta(Unit, Ttnew1);
				}
				else {
					for (j=0; j<4; j++){
						for (k=0; k<4; k++){
							Tttemp(j,k)=Tt[i][j][k];	
						}
					}	
					dx=WMRA_delta(To, Tttemp);
				}
				cout<<"\ndx:\n\n"<<dx;
			}
			
			else if (cont==2) {
			}
			else if (cont==3) {
				//Matrix spdata(6,1)
				//spdata(0,0)=spdata1(3)/20;
				//spdata(1,0)=-spdata1(1)/40;
				//spdata(2,0)=spdata1(2)/30;
				//spdata(3,0)=spdata1(6)/1500;
				//spdata(4,0)=-spdata1(4)/900;
				//spdata(5,0)=spdata1(5)/1300;
				//dt=0.05;
				//dx=spdata;
				//dx*=(v*dt);
				//dg=spdata1(7);
			}
			else if (cont==4) {
				//dx=WMRA_psy(port1);
				//dx*=(v*dt);
				//dg=dx(6,0);
				//dx.SetSize(6,1);
			}
			else {
				//dx=WMRA_VAR_DX(1:6);
				//dx*=(v*dt);
				//dg=VAR_DX(6,0);
			}
			
			// Changing the order of Cartesian motion in the case when gripper reference frame is selected for control with the screen or psy or SpaceBall interfaces: 
			if (coord==3 && cont>=3){
				dx(0,0)=-dx(1,0);
				dx(1,0)=-dx(2,0);
				dx(2,0)=dx(0,0);
				dx(3,0)=-dx(4,0);
				dx(4,0)=-dx(5,0);
				dx(5,0)=dx(3,0);
			}
			if (cart==2) {
				dx.SetSize(3,1);
			}

			// Calculating the resolved rate with optimization:
			// Index input values for "optim": 1= SR-I & WLN, 2= P-I & WLN, 3= SR-I & ENE, 4= P-I & ENE:
			if (WCA==2) {
				dq.SetSize(7,1);
				dq=WMRA_Opt(optim, JLA, JLO, Joa, detJoa, dq, dx, dt, qo);
				dq.SetSize(9,1);
			}
			else if (WCA==3) {
				Matrix dqtemp(2,1);
				dqtemp(0,0)=dq(7,0);
				dqtemp(1,0)=dq(8,0);
				dx.SetSize(2,1);
				dq=WMRA_Opt(optim, JLA, JLO, Jowc, 1, dqtemp, dx, dt, 1);
				dqtemp.Null(9,1);
				dqtemp(7,0)=dq(7,0);	
				dqtemp(8,0)=dq(8,0);	
				dq=dqtemp;
			}
			else {
				cout<<"\noptim "<<optim;
				cout<<"\nJLA"<<JLA;
				cout<<"\nJLO "<<JLO;
				cout<<"\nJo "<<Jo;
				cout<<"\ndetJo "<<detJo;
				cout<<"\ndq "<<dq;
				cout<<"\ndx"<<dx;
				cout<<"\ndt"<<dt;
				cout<<"\nqo "<<qo;
				dq=WMRA_Opt(optim, JLA, JLO, Jo, detJo, dq, dx, dt, qo);
				cout<<"\ndq is\n\n"<<dq;
			}
			
			Matrix qn(1,1), Tn(4,4), Tna(4,4), Tnwc(4,4);
			// Calculating the new Joint Angles:
			qn=qo+dq;
			// Calculating the new Transformation Matrices:
			Matrix dqtemp(2,1);
			dqtemp(0,0)=dq(7,0);
			dqtemp(1,0)=dq(8,0);
			WMRA_Tall(2, qn, dqtemp, Towc, Tn, Tna, Tnwc, T01, T12, T23, T34, T45, T56, T67);

			// A safety condition function to stop the joints that may cause colision of the arm with itself, the wheelchair, or the human user:
			if (JLO==1 && WCA!=3){
				Matrix dqtemp(2,1);
				dqtemp(0,0)=dq(7,0);
				dqtemp(1,0)=dq(8,0);
				dq.SetSize(7,1);
				dq=WMRA_collide(dq, T01, T12, T23, T34, T45, T56, T67);
				// Re-calculating the new Joint Angles:
				dq.SetSize(9,1);
				qn=qo+dq;
				// Re-calculating the new Transformation Matrices:				
				WMRA_Tall(2, qn, dqtemp, Towc, Tn, Tna, Tnwc, T01, T12, T23, T34, T45, T56, T67);
			}
			
			// Saving the plot data in case plots are required:
			if (plt==2){
				//WMRA_Plots(1, L, r2d, dt, i, tt, qn, dq, Tn, Tnwc, detJoa, detJo);
			}
			
			// Updating Physical Arm:
			if (arm==1){
				ddt=ddt+dt;
				if (ddt>=0.5 || i>=(n+1)){
					Matrix dqtemp(10,1);
					for (j=0; j<9; j++){
						dqtemp(j,0)=qn(j,0);	
					}
					dqtemp(9,0)=dg;
					WMRA_ARM_Motion(2, 1, dqtemp, ddt);
					ddt=0;
				}
			}
			
			// Updating Virtual Reality Animation:
			if (vr==1){
				//WMRA_VR_Animation(2, Tnwc, qn);
			}
			// Updating Matlab Graphics Animation:
			if (ml==1){
				//WMRA_ML_Animation(2, Ti, Td, Tnwc, T01, T12, T23, T34, T45, T56, T67);
			}
			
			// Re-Drawing the Animation:
			if (vr==1 || ml==1){
				//drawnow;
			}
			
			// Updating the old values with the new values for the next iteration:
			qo=qn;
			To=Tn;
			Toa=Tna;
			Towc=Tnwc;
			tt=tt+dt;
			i=i+1;
						
			// Stopping the simulation when the exit button is pressed:
			if (cont==3 || cont==4 || cont==5){
				//if (VAR_SCREENOPN == 1){
					//	n=n+1;
				//}
				//else {
					//break
				//}
			}

			// Delay to comply with the required speed:
			if (tic < tt){
			//	Sleep(tt-tic); 
			}
		}
		
		cout<<"dq"<<dq<<"\n\n";
		cout<<"qo"<<qo<<"\n\n"; 
		cout<<"To"<<To<<"\n\n";
		cout<<"Toa"<<Toa<<"\n\n";
		cout<<"Towc"<<Towc<<"\n\n";
		cout<<"tt"<<tt<<"\n\n";
		cout<<"i"<<i<<"\n\n";

		
		// Reading the elapsed time and printing it with the simulation time:
		if (cont==1 || cont==2){
			printf("\nSimula. time is %6.6f seconds.\n", total_time);
		}
		toc=clock()-tic / ((double)CLOCKS_PER_SEC);
		printf("\nElapsed time is %6.6f seconds.\n", toc);
						
		// Plotting:
		if (plt==2){
			//WMRA_Plots(2, L, r2d, dt, i, tt, qn, dq, Tn, Tnwc, detJoa, detJo);
		}
		
		if (vr==1 || ml==1 || arm==1){
			// Going back to the ready position:
			cout << "\n Do you want to go back to the 'ready' position? \n Press '1' for Yes, or press '2' for No. \n\n ";
			cin >> choice6;
			if (choice6==1){
				//WMRA_any2ready(2, vr, ml, arm, Tnwc, qn);
				// Going back to the parking position:
				cout << "\n Do you want to go back to the 'parking' position? \n Press '1' for Yes, or press '2' for No. \n\n ";
					if (choice7==1){
						//WMRA_ready2park(2, vr, ml, arm, Tnwc, qn(8:9));
					}
			}
			
			// Closing the Arm library and Matlab Graphics Animation and Virtual Reality Animation and Plots windows:
			cout << "\n Do you want to close all simulation windows and arm controls? \n Press '1' for Yes, or press '2' for No. \n\n ";
			cin >> choice8;
			if (choice8==1){
				Matrix temp(1,1);
				temp.Null(1,1);
				if (arm==1){
					WMRA_ARM_Motion(3, 0, temp, 0);
				}
				if (vr==1){
					//WMRA_VR_Animation(3, 0, 0);
				}
				if (ml==1){
					//WMRA_ML_Animation(3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
				}
				if (plt==2){
					//close (figure(2),figure(3),figure(4),figure(5),figure(6),figure(7),figure(8),figure(9),figure(10));
				}
			}
		}
  		return 0;
}
