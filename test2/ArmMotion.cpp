/* This function communicates with the physical USF WMRA system with 9 DOF to get the encoder readings and send the commands to be executed.
The (.H) file and the (.DLL) file that contains the used functions should be in the directory containing this program.
config=0: Set the current encoder readings to zeros.
config=1: Read the encoder readings from the configuration txt file.
config=2: Change the configuration file to the initial values provided by (qo), then read the encoder readings from the configuration txt file.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed by: Punya Basnayaka %%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Feb 2010 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/

#include "matrix.h"  
#include "WCD.h"  
#include "ArmMotion.h"
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h> //if #include<windows.h> is included, there will be error messages
#include "control.h"
//#include "C:\Documents and Settings\pbasnaya\Desktop\My Research\WMRA2all\functiontest\functiontest\controlt2.h"
#include "Galil.h"
using namespace std;
using namespace math;
//using namespace System;

/*void sleep(long d) //sleep function takes in ms value
{ //sleep(1000) sleeps for 1 second
	clock_t start=clock();
	while(clock() - start < d);
}*/

Matrix WMRA_ARM_Motion(int ind, int config, Matrix qo, float dt, Galil &g)
{
	
	//extern long * ptr;
	extern float e2r1, e2r2, e2r3, e2r4, e2r5, e2r6, e2r7, e2r8, e2r9, e2d;
	long int aa, bb, cc, dd, ee, ff, gg, hh;
	//long int aa1, bb2, cc3, dd4, ee5, ff6, gg7, hh8;
	string a1, b1, c1,d1,e1,f1,g1;
	float  Kp, Kd, Ki;
	Matrix qn(10,1), qntemp(10,1);
	Matrix L(1,1);
	int j;
	long int decel=480000;
			
	// Reading the Wheelchair's constant dimentions, all dimentions are converted in millimeters:
	L=WMRA_WCD();
	
	//The initialization of the Arm library:
	if (ind==1)
	{
		// PID controller gains:
		/*Kp=6;
		Kd=64;
		Ki=0;*/
		Brushed_Motor ('A', 1,g);
		Brushed_Motor ('B', 1,g);
		Brushed_Motor ('C', 1,g);
		Brushed_Motor ('D', 1,g);
		//PID('H',Kp,Kd,Ki,g);

		//Conversion of encoder readings to radians: Note that the encoder readings are negative of the kinematic arrangements in the control code.
		//e2r1=PI/5250000;//a
		//e2r1=PI/6595000;//a
		//e2r2=PI/5100000;//b
		//e2r3=PI/1800000;//c
		//e2r4=PI/2300000;//d
		//e2r5=PI/2000000;//e
		//e2r6=-PI/2000000;//f
		//e2r7=PI/1500000; //g
		//e2r8=1; // Redwan: change this to forward motion when wheelchair controllers are installed (Only when reading the encoders).
		//e2r9=1; // Redwan: change this to rotation motion when wheelchair controllers are installed (Only when reading the encoders).
		//e2d =1/100000;//h

		e2r1=2*PI/13200000;//a
		e2r2=2*PI/13320000;//b
		e2r3=2*PI/4720000;//c
		e2r4=2*PI/4720000;//d
		e2r5=2*PI/3840000;//e
		e2r6=-(2*PI)/4000000;//f
		e2r7=2*PI/1440000; //g
		e2r8=1; // Redwan: change this to forward motion when wheelchair controllers are installed (Only when reading the encoders).
		e2r9=1; // Redwan: change this to rotation motion when wheelchair controllers are installed (Only when reading the encoders).
		e2d =1/100000;//h
	
		// The case when changing the configuration file to qo is required:

		if (config==2)
		{
			// Converting the commanded angles to encoder readings:
			//cout<<"qo(9,0)------- txt write =" <<qo(9,0)<<endl;
			qo(0,0)=qo(0,0)/e2r1;
			qo(1,0)=qo(1,0)/e2r2;
			qo(2,0)=qo(2,0)/e2r3;
			qo(3,0)=qo(3,0)/e2r4;
			//cout<<"qo(3,0) txt write =" <<qo(3,0)<<endl;
			qo(4,0)=qo(4,0)/e2r5;
			qo(5,0)=qo(5,0)/e2r6;
			qo(6,0)=qo(6,0)/e2r7;
			//cout<<"qo(6,0) txt write =" <<qo(6,0)<<endl;
			qo(7,0)=qo(7,0)/e2r8;
			qo(8,0)=qo(8,0)/e2r9;
			qo(9,0)=0;
			//cout<<"qo(9,0) txt write =" <<qo(9,0)<<endl;
			
			// Changing the configuration file to qo:
			FILE * fid;
			fid = fopen("configuration.txt","w");
			int j;
			for (j=0;j<7;j++){
				fprintf(fid," %10.0f ",qo(j,0));
			}
			fclose (fid);
			config=1;
		}
				
		Pos_track(g);
		
		//Decceleration_all (90000, g);
		g.command("DCA=480000");
		g.command("DCB=480000");
		g.command("DCC=480000");
		g.command("DCD=480000");
		g.command("DCE=480000");
		g.command("DCF=480000");
		g.command("DCG=480000");
		//Time_constant (0.6, g);
		
		long int qp[10];

		qp[0]=qo(0,0);
		qp[1]=qo(1,0);
		qp[2]=qo(2,0);
		qp[3]=qo(3,0);
		qp[4]=qo(4,0);
		qp[5]=qo(5,0);
		qp[6]=qo(6,0);
		qp[7]=qo(7,0);
		qp[8]=qo(8,0);
		qp[9]=qo(9,0);
		
		Define_Position ('A', qp[0], g);
		Define_Position ('B', qp[1], g);
		Define_Position ('C', qp[2], g);
		Define_Position ('D', qp[3], g);
		Define_Position ('E', qp[4], g);
		Define_Position ('F', qp[5], g);
		Define_Position ('G', qp[6], g);
		Define_Position ('H', qp[9], g);

		/*for(j=0;j<7;j++)
			cout<<qp[j]<<"\t";
		cout<<endl;*/

		aa=Tell_pos('A', g);
		bb=Tell_pos('B', g);
		cc=Tell_pos('C', g);
		dd=Tell_pos('D', g);
		ee=Tell_pos('E', g);
		ff=Tell_pos('F', g);
		gg=Tell_pos('G', g);
		hh=Tell_pos('H', g);

		long int qc[10];

		qc[0]=aa;
		qc[1]=bb;
		qc[2]=cc;
		qc[3]=dd;
		qc[4]=ee;
		qc[5]=ff;
		qc[6]=gg;
		qc[7]=0;
		qc[8]=0;
		qc[9]=hh;
		
		qn(0,0)=qc[0]*e2r1;
		qn(1,0)=qc[1]*e2r2;
		qn(2,0)=qc[2]*e2r3;
		qn(3,0)=qc[3]*e2r4;
		qn(4,0)=qc[4]*e2r5;
		qn(5,0)=qc[5]*e2r6;
		qn(6,0)=qc[6]*e2r7;
		qn(7,0)=qc[7]*e2r8;
		qn(8,0)=qc[8]*e2r9;
		qn(9,0)=qc[9]*e2d;// gripper
		qn(8,0)=qc[8]*e2r9;
		qn(9,0)=qc[9]*e2d;
	}

	// Closing the Arm library:

	else if (ind==3)
	{
		// Reading the current positions to be saved in the configuration file:
		
		// Reporting the function output to be zero (This value will not be used):
		qn.Null(10,1);
		//cout<<"qn" <<qn<<endl;
	}
	
	// Updating the Arm:
	else if (ind==2)
	{
		try
		{
			long int qin[10];

			aa=Tell_pos('A', g);
			bb=Tell_pos('B', g);
			cc=Tell_pos('C', g);
			dd=Tell_pos('D', g);
			ee=Tell_pos('E', g);
			ff=Tell_pos('F', g);
			gg=Tell_pos('G', g);
			hh=Tell_pos('H', g);
			
			long int qc[10];
			
			qc[0]=aa;
			qc[1]=bb;
			qc[2]=cc;
			qc[3]=dd;
			qc[4]=ee;
			qc[5]=ff;
			qc[6]=gg;
			qc[7]=0;
			qc[8]=0;
			qc[9]=hh;
			
			float qt[10];
			float qg[1];

			qo(0,0)=qo(0,0)/e2r1;
			qo(1,0)=qo(1,0)/e2r2;
			qo(2,0)=qo(2,0)/e2r3;
			qo(3,0)=qo(3,0)/e2r4;
			qo(4,0)=qo(4,0)/e2r5;
			qo(5,0)=qo(5,0)/e2r6;
			qo(6,0)=qo(6,0)/e2r7;
			qo(7,0)=qo(7,0)/e2r8;
			qo(8,0)=qo(8,0)/e2r9;

			 if(qo(9,0)==0)
			 {
				 qo(9,0) = 0;
				 qo(9,0) = qo(9,0) + qc[9];
			 }
			 else
			 {
				 if(qo(9,0) == 1)
				 {
					 qo(9,0) = -qo(9,0)*100000;
					 qo(9,0) = qo(9,0) + qc[9];
				 }
				 if (qo(9,0) == -1)
				 {
					 qo(9,0) = -qo(9,0)*100000;
					 qo(9,0) = qo(9,0) + qc[9];
				 }
			 }
			//cout<<"qg[1] =  "<<qo(9,0)<<endl;

			float dq[8];
			
			dq[0]=qo(0,0)-qc[0];
			dq[1]=qo(1,0)-qc[1];
			dq[2]=qo(2,0)-qc[2];
			dq[3]=qo(3,0)-qc[3];
			dq[4]=qo(4,0)-qc[4];
			dq[5]=qo(5,0)-qc[5];
			dq[6]=qo(6,0)-qc[6];
			dq[7]=qo(9,0)-qc[9];

			long int dq1[8];

			dq1[0]=abs(dq[0]);
			dq1[1]=abs(dq[1]);
			dq1[2]=abs(dq[2]);
			dq1[3]=abs(dq[3]);
			dq1[4]=abs(dq[4]);
			dq1[5]=abs(dq[5]);
			dq1[6]=abs(dq[6]);
			dq1[7]=abs(dq[7]);
			 
			//cout<<"dq[7] =  "<<dq[7]<<endl;

			long int dq01[8];
			long int qddo1[8];
							
			//if (abs(dq[0])>25000)
			//{
			//	dq01[0]=170000;
			//	qddo1[0]=75000;
			//}
			//else{
			//	dq01[0]=2*abs(dq[0])/0.3;
			//	qddo1[0]=2*abs(dq[0])/0.7;

			//}
			////cout<<"dq01[0] =  "<<dq01[0]<<endl;
			////cout<<"qddo1[0]= "<<qddo1[0]<<endl;
			/////////////////

			//if (abs(dq[1])>25000)
			//{
			//	dq01[1]=170000;
			//	qddo1[1]=75000;
			//}
			//else{
			//	dq01[1]=2*abs(dq[1])/0.3;
			//	qddo1[1]=2*abs(dq[1])/0.7;

			//}
			////cout<<"dq01[1] =  "<<dq01[1]<<endl;
			////cout<<"qddo1[1]= "<<qddo1[1]<<endl;
			/////////////////////////

			//if (abs(dq[2])>25000)
			//{
			//	dq01[2]=170000;
			//	qddo1[2]=75000;
			//}
			//else{
			//	dq01[2]=2*abs(dq[2])/0.3;
			//	qddo1[2]=2*abs(dq[2])/0.7;

			//}
			////cout<<"dq01[2] =  "<<dq01[2]<<endl;
			////cout<<"qddo1[2]= "<<qddo1[2]<<endl;
			/////////////////////
			//if (abs(dq[3])>25000)
			//{
			//	dq01[3]=170000;
			//	qddo1[3]=75000;
			//}
			//else{
			//	dq01[3]=2*abs(dq[3])/0.3;
			//	qddo1[3]=2*abs(dq[3])/0.7;

			//}
			////cout<<"dq01[3] =  "<<dq01[3]<<endl;
			////cout<<"qddo1[3]= "<<qddo1[3]<<endl;
			///////////////////////

			//if (abs(dq[4])>25000)
			//{
			//	dq01[4]=170000;
			//	qddo1[4]=75000;
			//}
			//else{
			//	dq01[4]=2*abs(dq[4])/0.3;
			//	qddo1[4]=2*abs(dq[4])/0.7;

			//}
			////cout<<"dq01[4] =  "<<dq01[4]<<endl;
			////cout<<"qddo1[4]= "<<qddo1[4]<<endl;
			////////////////////
			//if (abs(dq[5])>25000)
			//{
			//	dq01[5]=170000;
			//	qddo1[5]=75000;
			//}
			//else{
			//	dq01[5]=2*abs(dq[5])/0.3;
			//	qddo1[5]=2*abs(dq[5])/0.7;

			//}
			////cout<<"dq01[5] =  "<<dq01[5]<<endl;
			////cout<<"qddo1[5]= "<<qddo1[5]<<endl;
			////////////////////
			//if (abs(dq[6])>25000)
			//{
			//	dq01[6]=170000;
			//	qddo1[6]=75000;
			//}
			//else{
			//	dq01[6]=2*abs(dq[6])/0.3;;
			//	qddo1[6]=2*abs(dq[6])/0.7;

			//}
			//cout<<"dq01[6] =  "<<dq01[6]<<endl;
			//cout<<"qddo1[6]= "<<qddo1[6]<<endl;
			
			FILE * fid1;
			fid1 = fopen("arm1.txt","a");
			
			for (j=0; j<7; j++)
				fprintf(fid1,"%d\n", dq1[j]);

			//fclose(fid1);
			/*fprintf(fid1," \n\n\n");
			fprintf(fid1," qc[3] is %4.2f ",  qc[3]);
			fprintf(fid1," \n\n\n");*/

			Pos_track(g);

			//Set Velocity
			for (j=0;j<8;j++)
			{
				dq01[j]=dq1[j]/dt;
				if (dq01[j]>300000) //800000
					dq01[j] = 300000; //800000
			}
			
			//dq01[7]=dq1[7]/2;
			//dq01[7] = 800000;
			
			//Set Acceleration
			for (j=0;j<8;j++)
				qddo1[j] = 160000; //160000

			aa=Tell_pos('A', g);
			bb=Tell_pos('B', g);
			cc=Tell_pos('C', g);
			dd=Tell_pos('D', g);
			ee=Tell_pos('E', g);
			ff=Tell_pos('F', g);
			gg=Tell_pos('G', g);
			hh=Tell_pos('H', g);
			cout << "\nBefore Tell_pos" << endl;
			cout<<"aa is = "<<aa<<endl;
			cout<<"bb is = "<<bb<<endl;
			cout<<"cc is = "<<cc<<endl;
			cout<<"dd is = "<<dd<<endl;
			cout<<"ee is = "<<ee<<endl;
			cout<<"ff is = "<<ff<<endl;
			cout<<"gg is = "<<gg<<endl;
			cout<<"hh is = "<<hh<<endl;
			cout<<"qo(0,0) is = "<<qo(0,0)<<endl;
			cout<<"qo(1,0) is = "<<qo(1,0)<<endl;
			cout<<"qo(2,0) is = "<<qo(2,0)<<endl;
			cout<<"qo(3,0) is = "<<qo(3,0)<<endl;
			cout<<"qo(4,0) is = "<<qo(4,0)<<endl;
			cout<<"qo(5,0) is = "<<qo(5,0)<<endl;
			cout<<"qo(6,0) is = "<<qo(6,0)<<endl;
			cout<<"qo(7,0) is = "<<qo(7,0)<<endl;

			
			Position_control('a', qo(0,0), dq01[0], qddo1[0], g);
			Position_control('b', qo(1,0), dq01[1], qddo1[1], g);
			Position_control('c', qo(2,0), dq01[2], qddo1[2], g);			
			Position_control('d', qo(3,0), dq01[3], qddo1[3], g);
			Position_control('e', qo(4,0), dq01[4], qddo1[4], g);			
			Position_control('f', qo(5,0), dq01[5], qddo1[5], g);
			Position_control('g', qo(6,0), dq01[6], qddo1[6], g);	
			Position_control('h', qo(9,0), dq01[7], qddo1[7], g);	

			aa=Tell_pos('A', g);
			bb=Tell_pos('B', g);
			cc=Tell_pos('C', g);
			dd=Tell_pos('D', g);
			ee=Tell_pos('E', g);
			ff=Tell_pos('F', g);
			gg=Tell_pos('G', g);
			hh=Tell_pos('H', g);

	while(abs(aa-qo(0,0)) > 10000 || abs(bb-qo(1,0)) > 10000  || abs(cc-qo(2,0)) > 10000  || abs(dd-qo(3,0)) > 10000 || abs(ee-qo(4,0)) > 10000  || abs(ff-qo(5,0)) > 10000 || abs(gg-qo(6,0)) > 10000 || abs(hh-qo(9,0)) > 10000)
	{
			aa=Tell_pos('A', g);
			bb=Tell_pos('B', g);
			cc=Tell_pos('C', g);
			dd=Tell_pos('D', g);
			ee=Tell_pos('E', g);
			ff=Tell_pos('F', g);
			gg=Tell_pos('G', g);
			hh=Tell_pos('H', g);
	}
			cout << "\nAfter Tell_pos" << endl;
			cout<<"aa is = "<<aa<<endl;
			cout<<"bb is = "<<bb<<endl;
			cout<<"cc is = "<<cc<<endl;
			cout<<"dd is = "<<dd<<endl;
			cout<<"ee is = "<<ee<<endl;
			cout<<"ff is = "<<ff<<endl;
			cout<<"gg is = "<<gg<<endl;
			cout<<"hh is = "<<hh<<endl;

			qc[0]=aa;
			qc[1]=bb;
			qc[2]=cc;
			qc[3]=dd;
			qc[4]=ee;
			qc[5]=ff;
			qc[6]=gg;
			qc[7]=0;
			//cout<<"qc[7] = "<<qc[7]<<endl;
			//	qc[8]=0;
			//cout<<"qc[6] = "<<qc[6]<<endl;
			//	qc[9]=hh;
			//cout<<"qc[7] = "<<qc[7]<<endl;
			//ptr=qc;	
			qn(0,0)=qc[0]*e2r1;
			qn(1,0)=qc[1]*e2r2;
			qn(2,0)=qc[2]*e2r3;
			qn(3,0)=qc[3]*e2r4;
			qn(4,0)=qc[4]*e2r5;
			qn(5,0)=qc[5]*e2r6;
			qn(6,0)=qc[6]*e2r7;
			qn(7,0)=qc[7]*e2r8;
		//	qn(8,0)=qc[8]*e2r9;
		//	qn(9,0)=qc[9]*e2d;
			//cout <<"qn"<<qn<<endl;

			/*fprintf(fid1," \n\n\n");
			fprintf(fid1," qn(3,0) is %4.2f ", qn(3,0));
			fprintf(fid1," \n\n\n");*/

		//	g.command("DP qn(0,0),qn(1,0),qn(2,0),qn(3,0),qn(4,0),qn(5,0),qn(6,0)");
			//sleep(20000);

			fclose(fid1);

		}
		catch(string e)
		{
			cout << e;
		}
	}

	return qn;
}