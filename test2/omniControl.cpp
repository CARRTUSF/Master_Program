/* This "new USF WMRA" script SIMULATES the WMRA system with ANIMATION and plots for 9 DOF. All angles are in Radians.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Modified By:Ana Catalina Torres & Punya A.Basnayaka%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2010%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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
//#include "maintenance.h"
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <sstream>
#include <string.h>

using namespace std;
using namespace math;
//using namespace System;
/*using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Data;*/
#define PI 3.14159265
#define d2r PI/180 //Conversions from Degrees to Radians.

// Declaring the global variables to be used for the touch screen control:
//int VAR_DX[7];
//global VAR_SCREENOPN

void sleep(long d) //sleep function takes in ms value
{ //sleep(1000) sleeps for 1 second
	clock_t start=clock();
	while(clock() - start < d);
}

//static Galil g("COM4 19200");
//static Galil g("192.168.1.104");

//int varscreenopn;
extern double wmraCtrl[10];
extern bool gGripperFrame;
extern bool gscaledrot;
bool scaledrot_first_time=true;
extern bool gFwdKin;

extern bool takeTa1;
extern bool autonomousFlag;
extern bool saveFileFlag;
int poseNum = 0;
extern FILE * fidTrain;


Matrix UnitVec(Matrix);
Matrix CalcProjectionVector(Matrix, Matrix);
float vector_mag(Matrix);

