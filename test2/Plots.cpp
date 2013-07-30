/* This function plots different animation variables for USF WMRA with 9 DOF.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%% COPY RIGHTS RESERVED %%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Developed By: Redwan M. Alqasemi %%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% April 2007 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Function Declaration:*/
#include "matrix.h" 
#include "Plots.h" 
#include "T2rpy.h"
#include <math.h>
using namespace std;
using namespace math;
#define PI 3.14159265
#define r2d 180/PI //Conversions from Radians to Degrees.

void WMRA_Plots(int st, Matrix L, float dt, int i, float tt, Matrix qn, Matrix dq, Matrix Tn, Matrix Tnwc, float detjoa, float detjo){

// Calling global variables:
	extern Matrix timep, q1, q2, q3, q4, q5, q6, q7, qll, qrr, qd1, qd2, qd3, qd4, qd5, qd6, qd7, qdl, qdr, x, y, z, roll, pitch, yaw, xc, yc, zc, rollc, pitchc, yawc, detJoap, detJop;

	timep.SetSize(i+2,1); q1.SetSize(i+2,1); q2.SetSize(i+2,1); q3.SetSize(i+2,1); q4.SetSize(i+2,1); q5.SetSize(i+2,1); q6.SetSize(i+2,1); q7.SetSize(i+2,1); qll.SetSize(i+2,1); qrr.SetSize(i+2,1); qd1.SetSize(i+2,1); qd2.SetSize(i+2,1); qd3.SetSize(i+2,1); qd4.SetSize(i+2,1); qd5.SetSize(i+2,1); qd6.SetSize(i+2,1); qd7.SetSize(i+2,1); qdl.SetSize(i+2,1); qdr.SetSize(i+2,1); x.SetSize(i+2,1); y.SetSize(i+2,1); z.SetSize(i+2,1); roll.SetSize(i+2,1); pitch.SetSize(i+2,1); yaw.SetSize(i+2,1); xc.SetSize(i+2,1); yc.SetSize(i+2,1); zc.SetSize(i+2,1); rollc.SetSize(i+2,1); pitchc.SetSize(i+2,1); yawc.SetSize(i+2,1); detJoap.SetSize(i+2,1); detJop.SetSize(i+2,1);


// Data collection at every iteration:
	if (st==1){

		// Generating a time vector for plotting:
		timep(i,0)=tt;
		// Joint Angles:
		q1(i,0)=qn(0,0)*r2d;
		q2(i,0)=qn(1,0)*r2d;
		q3(i,0)=qn(2,0)*r2d;
		q4(i,0)=qn(3,0)*r2d;
		q5(i,0)=qn(4,0)*r2d;
		q6(i,0)=qn(5,0)*r2d;
		q7(i,0)=qn(6,0)*r2d;
		qll(i,0)=qn(7,0)-L(0,0)*qn(8,0)/2;
		qrr(i,0)=qn(7,0)+L(0,0)*qn(8,0)/2;

		// Joint Velocities:
		qd1(i,0)=r2d*dq(0,0)/dt;
		qd2(i,0)=r2d*dq(1,0)/dt;
		qd3(i,0)=r2d*dq(2,0)/dt;
		qd4(i,0)=r2d*dq(3,0)/dt;
		qd5(i,0)=r2d*dq(4,0)/dt;
		qd6(i,0)=r2d*dq(5,0)/dt;
		qd7(i,0)=r2d*dq(6,0)/dt;
		qdl(i,0)=(dq(7,0)-L(0,0)*dq(8,0)/2)/dt;
		qdr(i,0)=(dq(7,0)+L(0,0)*dq(8,0)/2)/dt;
		
		// Hand Position and Orientation:
		x(i,0)=Tn(0,3);
		y(i,0)=Tn(1,3);
		z(i,0)=Tn(2,3);
		Matrix or(1,1);
		or.Null(1,1);
		or=WMRA_T2rpy(Tn);
		roll(i,0)=or(0,0)*r2d;
		pitch(i,0)=or(1,0)*r2d;
		yaw(i,0)=or(2,0)*r2d;
		
		// Arm Base Position and Orientation on the Wheelchair:
		xc(i,0)=Tnwc(0,3);
		yc(i,0)=Tnwc(1,3);
		zc(i,0)=Tnwc(2,3);
		Matrix orc(1,1);
		orc.Null(1,1);
		orc=WMRA_T2rpy(Tnwc);
		rollc(i,0)=orc(0,0)*r2d;
		pitchc(i,0)=orc(1,0)*r2d;
		yawc(i,0)=orc(2,0)*r2d;
		
		// Manipulability Measure:
		detJoap(i,0)=detjoa;
		detJop(i,0)=detjo;
	}
		// Plotting the data in graphas:
	else {
		int j;
		
		FILE * fid;
		fid = fopen("Joint Angular Velocities.cvs","w");
		fprintf(fid," time \t qd1 \t qd2 \t qd3 \t qd4 \t qd5 \t qd6 \t qd7 \n");
		for (j=0; j<timep.RowNo(); j++){
			fprintf(fid," %g \t %g \t %g \t %g \t %g \t %g \t %g \t %g \n", timep(j,0) , qd1(j,0) , qd2(j,0) , qd3(j,0) , qd4(j,0) , qd5(j,0) , qd6(j,0) , qd7(j,0) );
		}
		fclose(fid);

		FILE * fid1;
		fid1 = fopen("Wheels Track Velocities.cvs","w");
		fprintf(fid1," time \t qdl \t qdr \n");
		for (j=0; j<timep.RowNo(); j++){
			fprintf(fid1," %g \t %g \t %g \n", timep(j,0) , qdl(j,0) , qdr(j,0));
		}
		fclose(fid1);

		FILE * fid2;
		fid2 = fopen("Joint Angular Displacements.cvs","w");
		fprintf(fid2," time \t q1 \t q2 \t q3 \t q4 \t q5 \t q6 \t q7 \n");
		for (j=0; j<timep.RowNo(); j++){
			fprintf(fid2," %g \t %g \t %g \t %g \t %g \t %g \t %g \t %g \n", timep(j,0) , q1(j,0) , q2(j,0) , q3(j,0) , q4(j,0) , q5(j,0) , q6(j,0) , q7(j,0) );
		}
		fclose(fid2);

		FILE * fid3;
		fid3 = fopen("Wheels Track Distances.cvs","w");
		fprintf(fid3," time \t qll \t qrr \n");
		for (j=0; j<timep.RowNo(); j++){
			fprintf(fid3," %g \t %g \t %g \n", timep(j,0) , qll(j,0) , qrr(j,0));
		}
		fclose(fid3);

		FILE * fid4;
		fid4 = fopen("Hand Position.cvs","w");
		fprintf(fid4," time \t x \t y \t z \n");
		for (j=0; j<timep.RowNo(); j++){
			fprintf(fid4," %g \t %g \t %g \t %g \n", timep(j,0) , x(j,0) , y(j,0), z(j,0));
		}
		fclose(fid4);

		FILE * fid5;
		fid5 = fopen("Hand Orientation.cvs","w");
		fprintf(fid5," time \t roll \t pitch \t yaw \n");
		for (j=0; j<timep.RowNo(); j++){
			fprintf(fid5," %g \t %g \t %g \t %g \n", timep(j,0) , roll(j,0) , pitch(j,0), yaw(j,0));
		}
		fclose(fid5);

		FILE * fid6;
		fid6 = fopen("Arm Base Position.cvs","w");
		fprintf(fid6," time \t xc \t yc \t zc \n");
		for (j=0; j<timep.RowNo(); j++){
			fprintf(fid6," %g \t %g \t %g \t %g \n", timep(j,0) , xc(j,0) , yc(j,0), zc(j,0));
		}
		fclose(fid6);

		FILE * fid7;
		fid7 = fopen("Arm Orientation.cvs","w");
		fprintf(fid7," time \t rollc \t pitchc \t yawc \n");
		for (j=0; j<timep.RowNo(); j++){
			fprintf(fid7," %g \t %g \t %g \t %g \n", timep(j,0) , rollc(j,0) , pitchc(j,0), yawc(j,0));
		}
		fclose(fid7);

		FILE * fid8;
		fid8 = fopen("Manipulability Measure.cvs","w");
		fprintf(fid8," time \t detJoa \t detJo \n");
		for (j=0; j<timep.RowNo(); j++){
			fprintf(fid8," %g \t %g \t %g \n", timep(j,0) , detJoap(j,0) , detJop(j,0));
		}
		fclose(fid8);
	}
}