/* This "new USF WMRA" script SIMULATES the Joint control of the WMRA system with ANIMATION and plots for 9 DOF. All angles are in Radians.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2007 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Defining used parameters:*/

#ifndef _JOINTCONT_H // if not defined
#define _JOINTCONT_H // define MyHeader

#include "matrix.h"  
#include "any2ready.h" 
#include "park2ready.h" 
#include "ready2park.h"
#include "ArmMotion.h" 
#include "DH.h" 
#include "WCD.h" 
#include <time.h>
using namespace std;
using namespace math;

#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif

#define PI 3.14159265
#define d2r PI/180 //Conversions from Degrees to Radians.
#define r2d 180/PI //Conversions from Radians to Degrees.

Matrix L(1,1);
// Reading the Wheelchair's constant dimentions, all dimentions are converted in millimeters:
L=WMRA_WCD();
	
	// User input prompts:
	int choice1, choice2, choice3, choice4;
	int vr, ml, arm, ini, plt;
	int j, k;
	Matrix qi(7,1), WCi(3,1);

topchoice:;			
	cout << "\n Choose animation type or no animation: \n   For Virtual Reality Animation, press '1',\n For Matlab Graphics Animation, press '2',\n For BOTH Animations, press '3',\n For NO Animation, press '4'. \n\n ";
	cin >> choice1;
	if (choice1==2){
		vr = 0; ml = 1;
	}
	else if (choice1==3){
		vr = 1; ml = 1;
	}
	else if (choice1==4){
		vr = 0; ml = 0;
	else if (choice1=1){
		vr = 1; ml = 0;
	}
	else {
		cout << "\n Invalid choice. Try again. \n\n ";
		goto topchoice;
	}

top1:;
	cout << "\n Would you like to run the actual arm? \n For no, press '1',\n For yes, press '2'. \n\n ";
	cin >> choice2;
	if (choice1==2){
		arm = 1;
	}
	else if (choice1==1){
		arm = 0;
	}
	else {
		cout << "\n Invalid choice. Try again. \n\n ";
		goto top1;
	}

top2:;
	cout << "\n Press '1' if you want to start at the 'ready' position, \n or press '2' if you want to enter the initial joint angles. \n\n ";
	cin >> choice3;
	if (choice3==2){
		cout <<"\n Please enter the arms initial angles vector in radians (e.g. [PI/2;PI/2;0;PI/2;PI/2;PI/2;0])\n\n ";
		cin >> qi;
		cout <<"\n  Please enter the initial x,y position and z orientation of the WMRA base in millimeters and radians (e.g. [200;500;0.3])\n\n ";
		cin >> WCi;
		ini = 0;
	}
	else if (choice3==1){
		float qi2 [7]= {90, 90, 0, 90, 90, 90, 0};
		for ( j = 0 ; j < 7 ; j++ ) {
			qi(j,0)=qi2[j]*d2r;
		}
		WCi.Null(3,1);
		if (vr==1 && ml==1 && arm==1) {
top3:;	
			cout <<"\n Press '1' if you want to include 'park' to 'ready' motion, \n or press '2' if not.\n\n ";
			cin >> choice4;
			if (choice4==2){
				ini = 0;
			}
			else if (choice4==1){
				ini = 1;
			}
			else {
				cout << "\n Invalid choice. Try again. \n\n ";
				goto top3;
			}
		}
	}
	else {
		cout << "\n Invalid choice. Try again. \n\n ";
		goto top2;
	}

// Calculating the Transformation Matrix of the initial position of the WMRA's base:
	Matrix Tiwc(4,4), qiwc(2,1);
	Tiwc = WMRA_p2T(WCi(0,0),WCi(1,0),WCi(2,0));
	//cout<<"Tiwc\n"<<Tiwc<<"\n\n";
	
	//Calculating the initial Wheelchair Variables:
	qiwc(0,0)=sqrt(pow(WCi(0,0),2)+pow(WCi(1,0),2));
	qiwc(1,0)=WCi(2,0);
	//cout<<"qiwc\n"<<qiwc<<"\n\n";

	qi.SetSize(9,1);
	qi(7,0)=qiwc(0,0);
	qi(8,0)=qiwc(1,0);

	cout <<"\n Please enter the desired angles and distance vector in radians and mm (e.g. [PI/3;-PI/3;PI/3;-PI/3;PI/3;-PI/3;PI/3;500;PI/3])\n\n ";
	Matrix qd(9,1);
	cin >> qd;
	cout <<"\n Please enter the desired execution  time in seconds (e.g. 2) \n\n ";
	int ts;
	cin >> ts;

	//Calculating the initial transformation matrices:
	Matrix dq(1,1), Ti(4,4), Tia(4,4), Tiwco(4,4), T01(4,4), T12(4,4), T23(4,4), T34(4,4), T45(4,4), T56(4,4), T67(4,4), Unit(1,1), qtemp(7,1), Td(4,4), Tda(4,4), Tdwco(4,4), T01d(4,4), T12d(4,4), T23d(4,4), T34d(4,4), T45d(4,4), T56d(4,4), T67d(4,4), Unit(1,1);
	for ( j = 0 ; j < 7 ; j++ ) {
		qtemp(j,0)=qi(j,0);
	}
	Unit(0,0)=qi(7,0);
	Unit(1,0)=qi(8,0);
	WMRA_Tall(1, qtemp, Unit, Tiwc, Ti, Tia, Tiwco, T01, T12, T23, T34, T45, T56, T67);
	
	for ( j = 0 ; j < 7 ; j++ ) {
		qtemp(j,0)=qd(j,0);
	}
	Unit(0,0)=qd(7,0);
	Unit(1,0)=qd(8,0);
	WMRA_Tall(2, qtemp, Unit, Tiwc, Td, Tda, Tdwco, T01d, T12d, T23d, T34d, T45d, T56d, T67d);
	

	FILE * fid;
	FILE * fid2;
	fid = fopen("JointConti.txt","w");
	fid = fopen("JointContd.txt","w");
	fprintf(fid," Tiwc is: \n\n");
	fprintf(fid2," Tiwc is: \n\n");
	for (j=0;j<Tiwc.RowNo();j++){
		for (k=0;k<Tiwc.ColNo();k++){
			fprintf(fid," %4.2f ",  Tiwc(j,k));
			fprintf(fid2," %4.2f ",  Tiwc(j,k));
		}
		fprintf(fid," \n");
		fprintf(fid2," \n");
	}
	Tiwc=Tiwco;
	Tdwc=Tdwco;
	fprintf(fid," \n\n\n");
	fprintf(fid2," \n\n\n");
	
	fprintf(fid," Ti is: \n\n");
	fprintf(fid2," Td is: \n\n");
	for (j=0;j<Ti.RowNo();j++){
		for (k=0;k<Ti.ColNo();k++){
			fprintf(fid," %4.2f ",  Ti(j,k));
			fprintf(fid2," %4.2f ",  Td(j,k));
		}
		fprintf(fid," \n");
		fprintf(fid2," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid2," \n\n\n");

	fprintf(fid," Tia is: \n\n");
	fprintf(fid2," Tda is: \n\n");
	for (j=0;j<Tia.RowNo();j++){
		for (k=0;k<Tia.ColNo();k++){
			fprintf(fid," %4.2f ",  Tia(j,k));
			fprintf(fid2," %4.2f ",  Tda(j,k));
		}
		fprintf(fid," \n");
		fprintf(fid2," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid2," \n\n\n");

	fprintf(fid," Tiwc is: \n\n");
	fprintf(fid2," Tdwc is: \n\n");
	for (j=0;j<Tiwc.RowNo();j++){
		for (k=0;k<Tiwc.ColNo();k++){
			fprintf(fid," %4.2f ",  Tiwc(j,k));
			fprintf(fid2," %4.2f ",  Tdwc(j,k));
		}
		fprintf(fid," \n");
		fprintf(fid2," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid2," \n\n\n");

	fprintf(fid," T01 is: \n\n");
	fprintf(fid2," T01d is: \n\n");
	for (j=0;j<T01.RowNo();j++){
		for (k=0;k<T01.ColNo();k++){
			fprintf(fid," %4.2f ",  T01(j,k));
			fprintf(fid2," %4.2f ",  T01d(j,k));
		}
		fprintf(fid," \n");
		fprintf(fid2," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid2," \n\n\n");

	fprintf(fid," T12 is: \n\n");
	fprintf(fid2," T12d is: \n\n");
	for (j=0;j<T12.RowNo();j++){
		for (k=0;k<T12.ColNo();k++){
			fprintf(fid," %4.2f ",  T12(j,k));
			fprintf(fid2," %4.2f ",  T12d(j,k));
		}
		fprintf(fid," \n");
		fprintf(fid2," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid2," \n\n\n");

	fprintf(fid," T23 is: \n\n");
	fprintf(fid2," T23d is: \n\n");
	for (j=0;j<T23.RowNo();j++){
		for (k=0;k<T23.ColNo();k++){
			fprintf(fid," %4.2f ",  T23(j,k));
			fprintf(fid2," %4.2f ",  T23d(j,k));
		}
		fprintf(fid," \n");
		fprintf(fid2," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid2," \n\n\n");

	fprintf(fid," T34 is: \n\n");
	fprintf(fid2," T34d is: \n\n");
	for (j=0;j<T34.RowNo();j++){
		for (k=0;k<T34.ColNo();k++){
			fprintf(fid," %4.2f ",  T34(j,k));
			fprintf(fid2," %4.2f ",  T34d(j,k));
		}
		fprintf(fid," \n");
		fprintf(fid2," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid2," \n\n\n");

	fprintf(fid," T45 is: \n\n");
	fprintf(fid2," T45d is: \n\n");
	for (j=0;j<T45.RowNo();j++){
		for (k=0;k<T45.ColNo();k++){
			fprintf(fid," %4.2f ",  T45(j,k));
			fprintf(fid2," %4.2f ",  T45d(j,k));
		}
		fprintf(fid," \n");
		fprintf(fid2," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid2," \n\n\n");

	fprintf(fid," T56 is: \n\n");
	fprintf(fid2," T56d is: \n\n");
	for (j=0;j<T56.RowNo();j++){
		for (k=0;k<T56.ColNo();k++){
			fprintf(fid," %4.2f ",  T56(j,k));
			fprintf(fid2," %4.2f ",  T56d(j,k));
		}
		fprintf(fid," \n");
		fprintf(fid2," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid2," \n\n\n");

	fprintf(fid," T67 is: \n\n");
	fprintf(fid2," T67d is: \n\n");
	for (j=0;j<T67.RowNo();j++){
		for (k=0;k<T67.ColNo();k++){
			fprintf(fid," %4.2f ",  T67(j,k));
			fprintf(fid2," %4.2f ",  T67d(j,k));
		}
		fprintf(fid," \n");
		fprintf(fid2," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid2," \n\n\n");

	float dt, total_time, n;
	
	//Calculating the number of iteration and the time increment (delta t) if the linear step increment of the tip is 1 mm:
	dt=0.05;    // Time increment in seconds.
	total_time=D/v;     // Total time of animation.
	n=Math::Round(total_time/dt); // Number of iterations rounded up.
	dt=total_time/n;    // Adjusted time increment in seconds.
	dq=qd-qi;
	dq/=(n);

	// Initializing the joint angles, the Transformation matrix, and time:
	float tt;
	int i;
	Matrix qo(1,1), To(1,1), Toa(1,1), Towc(1,1);

	qo=qi;
	To=Ti;
	Toa=Tia;
	Towc=Tiwc;
	tt=0;
	i=1;

	Matrix zero(1,1);

	if (ini==0){ // When no "park" to "ready" motion required.
		// Initializing the Physical Arm:
		if (arm==1){
			zero=qo;
			zero.SetSize(10,1);
			WMRA_ARM_Motion(1, 2, zero, 0);
			ddt=0;
		}
		// Initializing Virtual Reality Animation:
		if (vr==1){
			//WMRA_VR_Animation(1, Towc, qo);
		}
		//Initializing Robot Animation in Matlab Graphics:
		if (ml==1){
			// WMRA_ML_Animation(1, To, Td, Towc, T01, T12, T23, T34, T45, T56, T67);
		}
	}
	else if (ini==1 && (vr==1 || ml==1 || arm==1)){  // When "park" to "ready" motion is required.
		zero.Null(2,1);
		zero(0,0)=qo(7,0);
		zero(1,0)=qo(8,0);
		WMRA_park2ready(1, vr, ml, arm, Towc, zero);
		if (arm==1){
			ddt=0;
		}
	}

	// Re-Drawing the Animation:
	if (vr==1 || ml==1){
		//drawnow;
	}
	
	// Starting a timer:
	clock_t startt, endt, endf;
	double timedif; 
	startt=clock()/ CLOCKS_PER_SEC;

	Matrix qn(1,1), Tn(4,4), Tna(4,4), Tnwc(4,4);
	Matrix q1(1,1), T1(4,4), Ta1(4,4), Twc1(4,4);
	Matrix dq1(1,1);
	// Starting the Iteration Loop:
	while (i <= n) {
		
		// Calculating the new Joint Angles:
		qn=qo+dq;
		
		// Calculating the new Transformation Matrices:
		Unit.Null(2,0);
		Unit(0,0)=dq(7,0);
		Unit(1,0)=dq(8,0);
		WMRA_Tall(2, qn, Unit, Towc, Tn, Tna, Tnwc, T01, T12, T23, T34, T45, T56, T67);
		
		// Updating the physical Arm:
		if (arm==1) {
			float ddt2;
			ddt2=0;
			ddt2=ddt+dt;
			if (ddt>=0.5 || i>=(n+1)){
				zero.Null(10,1);
				for ( j = 0 ; j < 9 ; j++ ) {
					zero(j,0)=qn(j,0);
				}
				WMRA_ARM_Motion(2, 1, zero, ddt2);
				ddt=0;
			}
			ddt=0;
			ddt=ddt2;
		}
		
		// Updating Virtual Reality Animation:
		if (vr==1){
			//WMRA_VR_Animation(2, Tnwc, qn);
		}
		
		// Updating Matlab Animation:
		if (ml==1){
			//WMRA_ML_Animation(2, Ti, Td, Tnwc, T01, T12, T23, T34, T45, T56, T67);
		}

		// Re-Drawing the Animation:
		if (vr==1 || ml==1){
			//drawnow;
		}

		// Updating the old values with the new values for the next iteration:
		
		qo.Null(7,1);
		qo=qn;
		To=Tn;
        Toa=Tna;
        Towc=Tnwc;
		tt=tt+dt;
		i++;
		
		// Delay to comply with the required speed:
		endt=0;
		endt=clock()/ CLOCKS_PER_SEC;
		timedif=0;
		timedif=endt-startt;
		if (timedif < tt){
			wait((dt-timedif));
		}
	}
// Reading the elapsed time and printing it with the simulation time:
	endf=clock()/ CLOCKS_PER_SEC;
	cout<<"\nElapsed time is "<<difftime(endf,startt)<<"\n\n";
	int choice 6, choice 7, choice 8;
	if (vr==1 || ml==1 || arm==1){
		// Going back to the ready position:
		cout << "\n Do you want to go back to the 'ready' position? \n Press '1' for Yes, or press '2' for No. \n\n ";
		cin >> choice6;
		if (choice6==1){
			WMRA_any2ready(2, vr, ml, arm, Tnwc, qn);
			// Going back to the parking position:
			cout << "\n Do you want to go back to the 'parking' position? \n Press '1' for Yes, or press '2' for No. \n\n ";	
			cin >> choice7;
			if (choice7==1){
				zero.Null(2,1);
				zero(0,0)=qn(7,0);
				zero(1,0)=qn(8,0);
				WMRA_ready2park(2, vr, ml, arm, Tnwc, zero);
			}
		}
		
		// Closing the Arm library and Matlab Graphics Animation and Virtual Reality Animation and Plots windows:
		cout << "\n Do you want to close all simulation windows and arm controls? \n Press '1' for Yes, or press '2' for No. \n\n ";
		cin >> choice8;
		if (choice8==1){
			zero.Null(1,1);
			if (arm==1){
				WMRA_ARM_Motion(3, 0, zero, 0);
			}
			if (vr==1){
				//WMRA_VR_Animation(3, 0, 0);
			}
			if (ml==1){
				//WMRA_ML_Animation(3, 0, 0, 0, 0, 0, 0, 0,0, 0, 0);
			}
			
		}
	}
#endif