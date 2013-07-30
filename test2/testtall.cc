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
	
	FILE * fid;
	fid = fopen("testtall.txt","a");
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
	fprintf(fid," T67 is: \n\n");
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
	dt=0.8;
	
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
	Tt=WMRA_traj(trajf,Ti,Td,8);


		Matrix qn(1,1), Tn(4,4), Tna(4,4), Tnwc(4,4);
		Matrix q1(1,1), T1(4,4), Ta1(4,4), Twc1(4,4);
		Matrix Joa(6,7), Jowc(1,1), Jowctemp(1,1), Joatemp(1,1);
		float detJoa, detJo, phi;
		q1=qo;
				T1=To;
				Ta1=Toa;
				Twc1=Towc;

			// Calculating the 6X7 Jacobian of the arm in frame 0:
			WMRA_J07(T01, T12, T23, T34, T45, T56, T67, Joa, detJoa);
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
			for (j=0; j<4; j++){
				for (k=0; k<4; k++){
					Tttemp(j,k)=Tt[1][j][k];	
				}
			}	
			dx=WMRA_delta(T1, Tttemp);
			
			fprintf(fid," dx is: \n\n");
			for (j=0;j<dx.RowNo();j++){
				fprintf(fid," %4.2f ",  dx(j,0));
			}
			fprintf(fid," \n\n\n");
			
			fprintf(fid," dq is: \n\n");
			for (j=0;j<dq.RowNo();j++){
				fprintf(fid," %4.2f ",  dq(j,0));
			}

				Matrix dq1(9,1);
				dq1=WMRA_Opt(optim, JLA, JLO, Jo, detJo, dq, dx, dt, q1);
				dq1;
			
			fprintf(fid," dq1 is: \n\n");
			for (j=0;j<dq1.RowNo();j++){
				fprintf(fid," %4.2f ",  dq1(j,0));
			}
			fprintf(fid," \n\n\n");
			
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
			fprintf(fid," T01 is: \n\n");
	for (j=0;j<T01.RowNo();j++){
		for (k=0;k<T01.ColNo();k++){
			fprintf(fid," %4.2f ",  T01(j,k));
		}
		fprintf(fid," \n");
	}
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
			fclose(fid);
			return 0;
}
				