int omniControl(Galil &g){


	Matrix L(1,1);
	// Reading the Wheelchair's constant dimensions, all dimentions are converted in millimeters:
	L=WMRA_WCD();
	exitvar=0;

	g.command("SH"); //turn on motors

	//initialize lab jack wheelchair controller
	//if (Initialize())
	//{
	//	cerr << "There was a problem opening the Lab Jack!" << endl;
	//	Stop();
	//	return 1;
	//}
	//Stop();
	
	// User input prompts:
	int choice001, choice002, choice003, choice004, choice005, choice006, choice0, choice00, choice1, choice10, choice2, choice3, choice4, choice6, choice7, choice8;
	int WCA, coord, cart, optim, JLA, JLO, vr, ml, arm;
	int j, k;
	int i;
	int port1, ts;
	Matrix Vd(3,1), qi(9,1), WCi(3,1);

	//scaled rotation - reference rotation variables
	float nx1_ref, ny1_ref, nz1_ref;
	float ox1_ref, oy1_ref, oz1_ref;
	float ax1_ref, ay1_ref, az1_ref;

	float nx2_ref1, ny2_ref1, nz2_ref1;
	float ox2_ref1, oy2_ref1, oz2_ref1;
	float ax2_ref1, ay2_ref1, az2_ref1;
	float wx_ref1, wy_ref1, wz_ref1;

	float nx2_ref2, ny2_ref2, nz2_ref2;
	float ox2_ref2, oy2_ref2, oz2_ref2;
	float ax2_ref2, ay2_ref2, az2_ref2;
	float wx_ref2, wy_ref2, wz_ref2;

	float wx, wy, wz;

	WCA=2;			//Arm Control only
	if (gGripperFrame)
		coord=3;		//Wheelchair Frame=2, Gripper Frame=3
	else
		coord=2;
	cart=1;			//Position and Orientation - both
	optim=1;		//SR-I
	JLA=1;			//JLA applied
	JLO=0;			//Obstacle Avoidance
	vr=0; ml=0;		//No animations
	arm=1;			//Run the WMRA
	
	//float qi2 [7]= {90, 90, 0, 90, 90, 90, 0};// ready position #1
	//float qi2 [7]= {90, 0, -90, 180, 0, 60, 0};// ready position #3
	//float qi2 [7]= {90, 90, -90, 180, 0, 60, 0}; //#4
	float qi2 [7]= {90, 90, 0, 90, 90, 60, 0};// ready position joint angles #2
	//float qi2 [7]= {90, 90, 0, 90, 120, 60, 0};// ready position joint angles #5
	//float qi2 [7]= {90, 90, 0, 90, 60, 60, 0};// ready position joint angles #6
	//float qi2 [7]= {90, 60, 0, 90, 60, 60, 0};// ready position joint angles #7
	//float qi2 [7]= {90, 90, 30, 90, 90, 60, 0};// ready position joint angles #8
	for ( j = 0 ; j < 7 ; j++ ) {
		qi(j,0)=qi2[j]*d2r;
	}
	WCi.Null(3,1);
	
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
		
	FILE * fid;
	fid = fopen("main.txt","w");
	FILE * poseData;
	poseData = fopen("posData.txt","w");
	ifstream source;

	int o,m;
	int cont = 1;
	int trajf=3;		//Polynomial with blending;
	int v=50;			//desired velocity of gripper
	Matrix Td(4,4), q_enc(10,1);
	Matrix dqtemp(2,1);
	dqtemp.Null(2,1);
	
	Tiwc=Tiwco;
		
	float D, dt, total_time, n, dg;
	Matrix dx(1,1);
	float ***Tt;
	
	n=1;			
	dt=0.05;
	
	// Initializing the joint angles, the Transformation Matrix, and time:
	Matrix qo(9,1), To(4,4), Towc(4,4), Toa(4,4), Jo(1,1);
	float tt, ddt;
	dq.Null(9,1);
	dg=0;
	for (j=0; j<7; j++){
		qo(j,0)=qi(j,0);
	}
	for (j=7; j<9; j++){
		qo(j,0)=qiwc(j-7,0);
	}
	
	To=Ti;
	Toa=Tia;
	Towc=Tiwc;
	tt=0;
	i=0;
	/*dH_prev.Null(7,1);

	fprintf(stdout,"In Main function:\n");
	for (j=0; j<7; j++)
		fprintf(stdout,"%f\t", dH_prev(j,0));
	fprintf(stdout,"\n");*/
	
	// Initializing the Physical Arm:
	if (arm==1){
		Matrix qotemp(1,1);
		qotemp.Null(10,1);
		for (j=0; j<9; j++){
			qotemp(j,0)=qo(j,0);
		}
		qotemp(9,0)=1;
		WMRA_ARM_Motion(1, 2, qotemp, 0, g);
		ddt=0;
	}

	cout << "Arm Initialized" << endl;

	// Re-Drawing the Animation:
	Matrix Joa(6,7), Jowc(1,1), Jowctemp(1,1), Joatemp(1,1), map_grip_base(6,6);
	float detJoa, detJo, phi;
	
	// Starting a timer:
	clock_t startt, start2, endt, endf, endtt;
	//time_t startt, endt, endf;
	//time(&startt);
	start2=clock()/ CLOCKS_PER_SEC;
	startt=clock()*CLK_TCK/1000;
	cout<<"\nStart is "<<startt<<"\n\n";
	double timedif;
	/*Stopwatch ^ timePerParse;
	timePerParse = Stopwatch::StartNew();
//	Stopwatch::StartNew();*/
		
	Matrix qn(1,1), Tn(4,4), Tna(4,4), Tnwc(4,4);
	Matrix q1(1,1), T1(4,4), Ta1(4,4), Twc1(4,4), Ta1_a(4,4);
	Matrix dq1(1,1);

	Matrix K_ref1(3,1), K_ref2(3,1), unit_K_ref1(3,1), unit_K_ref2(3,1), K(3,1), unit_K(3,1), k_proj1(3,1), k_proj2(3,1);
	float mag_k_proj1=0.0, mag_k_proj2=0.0;

	cout << "Start Loop" << endl;

	// Starting the Iteration Loop:
	//for (i=0;i<n;i++){

	while (i<=n+1){
		if (i==0){
			q1=qo;
			T1=To;
			Ta1=Toa;
			Twc1=Towc;
		}
		else if(autonomousFlag)
		{			
			autonomousFlag = false;
			q1=qn;
			source.open("posData.txt", ios_base::in);
			//poseData = fopen("posData.txt", "r");

			Unit = dqtemp;
			WMRA_Tall(1, q1, dqtemp, Twc1, Ti, Tia, Tiwco, T01, T12, T23, T34, T45, T56, T67);
			Twc1=Tiwco;

			string line;
			for (j=0;j<Ta1_a.RowNo();j++)
			{
				for (k=0;k<Ta1_a.ColNo();k++)
				{
					getline(source, line);
					istringstream in(line);
					in >> Ta1_a(j,k);

					//fscanf(poseData, "%4.2f", Ta1_a(j,k));
					cout << Ta1_a(j,k) << endl;
				}
				//fscanf(poseData, "/n", timedif); //timedif used so a different veriable doesnt need to be initialized
			}
			cout<<"Started Autonomous Movement" << endl;
 
			
			tt=0;
			o=0;

			Td(0,0) = Ta1_a(0,0);	Td(0,1) = Ta1_a(0,1);	Td(0,2) = Ta1_a(0,2);	Td(0,3) = Ta1_a(0,3);
			Td(1,0) = Ta1_a(1,0);	Td(1,1) = Ta1_a(1,1);	Td(1,2) = Ta1_a(1,2);	Td(1,3) = Ta1_a(1,3);
			Td(2,0) = Ta1_a(2,0);	Td(2,1) = Ta1_a(2,1);	Td(2,2) = Ta1_a(2,2);	Td(2,3) = Ta1_a(2,3);
			Td(3,0) = Ta1_a(3,0);	Td(3,1) = Ta1_a(3,1);	Td(3,2) = Ta1_a(3,2);	Td(3,3) = Ta1_a(3,3);

			//Calculating the linear distance from the initial position to the desired position and the linear velocity:
			D=sqrt(pow(Td(0,3)-Tia(0,3),2) + pow(Td(1,3)-Tia(1,3),2) + pow(Td(2,3)-Tia(2,3),2));

			//Calculating the number of iteration and the time increment (delta t) if the linear step increment of the tip is 1 mm:
			dt=0.05;    // Time increment in seconds.
			total_time=D/v;     // Total time of animation.
			m=ceil(total_time/dt); // Number of iterations rounded up.
			dt=total_time/m;    // Adjusted time increment in seconds.
			
			// Calculating the Trajectory of the end effector, and once the trajectory is calculated, we should redefine "Td" based on the ground frame:
			Tt=WMRA_traj(trajf,Tia,Td,m+1);
			Td=Twc1*Td;

			cout << "Trajectory initialized" << endl;

			// Starting the Iteration Loop:
			while (o<=m){
				if (o==0){
					q1=qn;
					T1=Ti;
					Ta1=Tia;
					//Twc1=Tnwc;
					dq=dq1;
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
					
				// Calculating the 6X2 Jacobian based on the WMRA's base in the ground frame:
				phi=atan2(Twc1(1,0),Twc1(0,0));
				Jowc=WMRA_Jga(1, phi, Ta1(0,3), Ta1(1,3));
cout << "Jacobian Finished"	<< endl;
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
				else if (coord==3){
					Joatemp.Null(6,6);
					for (j=0; j<3; j++){
						for (k=0; k<3; k++){
							Joatemp(j,k)=Ta1(k,j);
						}
					}
					for (j=3; j<6; j++){
						for (k=3; k<6; k++){
							Joatemp(j,k)=Ta1(k-3,j-3);
						}
					}
					map_grip_base=Joatemp;
					Joatemp=Joatemp*Joa;
					Joa=Joatemp;
					Jowctemp.Null(6,6);
					for (j=0; j<3; j++){
						for (k=0; k<3; k++){
							Jowctemp(j,k)=T1(k,j);
						}
					}
					for (j=3; j<6; j++){
						for (k=3; k<6; k++){
							Jowctemp(j,k)=T1(k-3,j-3);
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
cout << "coord Finished" << endl;
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
cout << "other jacobian Finished" << endl;
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
								Tttemp(j,k)=Tt[o][j][k];	        			
							}
						}
						Ttnew=invTowc*Tiwc*Tttemp;
						dx=WMRA_delta(Ta1, Ttnew);
					}
				}
cout << "cont Finished" << endl;
				
				// Calculating the resolved rate with optimization:
				// Index input values for "optim": 1= SR-I & WLN, 2= P-I & WLN, 3= SR-I & ENE, 4= P-I & ENE:
				if (WCA==2) {
					dq.SetSize(7,1);
					dq1.Null(7,1);
					dq1=WMRA_Opt(optim, JLA, JLO, Joa, detJoa, dq, dx, dt, q1);
					dq1.SetSize(9,1);
				}
cout << "WCA Finished" << endl;
				// Calculating the new Joint Angles:			
				qn=q1+dq1;
cout << "new joint angles Finished" << endl;
				// Updating Physical Arm:
				if (arm==1){
cout << "In ARM if statement" << endl;
					ddt=ddt+dt;
					if (ddt>=0.5 || o>=(m+1)){  // if (ddt>=0.5 || o>=(m+1)){  <- original
cout << "In other if statement" << endl;
						Matrix Unit(10,1);
						for (j=0; j<9; j++){
							Unit(j,0)=qn(j,0);	
						}
						Unit(9,0)=dg;
						
						q_enc=WMRA_ARM_Motion(2, 1, Unit, ddt, g);
						cout<<"Unit(9,0)"<<Unit(9,0)<<endl;
						wait(.5);
						ddt=0;

						cout << " q_enc: " << q_enc(0,0) << "\t" << q_enc(1,0) << "\t" << q_enc(2,0) << "\t" << q_enc(3,0) << "\t" << q_enc(4,0) << "\t" << q_enc(5,0) << "\t" << q_enc(6,0) << q_enc(7,0) << q_enc(8,0) << endl;

						for (j=0;j<9;j++)
							qi(j,0)=q_enc(j,0);
						
					}
				}
cout << "new transform Finished" << endl;
				// Calculating the new Transformation Matrices:
				Matrix Unit(2,1);
				Unit(0,0)=dq1(7,0);
				Unit(1,0)=dq1(8,0);
				WMRA_Tall(2, qi, Unit, Twc1, Tn, Tna, Tnwc, T01, T12, T23, T34, T45, T56, T67);

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
					qi=q1+dq1;
					
					// Re-calculating the new Transformation Matrices:				
					WMRA_Tall(2, qi, dqtemp2, Twc1, Tn, Tna, Tnwc, T01, T12, T23, T34, T45, T56, T67);
				}

				// Updating the old values with the new values for the next iteration:
				tt=tt+dt;

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

				o++;
cout << "END of Loop" << endl;

			}

			q1=qn;
			T1=Tn;
			Ta1=Tna;
			Twc1=Tnwc;
			dq=dq1;

			cout << "Finished Autonomous Movement" << endl;

		}
		else
		{
			q1=qn;
			T1=Tn;
			Ta1=Tna;
			Twc1=Tnwc;
			dq=dq1;
		}
//////////////////////////////////////////////////////////////////////////////////// D //////////////////////////////////////////////////////////////////////////////

		if(takeTa1)
		{
			takeTa1=false;
			poseNum++;
			//fprintf(poseData, "# %i\n", poseNum);
			fprintf(stdout,"Joint 7 frame wrt base from Fwd Kin is:\n");
			for (j=0;j<Ta1.RowNo();j++)
			{
				for (k=0;k<Ta1.ColNo();k++)
				{
					fprintf(stdout,"%4.2f\t",  Ta1(j,k));
					fprintf(poseData," %4.2f\n", Ta1(j,k));
				}
				//fprintf(poseData,"\n");
				fprintf(stdout,"\n");
			}
		}
		if(saveFileFlag)
		{
			saveFileFlag = false;
			fclose(poseData);
		}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (gGripperFrame)
			coord=3;		//Wheelchair Frame=2, Gripper Frame=3
		else
			coord=2;

		if (gscaledrot)
		{
			if (scaledrot_first_time)
			{
				scaledrot_first_time=false;

				nx1_ref = Ta1(0,0);	ox1_ref = Ta1(0,1);	ax1_ref = Ta1(0,2);
				ny1_ref = Ta1(1,0);	oy1_ref = Ta1(1,1);	ay1_ref = Ta1(1,2);
				nz1_ref = Ta1(2,0);	oz1_ref = Ta1(2,1);	az1_ref = Ta1(2,2);

				//Grasp from top configuration
				/*nx2_ref1 = 0.28;	ox2_ref1 = -0.79;	ax2_ref1 = 0.54;
				ny2_ref1 = -0.84;	oy2_ref1 = 0.07;	ay2_ref1 = 0.54;
				nz2_ref1 = -0.47;	oz2_ref1 = -0.60;	az2_ref1 = -0.64;*/
				nx2_ref1 = 0.0;		ox2_ref1 = -1.0;	ax2_ref1 = 0.0;
				ny2_ref1 = -1.0;	oy2_ref1 = 0.0;		ay2_ref1 = 0.0;
				nz2_ref1 = 0.0;		oz2_ref1 = 0.0;		az2_ref1 = -1.0;

				//Grasp from side configuration
				/*nx2_ref2 = 0.35;	ox2_ref2 = 0.47;	ax2_ref2 = 0.81;
				ny2_ref2 = -0.93;	oy2_ref2 = 0.10;	ay2_ref2 = 0.35;
				nz2_ref2 = 0.08;	oz2_ref2 = -0.88;	az2_ref2 = 0.48;*/
				nx2_ref2 = 0.0;		ox2_ref2 = 0.0;		ax2_ref2 = 1.0;
				ny2_ref2 = -1.0;	oy2_ref2 = 0.0;		ay2_ref2 = 0.0;
				nz2_ref2 = 0.0;		oz2_ref2 = -1.0;	az2_ref2 = 0.0;

				wx_ref1 = 0.5*(oy1_ref*oz2_ref1 - oz1_ref*oy2_ref1 + ny1_ref*nz2_ref1 - nz1_ref*ny2_ref1 + ay1_ref*az2_ref1 - az1_ref*ay2_ref1);
				wy_ref1 = 0.5*(oz1_ref*ox2_ref1 - ox1_ref*oz2_ref1 + nz1_ref*nx2_ref1 - nx1_ref*nz2_ref1 + az1_ref*ax2_ref1 - ax1_ref*az2_ref1);
				wz_ref1 = 0.5*(ox1_ref*oy2_ref1 - oy1_ref*ox2_ref1 + nx1_ref*ny2_ref1 - ny1_ref*nx2_ref1 + ax1_ref*ay2_ref1 - ay1_ref*ax2_ref1);

				wx_ref2 = 0.5*(oy1_ref*oz2_ref2 - oz1_ref*oy2_ref2 + ny1_ref*nz2_ref2 - nz1_ref*ny2_ref2 + ay1_ref*az2_ref2 - az1_ref*ay2_ref2);
				wy_ref2 = 0.5*(oz1_ref*ox2_ref2 - ox1_ref*oz2_ref2 + nz1_ref*nx2_ref2 - nx1_ref*nz2_ref2 + az1_ref*ax2_ref2 - ax1_ref*az2_ref2);
				wz_ref2 = 0.5*(ox1_ref*oy2_ref2 - oy1_ref*ox2_ref2 + nx1_ref*ny2_ref2 - ny1_ref*nx2_ref2 + ax1_ref*ay2_ref2 - ay1_ref*ax2_ref2);

				K_ref1(0,0)=wx_ref1; K_ref1(1,0)=wy_ref1; K_ref1(2,0)=wz_ref1;
				K_ref2(0,0)=wx_ref2; K_ref2(1,0)=wy_ref2; K_ref2(2,0)=wz_ref2;

				unit_K_ref1 = UnitVec(K_ref1);
				unit_K_ref2 = UnitVec(K_ref2);

				fprintf(stdout, "Reference vectors are \n%f\t%f\t%f\n%f\t%f\t%f\n", unit_K_ref1(0,0), unit_K_ref1(1,0), unit_K_ref1(2,0), unit_K_ref2(0,0), unit_K_ref2(1,0), unit_K_ref2(2,0));
			}
		}
		else
			scaledrot_first_time=true;

			
		// Calculating the 6X7 Jacobian of the arm in frame 0:
		WMRA_J07(T01, T12, T23, T34, T45, T56, T67, Joa, detJoa);
		fprintf(fid," i is: %d\n\n",i);
		
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
		else if (coord==3){
			Joatemp.Null(6,6);
			for (j=0; j<3; j++){
				for (k=0; k<3; k++){
					Joatemp(j,k)=Ta1(k,j);
				}
			}
			for (j=3; j<6; j++){
				for (k=3; k<6; k++){
					Joatemp(j,k)=Ta1(k-3,j-3);
				}
			}
			map_grip_base=Joatemp;
			Joatemp=Joatemp*Joa;
			Joa=Joatemp;
			Jowctemp.Null(6,6);
			for (j=0; j<3; j++){
				for (k=0; k<3; k++){
					Jowctemp(j,k)=T1(k,j);
				}
			}
			for (j=3; j<6; j++){
				for (k=3; k<6; k++){
					Jowctemp(j,k)=T1(k-3,j-3);
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

		//Calculating the 3X9 or 6X9 augmented Jacobian of the WMRA system based on the ground frame:
		if (cart==2){
			Joa.SetSize(3,7);
			Joatemp.Null(7,3);
			Joatemp=~Joa;
			Joatemp=Joa*Joatemp;
			detJoa=sqrt(Joatemp.Det());
			Jowc.SetSize(3,2);
			Jo.SetSize(3,9);
			for (j=0; j<3; j++){
				for (k=0; k<7; k++){
					Jo(j,k)=Joa(j,k);
				}
				for (k=7; k<9; k++){
					Jo(j,k)=Jowc(j,k-7);
				}
			}
			Joatemp.Null(9,3);
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
		Matrix invTowc(1,1), invTo(1,1), Towctemp(1,1), Towctemp2(1,1), Tttemp(4,4), Ttnew(4,4);

		dx.Null(6,1);

		dx(0,0) = wmraCtrl[0];
		dx(1,0) = wmraCtrl[1];
		dx(2,0) = wmraCtrl[2];
		dx(3,0) = wmraCtrl[3];
		dx(4,0) = wmraCtrl[4];
		dx(5,0) = wmraCtrl[5];

		if (coord==3)
			dx=map_grip_base*dx;

		dg = wmraCtrl[6];
		varscreenopn = wmraCtrl[7];

		if (gscaledrot)
		{


			wx=dx(3,0);
			wy=dx(4,0);
			wz=dx(5,0);

			K(0,0)=wx; K(1,0)=wy; K(2,0)=wz;
			unit_K = UnitVec(K);

			k_proj1 = CalcProjectionVector(unit_K_ref1, unit_K);
			k_proj2 = CalcProjectionVector(unit_K_ref2, unit_K);

			mag_k_proj1 = vector_mag(k_proj1);
			mag_k_proj2 = vector_mag(k_proj2);

			if (mag_k_proj1!=0.0 || mag_k_proj2!=0.0)
				fprintf(fidTrain, "%f\t%f\n", mag_k_proj1, mag_k_proj2);
			
			/*wx = wx * mag_k_proj1;
			wy = wy * mag_k_proj1;
			wz = wz * mag_k_proj1;*/

			dx(3,0)=wx;
			dx(4,0)=wy;
			dx(5,0)=wz;
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
		//	q1.SetSize(7,1);

			dq1=WMRA_Opt(optim, JLA, JLO, Joa, detJoa, dq, dx, dt, q1);
			dq1.SetSize(9,1);
		}
		else if (WCA==3) {
			Matrix dxtemp(2,1);
			dqtemp(0,0)=dq(7,0);
			dqtemp(1,0)=dq(8,0);
			dxtemp(0,0)=dx(0,0);
			dxtemp(1,0)=dx(1,0);
			Unit.Unit(1);
			dq1.Null(2,1);
			dq1=WMRA_Opt(optim, JLA, JLO, Jowc, 1, dqtemp, dxtemp, dt, Unit);
			dqtemp.Null(9,1);
			dqtemp(7,0)=dq1(0,0);	
			dqtemp(8,0)=dq1(1,0);	
			dq1=dqtemp;
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
		
		// Calculating the new Joint Angles:			
		qn=q1+dq1;
		/*for(j=1;j<7;j++)
			qn(j,0)=qi(j,0);
		qn(1,0)=qi(1,0);
		qn(2,0)=qi(2,0);
		qn(3,0)=qi(3,0);
		qn(4,0)=qi(4,0);
		qn(5,0)=qi(5,0);
		qn(6,0)=qi(6,0);*/

		fprintf(fid," qn is: \n\n");
		for (j=0;j<qn.RowNo();j++){
			fprintf(fid," %4.2f ",  qn(j,0));
		}
		fprintf(fid," \n\n\n");

		//cout<< qn(0,0)<< " " << qn(1,0) << " " << qn(2,0) << " " << qn(3,0) << " " << qn(4,0) << " " << qn(5,0) << " " << qn(6,0) << endl;

		// Calculating the new Transformation Matrices:
		dqtemp(0,0)=dq1(7,0);
		dqtemp(1,0)=dq1(8,0);
		WMRA_Tall(2, qn, dqtemp, Twc1, Tn, Tna, Tnwc, T01, T12, T23, T34, T45, T56, T67);

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
			
		// Updating Physical Arm:
		if (arm==1){
			ddt=ddt+dt;
			if (ddt>=0.5 || i>=(n+1))
			{
				Matrix dqtemp(10,1);
				for (j=0; j<9; j++){
					dqtemp(j,0)=qn(j,0);	
				}
				dqtemp(9,0)=dg;

				
				//WMRA_ARM_Motion(2, 1, dqtemp, ddt, g);
				WMRA_ARM_Motion(2, 1, dqtemp, dt, g);
				//cout<<"dqtemp(9,0)"<<dqtemp(9,0)<<endl;
				//wait(.5);
				ddt=0;
			}
		}
			
		// Updating the old values with the new values for the next iteration:
		tt=tt+dt;
		fprintf(fid," tt is %g ",  tt);
		fprintf(fid," \n\n\n");

		// Stopping the simulation when the exit button is pressed:
		if (varscreenopn == 1){
			n=n+1;
		}
		else {
			break;
		}

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

	fclose(fid);
	
	//time(&endf);
	//endf=clock()/ CLOCKS_PER_SEC;
	endf=clock()*CLK_TCK/1000;
	cout<<"\nEnd is "<<endf<<"\n\n";
	//cout<<"\nElapsed time is "<<difftime(endf,startt)*1000<<"\n\n";
	cout<<"\nElapsed time is "<<(endf-startt)<<"\n\n";
		
		
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
		
		// Closing the Arm library and Matlab Graphics Animation and Virtual Reality Animation and Plots windows:
		cout << "\n Do you want to close all simulation windows and arm controls? \n Press '1' for Yes, or press '2' for No. \n\n ";
		cin >> choice8;
		if (choice8==1){
			Matrix temp(1,1);
			temp.Null(1,1);
			if (arm==1){
				WMRA_ARM_Motion(3, 0, temp, 0, g);
			}
		}
	}
	return 0;
}




/***********************************************/
/* Computation of a unit vector given a vector */
/***********************************************/
Matrix UnitVec(Matrix vec)
{
	float mag_Vec;
	Matrix u_vec(3,1);

	mag_Vec = sqrt((vec(0,0)*vec(0,0)) + (vec(1,0)*vec(1,0)) + (vec(2,0)*vec(2,0)));
	
	if (mag_Vec != 0.0)
	{
		u_vec(0,0) = vec(0,0)/mag_Vec;  u_vec(1,0) = vec(1,0)/mag_Vec;  u_vec(2,0) = vec(2,0)/mag_Vec;
		return u_vec;
	}
	else
	{
		//cout << "Unable to compute unit vector since the vector has zero magnitude" << endl;
		//exit(1);
		u_vec.Null(3,1);
		return u_vec;
	}
}

/**********************************************************/
/* Computation of the projection vector of a_vec on K_vec */
/**********************************************************/
Matrix CalcProjectionVector(Matrix K_vec, Matrix a_vec)
{
	float dot_v;
	Matrix unit_K_vec(3,1), a_proj(3,1);

	dot_v = K_vec(0,0)*a_vec(0,0) + K_vec(1,0)*a_vec(1,0) + K_vec(2,0)*a_vec(2,0);
	unit_K_vec = UnitVec(K_vec);
	
	a_proj(0,0) = dot_v*unit_K_vec(0,0);
	a_proj(1,0) = dot_v*unit_K_vec(1,0);
	a_proj(2,0) = dot_v*unit_K_vec(2,0);
		
	return a_proj;
}

/*********************************************************/
/*	Calculates magnitude of the vector					 */
/*********************************************************/
float vector_mag(Matrix a_vec)
{
	float mag = sqrt(a_vec(0,0)*a_vec(0,0) + a_vec(1,0)*a_vec(1,0) + a_vec(2,0)*a_vec(2,0));
	return mag;
}

