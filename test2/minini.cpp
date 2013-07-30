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
	
	int choice000, choice00000, choice0000, choice5, choice50, choice500, choice0, choice00, choice1, choice10, choice2, choice3, choice4, choice6, choice7, choice8;
	Matrix L(1,1);
	// Reading the Wheelchair's constant dimentions, all dimentions are converted in millimeters:
	L=WMRA_WCD();
	
	// User input prompts:
	int WCA, coord, cart, optim, JLA, JLO, cont, trajf, vr, ml, arm, ini, plt;
	int j, k;
	int port1, ts;
	float v;
	int i;
	float tt;

	Matrix Td(4,4), Vd(3,1), qi(7,1), WCi(3,1);

	WCA=1;
	coord=1;
	cart=1;
	optim=1;
	JLA=1;
	JLO=1;
	cont=1;
	int Tdte[4][4]={{0, 0, 1, 1455},{-1, 0, 0, 369},{0, -1, 0, 999},{ 0, 0, 0, 1}};
	for (j=0;j<4;j++){
		for (k=0;k<4;k++){
			Td(j,k)=Tdte[j][k];
		}
	}
	v=50;
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
	//cout<<"Tiwc\n"<<Tiwc<<"\n\n";
	
	//Calculating the initial Wheelchair Variables:
	qiwc(0,0)=sqrt(pow(WCi(0,0),2)+pow(WCi(1,0),2));
	qiwc(1,0)=WCi(2,0);
	//cout<<"qiwc\n"<<qiwc<<"\n\n";

	//Calculating the initial transformation matrices:
	Matrix dq(1,1), Ti(4,4), Tia(4,4), Tiwco(4,4), T01(4,4), T12(4,4), T23(4,4), T34(4,4), T45(4,4), T56(4,4), T67(4,4), Unit(1,1);
	Unit.Null(2,1);
	WMRA_Tall(1, qi, Unit, Tiwc, Ti, Tia, Tiwco, T01, T12, T23, T34, T45, T56, T67);
	Tiwc=Tiwco;
	//cout<<"Ti\n"<<Ti<<"\n\n";
	//cout<<"Tia\n"<<Tia<<"\n\n";
	//cout<<"Tiwc\n"<<Tiwc<<"\n\n";
	//cout<<"T01\n"<<T01<<"\n\n";
	//cout<<"T12\n"<<T12<<"\n\n";
	//cout<<"T23\n"<<T23<<"\n\n";
	//cout<<"T34\n"<<T34<<"\n\n";
	//cout<<"T45\n"<<T45<<"\n\n";
	//cout<<"T56\n"<<T56<<"\n\n";
	//cout<<"T67\n"<<T67<<"\n\n";

	FILE * fid;
	fid = fopen("minini.txt","w");
	fprintf(fid," Tiwc is: \n\n");
	for (j=0;j<Tiwc.RowNo();j++){
		for (k=0;k<Tiwc.ColNo();k++){
			fprintf(fid," %4.2f ",  Tiwc(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," qiwc is: \n\n");
	for (j=0;j<qiwc.RowNo();j++){
		fprintf(fid," %4.2f ",  qiwc(j,0));
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," Ti is: \n\n");
	for (j=0;j<Ti.RowNo();j++){
		for (k=0;k<Ti.ColNo();k++){
			fprintf(fid," %4.2f ",  Ti(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," Tia is: \n\n");
	for (j=0;j<Tia.RowNo();j++){
		for (k=0;k<Tia.ColNo();k++){
			fprintf(fid," %4.2f ",  Tia(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," Tiwc is: \n\n");
	for (j=0;j<Tiwc.RowNo();j++){
		for (k=0;k<Tiwc.ColNo();k++){
			fprintf(fid," %4.2f ",  Tiwc(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," T12 is: \n\n");
	for (j=0;j<T12.RowNo();j++){
		for (k=0;k<T12.ColNo();k++){
			fprintf(fid," %4.2f ",  T12(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," T23 is: \n\n");
	for (j=0;j<T23.RowNo();j++){
		for (k=0;k<T23.ColNo();k++){
			fprintf(fid," %4.2f ",  T23(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," T34 is: \n\n");
	for (j=0;j<T34.RowNo();j++){
		for (k=0;k<T34.ColNo();k++){
			fprintf(fid," %4.2f ",  T34(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," T45 is: \n\n");
	for (j=0;j<T45.RowNo();j++){
		for (k=0;k<T45.ColNo();k++){
			fprintf(fid," %4.2f ",  T45(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," T56 is: \n\n");
	for (j=0;j<T56.RowNo();j++){
		for (k=0;k<T56.ColNo();k++){
			fprintf(fid," %4.2f ",  T56(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," Tiwc is: \n\n");
	for (j=0;j<T67.RowNo();j++){
		for (k=0;k<T67.ColNo();k++){
			fprintf(fid," %4.2f ",  T67(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");


	float D, dt, total_time, dg;
	int n;
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
			//cout<<"D\n"<<D<<"\n\n";
		}
		fprintf(fid," D is: %g \n\n", D);
		fprintf(fid," \n\n\n");
		//Calculating the number of iteration and the time increment (delta t) if the linear step increment of the tip is 1 mm:
		dt=0.05;    // Time increment in seconds.
		total_time=D/v;     // Total time of animation.
		n=Math::Round(total_time/dt); // Number of iterations rounded up.
		dt=total_time/n;    // Adjusted time increment in seconds.
		fprintf(fid," Total time is: %4.2f \n\n", total_time);
		fprintf(fid," n is: %d \n\n", n);
		fprintf(fid," dt is: %g \n\n", dt);
		fprintf(fid," \n\n\n");
		
		
		// Calculating the Trajectory of the end effector, and once the trajectory is calculated, we should redefine "Td" based on the ground frame:
		if (coord==2){
			Tt=WMRA_traj(trajf,Tia,Td,n+1);
			Td=Tiwc*Td;
		}
		else if (coord==3){
			Unit.Unit(4);
			Tt=WMRA_traj(trajf,Unit,Td,n+1);
			Td=Ti*Td;
		}
		else {
			Tt=WMRA_traj(trajf,Ti,Td,n+1);
			FILE * fid2;
			fid2 = fopen("traj.txt","w");
			fprintf(fid2," Trajectory is \n\n");
			for (i=0;i<n+1;i++){
				for (j=0;j<4;j++){
					for (k=0;k<4;k++){
						fprintf(fid2," %4.2f ",  Tt[i][j][k]);
					}
					fprintf(fid2," \n");
				}
				fprintf(fid2," \n\n\n");
			}
			fprintf(fid2," \n\n\n");
			fclose (fid2);
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
	float ddt;
	dq.Null(9,1);
//	cout<<"dq"<<dq<<"\n\n";
	dg=0;
	for (j=0; j<7; j++){
		qo(j,0)=qi(j,0);
	}
	for (j=7; j<9; j++){
		qo(j,0)=qiwc(j-7,0);
	}
//	cout<<"qo\n"<<qo<<"\n\n";
	To=Ti;
//	cout<<"To\n"<<To<<"\n\n";
	Toa=Tia;
//	cout<<"Toa\n"<<Toa<<"\n\n";
	Towc=Tiwc;
//	cout<<"Towc\n"<<Towc<<"\n\n";
	tt=0;
	i=0;
	dHo.Null(7,1);
	
	fprintf(fid," dq is: \n\n");
	for (j=0;j<dq.RowNo();j++){
		fprintf(fid," %4.2f ",  dq(j,0));
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," qo is: \n\n");
	for (j=0;j<qo.RowNo();j++){
		fprintf(fid," %4.2f ",  qo(j,0));
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," To is: \n\n");
	for (j=0;j<To.RowNo();j++){
		for (k=0;k<To.ColNo();k++){
			fprintf(fid," %4.2f ",  To(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," Toa is: \n\n");
	for (j=0;j<Toa.RowNo();j++){
		for (k=0;k<Toa.ColNo();k++){
			fprintf(fid," %4.2f ",  Toa(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");
	fprintf(fid," Towc is: \n\n");
	for (j=0;j<Towc.RowNo();j++){
		for (k=0;k<Towc.ColNo();k++){
			fprintf(fid," %4.2f ",  Towc(j,k));
		}
		fprintf(fid," \n");
	}
	fprintf(fid," \n\n\n");


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
		cout<<"tic is "<<tic<<"\n\n";
		cout<<"i is "<<i<<"\n\n";

		float ***Ttnew;
		Ttnew = new float**[n];
		for (j = 0; j < n; ++j) {
			Ttnew[j] = new float*[4];
			for (k = 0; k < 4; ++k){
				Ttnew[j][k] = new float[4];
			}
		}
		Matrix qn(1,1), Tn(4,4), Tna(4,4), Tnwc(4,4);
		Matrix q1(1,1), T1(4,4), Ta1(4,4), Twc1(4,4);
		Matrix dq1(1,1);

// Starting the Iteration Loop:
		for (i=0;i<n;i++){
			if (i==0){
				q1=qo;
				T1=To;
				Ta1=Toa;
				Twc1=Towc;
			}
			else{
				q1=qn;
				T1=Tn;
				Ta1=Tna;
				Twc1=Tnwc;
				dq=dq1;
			}


			// Calculating the 6X7 Jacobian of the arm in frame 0:
			WMRA_J07(T01, T12, T23, T34, T45, T56, T67, Joa, detJoa);
			fprintf(fid," i is: %d\n\n",i);
			fprintf(fid," Joa is: \n\n");
			for (j=0;j<Joa.RowNo();j++){
				for (k=0;k<Joa.ColNo();k++){
					fprintf(fid," %4.2f ",  Joa(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," detJoa is: %g \n\n", detJoa);

			// Calculating the 6X2 Jacobian based on the WMRA's base in the ground frame:
			phi=atan2(Twc1(1,0),Twc1(0,0));
			Jowc=WMRA_Jga(1, phi, Ta1(0,3), Ta1(1,3));
			fprintf(fid," Jowc is: \n\n");
			for (j=0;j<Jowc.RowNo();j++){
				for (k=0;k<Jowc.ColNo();k++){
					fprintf(fid," %4.2f ",  Jowc(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," phi is: %g \n\n", phi);
			
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
						Joatemp(j,k)=Twc1(j,k);
					}
				}
				for (j=3; j<6; j++){
					for (k=3; k<6; k++){
						Joatemp(j,k)=Twc1(j-3,k-3);
					}
				}
				
				Joatemp=Joatemp*Joa;
				Joa=Joatemp;
				Jowc=Jowc;				
			}
			fprintf(fid," Joa is: \n\n");
				for (j=0;j<Joa.RowNo();j++){
					for (k=0;k<Joa.ColNo();k++){
						fprintf(fid," %4.2f ",  Joa(j,k));
					}
					fprintf(fid," \n");
				}
				fprintf(fid," \n\n\n");

				fprintf(fid," Jowc is: \n\n");
				for (j=0;j<Jowc.RowNo();j++){
					for (k=0;k<Jowc.ColNo();k++){
						fprintf(fid," %4.2f ",  Jowc(j,k));
					}
					fprintf(fid," \n");
				}
				fprintf(fid," \n\n\n");

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
			fprintf(fid," Jo is: \n\n");
			for (j=0;j<Jo.RowNo();j++){
				for (k=0;k<Jo.ColNo();k++){
					fprintf(fid," %4.2f ",  Jo(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," detJo is: %g \n\n", detJo);

			
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
					fprintf(fid," T1 is: \n\n");
					for (j=0;j<T1.RowNo();j++){
						for (k=0;k<T1.ColNo();k++){
							fprintf(fid," %4.2f ",  T1(j,k));
						}
						fprintf(fid," \n");
					}
					fprintf(fid," \n\nTt is: \n\n");
					for (j=0;j<Tttemp.RowNo();j++){
						for (k=0;k<Tttemp.ColNo();k++){
							fprintf(fid," %4.2f ",  Tttemp(j,k));
						}
						fprintf(fid," \n");
					}
					dx=WMRA_delta(T1, Tttemp);
				}
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
			fprintf(fid," dx is: \n\n");
			for (j=0;j<dx.RowNo();j++){
				fprintf(fid," %4.2f ",  dx(j,0));
			}
			fprintf(fid," \n\n\n");
			
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
		
			fprintf(fid," dq is: \n\n");
			for (j=0;j<dq.RowNo();j++){
				fprintf(fid," %4.2f ",  dq(j,0));
			}
			
			
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
				dq1.Null(9,1);
				dq1=WMRA_Opt(optim, JLA, JLO, Jo, detJo, dq, dx, dt, q1);
			}
			fprintf(fid," dq is: \n\n");
			for (j=0;j<dq1.RowNo();j++){
				fprintf(fid," %4.2f ",  dq1(j,0));
			}
			fprintf(fid," \n\n\n");
		//	dq=dq1;
			// Calculating the new Joint Angles:
			qn=q1+dq1;
			fprintf(fid," qn is: \n\n");
			for (j=0;j<qn.RowNo();j++){
				fprintf(fid," %4.2f ",  qn(j,0));
			}
			fprintf(fid," \n\n\n");
			
			// Calculating the new Transformation Matrices:
			Matrix dqtemp(2,1);
			dqtemp(0,0)=dq1(7,0);
			dqtemp(1,0)=dq1(8,0);
			WMRA_Tall(2, qn, dqtemp, Twc1, Tn, Tna, Tnwc, T01, T12, T23, T34, T45, T56, T67);
			fprintf(fid," Tn is: \n\n");
			for (j=0;j<Tn.RowNo();j++){
				for (k=0;k<Tn.ColNo();k++){
					fprintf(fid," %4.2f ",  Tn(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," Tna is: \n\n");
			for (j=0;j<Tna.RowNo();j++){
				for (k=0;k<Tna.ColNo();k++){
					fprintf(fid," %4.2f ",  Tna(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," Tnwc is: \n\n");
			for (j=0;j<Tnwc.RowNo();j++){
				for (k=0;k<Tnwc.ColNo();k++){
					fprintf(fid," %4.2f ",  Tnwc(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			
			// A safety condition function to stop the joints that may cause collision of the arm with itself, the wheelchair, or the human user:
			if (JLO==1 && WCA!=3){
				Matrix dqtemp2(2,1), dq2(7,1),dq3(7,1);
				dqtemp2(0,0)=dq1(7,0);
				dqtemp2(1,0)=dq1(8,0);
				for (j=0;j<7;j++){
					dq2(j,0)=dq1(j,0);
				}
				dq3=WMRA_collide(dq2, T01, T12, T23, T34, T45, T56, T67);
				// Re-calculating the new Joint Angles:
				dq1.Null(9,1);
				for (j=0;j<7;j++){
					dq1(j,0)=dq3(j,0);
				}
				for (j=7;j<9;j++){
					dq1(j,0)=dqtemp2(j-7,0);
				}
				qn=q1+dq1;
												
				// Re-calculating the new Transformation Matrices:				
				WMRA_Tall(2, qn, dqtemp2, Twc1, Tn, Tna, Tnwc, T01, T12, T23, T34, T45, T56, T67);
			}
			fprintf(fid," qn is: \n\n");
			for (j=0;j<qn.RowNo();j++){
				fprintf(fid," %4.2f ",  qn(j,0));
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," Tn is: \n\n");
			for (j=0;j<Tn.RowNo();j++){
				for (k=0;k<Tn.ColNo();k++){
					fprintf(fid," %4.2f ",  Tn(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," Tna is: \n\n");
			for (j=0;j<Tna.RowNo();j++){
				for (k=0;k<Tna.ColNo();k++){
					fprintf(fid," %4.2f ",  Tna(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," Tnwc is: \n\n");
			for (j=0;j<Tnwc.RowNo();j++){
				for (k=0;k<Tnwc.ColNo();k++){
					fprintf(fid," %4.2f ",  Tnwc(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			
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
			/*qo=qn;
			To=Tn;
			Toa=Tna;
			Towc=Tnwc;*/
			tt=tt+dt;
			fprintf(fid," qo is: \n\n");
			for (j=0;j<q1.RowNo();j++){
				fprintf(fid," %4.2f ",  q1(j,0));
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," To is: \n\n");
			for (j=0;j<T1.RowNo();j++){
				for (k=0;k<T1.ColNo();k++){
					fprintf(fid," %4.2f ",  T1(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," Toa is: \n\n");
			for (j=0;j<Ta1.RowNo();j++){
				for (k=0;k<Ta1.ColNo();k++){
					fprintf(fid," %4.2f ",  Ta1(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," Towc is: \n\n");
			for (j=0;j<Twc1.RowNo();j++){
				for (k=0;k<Twc1.ColNo();k++){
					fprintf(fid," %4.2f ",  Twc1(j,k));
				}
				fprintf(fid," \n");
			}
			fprintf(fid," \n\n\n");
			fprintf(fid," tt is %g ",  tt);
			fprintf(fid," \n\n\n");
									
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
			//if (tic < tt){
				//pause(tt-tic);
			//}
		}
		fclose (fid);
		
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
