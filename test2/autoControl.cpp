/* This "new USF WMRA" script SIMULATES the WMRA system with ANIMATION and plots for 9 DOF. All angles are in Radians.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres & Punya A.Basnayaka%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

// Test Delete this line

#include "matrix.h"
#include "var_included.h"
#include "any2ready.h"
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
#include "plots2.h"
#include "park2ready.h" 
#include "Polynomial.h"
#include "q2T.h"
#include "ready2any.h"
#include "ready2park.h"
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
#include "wmraMotion.h"
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <string.h>

using namespace std;
using namespace math;
//using namespace System;
//using namespace System::Windows::Forms;
//using namespace System::Drawing;
//using namespace System::ComponentModel;
//using namespace System::Collections;
//using namespace System::Data;
#define PI 3.14159265
#define d2r PI/180 //Conversions from Degrees to Radians.
#define r2d 180/PI //Conversions from Radians to Degrees.

extern double wmraCtrl[10];
extern float e2r1, e2r2, e2r3, e2r4, e2r5, e2r6, e2r7, e2r8, e2r9, e2d;
extern int traj_exe;
extern int wmraCont; //return value from WMRA thread
extern float x_coord, y_coord, z_coord, a_ang, b_ang, g_ang;
extern bool wmraThreadRet;

// Declaring the global variables to be used for the touch screen control:
//int VAR_DX[7];
//global VAR_SCREENOPN
//static Galil g("COM4 19200");
static Galil g("192.168.1.22");

void slep(long d) //sleep function takes in ms value
{ //sleep(1000) sleeps for 1 second
	clock_t start=clock();
	while(clock() - start < d);
}

int autoControl(Galil &g){
	double wmraRef=6, wmraX, wmraY;
	Matrix L(1,1);
	// Reading the Wheelchair's constant dimentions, all dimentions are converted in millimeters:
	L=WMRA_WCD();
	exitvar=0;
	//cout<<"Hi1";
	g.command("SH"); //turn on motors

	FILE * fid;
	fid = fopen("main.txt","w");
	
	// User input prompts:
	int choice001, choice002, choice003, choice004, choice005, choice006, choice0, choice00, choice1, choice10, choice2, choice3, choice4, choice6, choice7, choice8;
	int WCA, coord, cart, optim, JLA, JLO, cont, trajf, vr, ml, arm, ini, plt;
	int j, k;
	int i,m;
	int port1, ts;
	float v;
	Matrix Td(4,4), Vd(3,1), qi(7,1), WCi(3,1), T_tool(4,4), T_ang(4,4), T_ang_g(4,4);

	//Calculating the initial transformation matrices:
	Matrix dq(1,1), Ti(4,4), Tia(4,4), Tiwco(4,4), T01(4,4), T12(4,4), T23(4,4), T34(4,4), T45(4,4), T56(4,4), T67(4,4), Unit(1,1);
	Unit.Null(2,1);

	Matrix qn(9,1), q_enc(10,1), Tn(4,4), Tna(4,4), Tnwc(4,4);
	Matrix q1(1,1), T1(4,4), Ta1(4,4), Twc1(4,4);
	Matrix dq1(1,1);
	Matrix dqtemp(2,1);
	dqtemp.Null(2,1);

	WCA=2;			//Arm Control only
	coord=2;		//Wheelchair Frame
	cart=1;			//Position and Orientation - both
	optim=1;		//SR-I
	JLA=1;			//JLA applied
	JLO=0;			//Obstacle Avoidance
	cont=1;			//Position Control - not velocity, spaceball etc.
	
	v=50;			//desired velocity of gripper
	trajf=3;		//Polynomial with blending
	vr=0; ml=0;		//No animations
	arm=1;			//Run the WMRA
	ini = 0;	//no park-to-ready
	plt=1;		//No simulation results
	
	WCi.Null(3,1);

	// Calculating the Transformation Matrix of the initial position of the WMRA's base:
	Matrix Tiwc(4,4), qiwc(2,1);
	Tiwc = WMRA_p2T(WCi(0,0),WCi(1,0),WCi(2,0));

	float qi2 [7]= {90, 90, 0, 90, 90, 90, 0};
	for ( j = 0 ; j < 7 ; j++ ) {
		qi(j,0)=qi2[j]*d2r;
	}
		
	//Calculating the initial Wheelchair Variables:
	qiwc(0,0)=sqrt(pow(WCi(0,0),2)+pow(WCi(1,0),2));
	qiwc(1,0)=WCi(2,0);
	
	for (j=0; j<7; j++)
		qn(j,0)=qi(j,0);

	for (j=7; j<9; j++)
		qn(j,0)=qiwc(j-7,0);

	int n;
	float D, dt, total_time, dg;
	Matrix dx(1,1);
	float ***Tt;

	// Initializing the joint angles, the Transformation Matrix, and time:
	Matrix qo(9,1), To(4,4), Towc(4,4), Toa(4,4), Jo(1,1);
	float tt, ddt=0.0;
	dq.Null(9,1);
	dg=0;

	Matrix Joa(6,7), Jowc(1,1), Jowctemp(1,1), Joatemp(1,1);
	float detJoa, detJo, phi;

	// Initializing the Physical Arm:
	if (arm==1){
		Matrix qotemp(1,1);
		qotemp.Null(10,1);
		for (j=0; j<9; j++){
			qotemp(j,0)=qn(j,0);
		}
		qotemp(9,0)=1;
		WMRA_ARM_Motion(1, 2, qotemp, 0, g);
		ddt=0;
	}

	wait(1);

	cout << "Arm initialized..." << endl;

	// Starting a timer:
	clock_t startt, start2, endt, endf, endtt;
	//time_t startt, endt, endf;
	//time(&startt);
	start2=clock()/ CLOCKS_PER_SEC;
	startt=clock()*CLK_TCK/1000;
	cout<<"\nStart is "<<startt<<"\n\n";
	double timedif;

	while (wmraCont)
	{
		if(traj_exe)
		{
			Position_control_2(0, 0, 0, 0, 0, 0, 0, 0, 0, g);
			//cout<<"Hi2";
	
			WMRA_Tall(1, qn, dqtemp, Tiwc, Ti, Tia, Tiwco, T01, T12, T23, T34, T45, T56, T67);
		
	
			Tiwc=Tiwco;

			fprintf(fid," \n\n\n");
			fprintf(fid," Tia is: \n\n");
			for (j=0;j<Tia.RowNo();j++){
				for (k=0;k<Tia.ColNo();k++){
					fprintf(fid," %4.2f ",  Tia(j,k));
				}
				fprintf(fid," \n");
			}

			tt=0;
			i=0;

			T_ang = WMRA_rotz(a_ang)*WMRA_roty(b_ang)*WMRA_rotx(g_ang);
			/*fprintf(stdout," %4.2f %4.2f %4.2f %4.2f \n",  T_ang(0,0), T_ang(0,1), T_ang(0,2), T_ang(0,3));
			fprintf(stdout," %4.2f %4.2f %4.2f %4.2f \n",  T_ang(1,0), T_ang(1,1), T_ang(1,2), T_ang(1,3));
			fprintf(stdout," %4.2f %4.2f %4.2f %4.2f \n",  T_ang(2,0), T_ang(2,1), T_ang(2,2), T_ang(2,3));
			fprintf(stdout," %4.2f %4.2f %4.2f %4.2f \n",  T_ang(3,0), T_ang(3,1), T_ang(3,2), T_ang(3,3));
			fprintf(stdout, "\n\n");*/

			T_ang_g = Tia * T_ang;

			Td(0,0) = T_ang_g(0,0);	Td(0,1) = T_ang_g(0,1);	Td(0,2) = T_ang_g(0,2);	Td(0,3) = Tia(0,3) + x_coord;
			Td(1,0) = T_ang_g(1,0);	Td(1,1) = T_ang_g(1,1);	Td(1,2) = T_ang_g(1,2);	Td(1,3) = Tia(1,3) + y_coord;
			Td(2,0) = T_ang_g(2,0);	Td(2,1) = T_ang_g(2,1);	Td(2,2) = T_ang_g(2,2);	Td(2,3) = Tia(2,3) + z_coord;
			Td(3,0) = Tia(3,0);		Td(3,1) = Tia(3,1);		Td(3,2) = Tia(3,2);		Td(3,3) = Tia(3,3);

			fprintf(stdout," %4.2f %4.2f %4.2f %4.2f \n",  Td(0,0), Td(0,1), Td(0,2), Td(0,3));
			fprintf(stdout," %4.2f %4.2f %4.2f %4.2f \n",  Td(1,0), Td(1,1), Td(1,2), Td(1,3));
			fprintf(stdout," %4.2f %4.2f %4.2f %4.2f \n",  Td(2,0), Td(2,1), Td(2,2), Td(2,3));
			fprintf(stdout," %4.2f %4.2f %4.2f %4.2f \n",  Td(3,0), Td(3,1), Td(3,2), Td(3,3));
			fprintf(stdout, "\n\n");
	
			//Calculating the linear distance from the initial position to the desired position and the linear velocity:
			D=sqrt(pow(Td(0,3)-Tia(0,3),2) + pow(Td(1,3)-Tia(1,3),2) + pow(Td(2,3)-Tia(2,3),2));
			fprintf(fid," D is: %g \n\n", D);
			fprintf(fid," \n\n\n");
		
			//Calculating the number of iteration and the time increment (delta t) if the linear step increment of the tip is 1 mm:
			dt=0.05;    // Time increment in seconds.
			total_time=D/v;     // Total time of animation.
			n=(total_time/dt); // Number of iterations rounded up.
			dt=total_time/n;    // Adjusted time increment in seconds.
			fprintf(fid," Total time is: %4.2f \n\n", total_time);
			fprintf(fid," n is: %g \n\n", n);
			fprintf(fid," dt is: %g \n\n", dt);
			fprintf(fid," \n\n\n");

			// Calculating the Trajectory of the end effector, and once the trajectory is calculated, we should redefine "Td" based on the ground frame:
			FILE * fid2;
			fid2 = fopen("traj.txt","w");
		
			Tt=WMRA_traj(trajf,Tia,Td,n+1);
			Td=Tiwc*Td;

			cout << "Trajectory initialized" << endl;
		
			fprintf(fid2," Trajectory is \n\n");
			for (m=0;m<n+1;m++){
				for (j=0;j<4;j++){
					for (k=0;k<4;k++){
						fprintf(fid2," %4.2f ",  Tt[m][j][k]);
					}
					fprintf(fid2," \n");
				}
				fprintf(fid2," \n\n\n");
			}
			fprintf(fid2," \n\n\n");
			fclose (fid2);

			// Starting the Iteration Loop:
			//for (i=0;i<n;i++){
			int flagspace=0;
			Matrix space(6,1);
			while (i<=n){
				if (i==0){
					q1=qn;
					T1=Ti;
					Ta1=Tia;
					Twc1=Tiwc;
				}
				else{
					q1=qn;
					T1=Tn;
					Ta1=Tna;
					Twc1=Tnwc;
					dq=dq1;
				}
				
				//cout<<"Hi3";
			
				// Calculating the 6X7 Jacobian of the arm in frame 0:
				WMRA_J07(T01, T12, T23, T34, T45, T56, T67, Joa, detJoa);
							
				// Calculating the 6X2 Jacobian based on the WMRA's base in the ground frame:
				phi=atan2(Twc1(1,0),Twc1(0,0));
				Jowc=WMRA_Jga(1, phi, Ta1(0,3), Ta1(1,3));
			
				// Changing the Jacobian reference frame based on the choice of which coordinates frame are referenced in the Cartesian control:
				// coord=1 for Ground Coordinates Control.
				// coord=2 for Wheelchair Coordinates Control.
				// coord=3 for Gripper Coordinates Control.
				if (coord==2){
					Joa=Joa;
					Jowctemp.Null(6,6);
					for (j=0; j<3; j++){
						for (k=0; k<3; k++){
							Jowctemp(j,k)=Twc1(k,j);
						}
					}
					for (j=3; j<6; j++){
						for (k=3; k<6; k++){
							Jowctemp(j,k)=Twc1(k-3,j-3);
						}
					}
					Jowctemp=Jowctemp*Jowc;
					Jowc=Jowctemp;
				}
			
							
				//Calculating the 3X9 or 6X9 augmented Jacobian of the WMRA system based on the ground frame:
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
	
						
				// Finding the Cartesian errors of the end effector:
				Matrix invTowc(1,1), invTo(1,1), Towctemp(1,1), Towctemp2(1,1), Tttemp(4,4), Ttnew(4,4);
				if (cont==1){
					// Calculating the Position and Orientation errors of the end effector, and the rates of motion of the end effector:            
					if (coord==2){
						invTowc.Unit(4);
						Towctemp=Twc1;
						Towctemp.SetSize(3,3);
						Towctemp2=~Towctemp;					
						Towctemp2*=(-1);
						Towctemp.Null(3,1);
						for (j=0; j<3; j++){
							Towctemp(j,0)=Twc1(j,3);
						}
						Towctemp=Towctemp2*Towctemp;
						for (j=0; j<3; j++){
							for (k=0; k<3; k++){
								invTowc(j,k)=Twc1(k,j);
							}
							invTowc(j,3)=Towctemp(j,0);
						}
						
						Tttemp.Null(4,4);
						for (j=0; j<4; j++){
							for (k=0; k<4; k++){
								Tttemp(j,k)=Tt[i][j][k];	        			
							}
						}
						Ttnew=invTowc*Tiwc*Tttemp;
						dx=WMRA_delta(Ta1, Ttnew);
					}
				}
			
				fprintf(fid," dx is: \n\n");
				for (j=0;j<dx.RowNo();j++){
					fprintf(fid," %4.2f ",  dx(j,0));
				}
				fprintf(fid," \n\n\n");			
			
				// Calculating the resolved rate with optimization:
				// Index input values for "optim": 1= SR-I & WLN, 2= P-I & WLN, 3= SR-I & ENE, 4= P-I & ENE:
				fprintf(fid," dq is: \n\n");
				for (j=0;j<dq.RowNo();j++){
					fprintf(fid," %4.2f ",  dq(j,0));
				}
				fprintf(fid," \n\n\n");
				
				if (WCA==2) {
					dq.SetSize(7,1);
					dq1.Null(7,1);
					dq1=WMRA_Opt(optim, JLA, JLO, Joa, detJoa, dq, dx, dt, q1);
					dq1.SetSize(9,1);
				}
				
				fprintf(fid," dq is: \n\n");
				for (j=0;j<dq1.RowNo();j++){
					fprintf(fid," %4.2f ",  dq1(j,0));
				}
				fprintf(fid," \n\n\n");
				
				// Calculating the new Joint Angles:			
				qn=q1+dq1;
				fprintf(fid," qn is: \n\n");
				for (j=0;j<qn.RowNo();j++){
					fprintf(fid," %4.2f ",  qn(j,0));
				}
				fprintf(fid," \n\n\n");

				//// Updating Physical Arm:
				//if (arm==1){
				//	ddt=ddt+dt;
				//	if (ddt>=0.1 || i>=(n+1)){
				//		Matrix dqtemp(10,1);
				//		for (j=0; j<9; j++){
				//			dqtemp(j,0)=qn(j,0);	
				//		}
				//		dqtemp(9,0)=dg;
				//		
				//		q_enc=WMRA_ARM_Motion(2, 1, dqtemp, ddt, g);
				//		cout<<"dqtemp(9,0)"<<dqtemp(9,0)<<endl;
				//		wait(.1);
				//		ddt=0;

				//		cout << " q_enc: " << q_enc(0,0) << "\t" << q_enc(1,0) << "\t" << q_enc(2,0) << "\t" << q_enc(3,0) << "\t" << q_enc(4,0) << "\t" << q_enc(5,0) << "\t" << q_enc(6,0) << q_enc(7,0) << q_enc(8,0) << endl;

				//		for (j=0;j<9;j++)
				//			qn(j,0)=q_enc(j,0);
				//	}
				//}

				// Updating Physical Arm:
				if (arm==1){
					Position_control_2(qn(0,0), qn(1,0), qn(2,0), qn(3,0), qn(4,0), qn(5,0), qn(6,0), 30000, 1, g);
				}



				// Calculating the new Transformation Matrices:
				Matrix dqtemp(2,1);
				dqtemp(0,0)=dq1(7,0);
				dqtemp(1,0)=dq1(8,0);
				WMRA_Tall(2, qn, dqtemp, Twc1, Tn, Tna, Tnwc, T01, T12, T23, T34, T45, T56, T67);
			
				fprintf(fid," Tna is: \n\n");
				for (j=0;j<Tna.RowNo();j++){
					for (k=0;k<Tna.ColNo();k++){
						fprintf(fid," %4.2f ",  Tna(j,k));
					}
					fprintf(fid," \n");
				}
				fprintf(fid," \n\n\n");
			
				// A safety condition function to stop the joints that may cause colision of the arm with itself, the wheelchair, or the human user:
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
				
				fprintf(fid," Tna is: \n\n");
				for (j=0;j<Tna.RowNo();j++){
					for (k=0;k<Tna.ColNo();k++){
						fprintf(fid," %4.2f ",  Tna(j,k));
					}
					fprintf(fid," \n");
				}
				fprintf(fid," \n\n\n");
						
				// Updating the old values with the new values for the next iteration:
				tt=tt+dt;
				fprintf(fid," tt is %g ",  tt);
				fprintf(fid," \n\n\n");
							
				// Delay to comply with the required speed:
				endt=0;
				//time(&endt);
				endt=clock()/ CLOCKS_PER_SEC;
				//endt=clock()*CLK_TCK; 
				timedif=0;
				//timedif=difftime(endt,startt);
				timedif=(endt-start2);
				if (timedif < tt){
					//wait((tt-timedif));
				}
				//endtt=clock()*CLK_TCK; 
				//cout<<"\n"<<endtt;
				i++;
			}
			
			traj_exe=0;
			Position_control_2(0, 0, 0, 0, 0, 0, 0, 0, 2, g);
		}// if(traj_exe)

	}//while(wmraCont)

		fclose(fid);

		cout << "WMRA thread trajectory over..." << endl;

		// Reading the elapsed time and printing it with the simulation time:
		if (cont==1 || cont==2){
			printf("\nSimula. time is %6.6f seconds.\n", total_time);
		}
		//time(&endf);
		//endf=clock()/ CLOCKS_PER_SEC;
		endf=clock()*CLK_TCK/1000;
		cout<<"\nEnd is "<<endf<<"\n\n";
		//cout<<"\nElapsed time is "<<difftime(endf,startt)*1000<<"\n\n";
		cout<<"\nElapsed time is "<<(endf-startt)<<"\n\n";
						
		if (vr==1 || ml==1 || arm==1){
			// Going back to the ready position:
			cout << "\n Do you want to go back to the 'ready' position? \n Press '1' for Yes, or press '2' for No. \n\n ";
			cin >> choice6;
			if (choice6==1){
				WMRA_any2ready(2, vr, ml, arm, Tnwc, qn, g);
				// Going back to the parking position:
				cout << "\n Do you want to go back to the 'parking' position? \n Press '1' for Yes, or press '2' for No. \n\n ";
				cin >> choice7;
					if (choice7==1){
						Matrix temp(2,1);
						temp(0,0)=qn(7,0);
						temp(1,0)=qn(8,0);
						WMRA_ready2park(2, vr, ml, arm, Tnwc, temp, g);
					}
			}
			//
			//// Closing the Arm library and Matlab Graphics Animation and Virtual Reality Animation and Plots windows:
			//cout << "\n Do you want to close all simulation windows and arm controls? \n Press '1' for Yes, or press '2' for No. \n\n ";
			//cin >> choice8;
			//if (choice8==1){
			//	Matrix temp(1,1);
			//	temp.Null(1,1);
			//	if (arm==1){
			//		WMRA_ARM_Motion(3, 0, temp, 0, g);
			//	}
			//}
		}
		wmraThreadRet = false;
  		return 0;
}
