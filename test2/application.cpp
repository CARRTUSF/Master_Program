
//#include "Galil.h"
//static Galil g("192.168.1.104");
//int main()
//{
//	int stop;
//}
#include "Galil.h"
#include <iostream>
#include <fstream>
#include <Afxwin.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "maintenance.h"
#include "controlMotor.h"
//#include "match.h"
//#include "omniControl.h"
#include "autoControl.h"

//#include "camshift.h"
//#include <cv.h>
//#include <cxcore.h>
//#include <highgui.h>
#include "conio.h"
#include "math.h"

// Sensable's includes 
#include <HL/hl.h>
#include <HLU/hlu.h>
#include <HD/hd.h>
#include <HDU/hdu.h>
//#include <HDU/hduError.h>
#include <HDU/hduVector.h>

#define PI 3.14159265


extern Galil g("192.168.1.22");

int traj_exe=0;
int wmraCont = 1; //return value from WMRA thread
float x_coord, y_coord, z_coord, a_ang, b_ang, g_ang;


typedef struct OmniStruct {
	double noap[12];
	int buttDown;				//Omni button number
	bool gripperOpen;			//Open = 1, Closed = 0
};
OmniStruct gOmni;

double wmraCtrl[10] = {0, 0, 0, 0, 0, 0, 0, 1, 135, 135};

HDSchedulerHandle gCallbackHandle = 0;
HHD hHD;
HLboolean buttDown1, buttDown2;
HDErrorInfo error;

void autonomousControl();
int InitHapticDevice();
void omniLoop();
int omniControl();
void autonomousControl();
HDCallbackCode HDCALLBACK Controller(void *pUserData);

int wmraEnd = 1; //end flag for WMRA control program
//extern float v;

char *tempChar; //temporary char pointer passed to thread

using namespace std;

FILE * fid_trans;
FILE * fidTrain;

int count_skip=0;
bool ButtPress_FirstTime=false;	//false-button not pressed
bool bool_grip_open=false, bool_grip_close=false;
bool bool_grip_frame=false;
bool gGripperFrame=false;
bool gscaledrot=false;
bool gFwdKin=false;
bool takeTa1 = false;
bool saveFileFlag = false;
bool autonomousFlag = false;
bool wmraThreadRet = true;

static HDdouble nx1, ox1, ax1, px1=0.0;
static HDdouble ny1, oy1, ay1, py1=0.0;
static HDdouble nz1, oz1, az1, pz1=0.0;
static HDdouble ni1, oi1, ai1, pi1;
static HDdouble nx2, ox2, ax2, px2=0.0;
static HDdouble ny2, oy2, ay2, py2=0.0;
static HDdouble nz2, oz2, az2, pz2=0.0;
static HDdouble ni2, oi2, ai2, pi2;

//UINT omniThread (LPVOID pParam)
//{
//	//thread for moving WMRA
//	wmraEnd = omniControl(g); //calls main WMRA program
//	//AfxEndThread(0);
//	return 0;
//}
UINT autoThread (LPVOID pParam)
{
	//thread for moving WMRA
	wmraEnd = autoControl(g); //calls main WMRA program
	//AfxEndThread(0);
	return 0;
}

int main()
{
	g.command("SH"); //turn on motors
	int menuChoice;
	int mainFlag = 1;

	while(mainFlag){
		cout << "\n\tMAIN MENU\n" << endl;
		cout << "Enter the number of a choice below" << endl;
		cout << "1: Quick Fix" << endl;
		cout << "2: Manual Control" << endl;
		cout << "3: Omni Control (Not Avil.)" << endl;
		cout << "4: Autonomous Control" << endl;
		cout << "0: Exit\n" << endl;
		cin >> menuChoice;
		switch(menuChoice)
		{
		case 0: mainFlag = 0; break;
		case 1: adjustVelocity(g); break;
		case 2: maintenance(g); break;
		case 3: break; //omniControl(); break;
		case 4: autonomousControl(); break;
		}
	}
	return 0;
}

void autonomousControl()
{
	int choice1 = 1;
	//float x_coord, y_coord, z_coord, a_ang, b_ang, g_ang;
	//Sleep(2000);

	//<------------------------ START WMRA THREAD ------------------------>
	cout << "Starting WMRA thread..." << endl;
	AfxBeginThread (autoThread, tempChar);
	Sleep(5000);
	do
	{
		cout << "Enter the x, y and z coordinates and a, b and g angles of the target" << endl;
		
		cout << "X Coordinate: ";
		cin >> x_coord;
		
		cout << "Y Coordinate: ";
		cin >> y_coord;
		
		cout << "Z Coordinate: ";
		cin >> z_coord;

		cout << "A Angle: ";
		cin >> a_ang;
		
		cout << "B Angle: ";
		cin >> b_ang;
		
		cout << "G Angle: ";
		cin >> g_ang;

		cout << "Angles in degrees are: " << a_ang << b_ang << g_ang <<endl;

		a_ang=a_ang*PI/180;
		b_ang=b_ang*PI/180;
		g_ang=g_ang*PI/180;

		cout << "Angles in radians are: " << a_ang << b_ang << g_ang <<endl;

		traj_exe=1;

		while(traj_exe)
			;	//pause this loop when trajectory is being executed

		cout << "\n\nDo you want to continue? Enter '1' for Yes and '2' for No" << endl;
		cin >> choice1;

	}while(choice1==1);

	//choice is 2, so we must end the WMRA thread
	wmraCont=0;

	//wait for the wmra thread to finish
	while (wmraThreadRet != 0)
	{
		//loop until WMRA thread is finished
	}
}
//
//int omniControl(){
//
//	fid_trans = fopen("Trans.txt","w");
//
//	//v = 25; //set initial WMRA arm speed
//
//	//<------------------------ START WMRA CODE ------------------------>
//	cout << "Initializing WMRA..." << endl;
//	AfxBeginThread (omniThread, tempChar);
//	cout << "WMRA initialized..." << endl;
//
//	if (InitHapticDevice())
//		cout << "\nHaptic Device Intitialized\n" << endl;
//	else
//		exit(1);
//
//	/* Schedule the haptic callback function for continuously monitoring the
//	button state and rendering the anchored spring force. */
//	gCallbackHandle = hdScheduleAsynchronous(Controller, 0, HD_MAX_SCHEDULER_PRIORITY);
//
//	/* Start the haptic rendering loop. */
//	hdSetSchedulerRate( 500 /* Hz */); 
//	hdStartScheduler();
//	if (HD_DEVICE_ERROR(error = hdGetError()))
//	{
//		//hduPrintError(stderr, &error, "Failed to start scheduler");
//		fprintf(stderr, "\nPress any key to quit.\n");
//		_getch();
//		return -1;
//	}
//
//	cout << endl << "Scheduler Started" << endl;
//
//	/* Start the main application loop. */
//	omniLoop();
//	
//	while (wmraEnd != 0) //wait for wmra thread to finish
//	{
//		//loop until WMRA thread is finished
//	}
//
//	/* Cleanup by stopping the haptics loop, unscheduling the asynchronous
//		callback, disabling the device. */
//	hdStopScheduler();
//	hdUnschedule(gCallbackHandle);
//	hdDisableDevice(hHD);
//	fclose(fid_trans);
//	return 0;
//}
//
///******************************************************************************
// Main omni loop.  
// Detects keypresses
//******************************************************************************/
//void omniLoop()
//{
//    printf("\nMain Omni Loop Running\n\n");
//	int keypress;
//
//	char * fileName = new char [50];
//	int fileNum = 0;
//	char * fileLet = new char [50];
//	bool omniFlag = 1;
//
//
//	while (omniFlag)
//    {
//		keypress = ' ';
//		// Check for keyboard input.
//		if (_kbhit())
//			keypress = _getch();
//		
//		keypress = toupper(keypress);
//
//		if (keypress == 'Q')
//		{
//            wmraCtrl[7] = 0; //stop WMRA arm motion
//			omniFlag = 0;
//			return;
//		}
//		else if (keypress == 'C')
//		{
//			bool_grip_close = !bool_grip_close;
//			if (bool_grip_close)
//			{
//				wmraCtrl[6] = -1;
//				cout << "gripper closing" << endl;
//			}
//			else
//			{
//				wmraCtrl[6] = 0;
//				cout << "gripper closing stopped" << endl;
//			}
//		}
//		else if (keypress == 'O')
//		{
//			bool_grip_open = !bool_grip_open;
//			if (bool_grip_open)
//			{
//				wmraCtrl[6] = 1;
//				cout << "gripper opening" << endl;
//			}
//			else
//			{
//				wmraCtrl[6] = 0;
//				cout << "gripper opening stopped" << endl;
//			}
//		}
//		else if (keypress == 'G')
//		{
//			bool_grip_frame = !bool_grip_frame;
//			if (bool_grip_frame)
//			{
//				gGripperFrame=true;
//				cout << "gripper frame active" << endl;
//			}
//			else
//			{
//				gGripperFrame=false;
//				cout << "base frame active" << endl;
//			}
//		}
//		else if (keypress == 'S')
//		{
//			if(gscaledrot)
//				fclose(fidTrain);
//			else
//			{
//
//			fileNum++;
//			itoa(fileNum,fileLet,10);
//			strcpy(fileName,"HmmTrainData");
//			strcat(fileName,fileLet);
//			strcat(fileName,".txt");
//			fidTrain = fopen(fileName,"w");
//			gscaledrot = !gscaledrot;
//
//			}
//		}
//		else if (keypress == 'R')
//		{
//			takeTa1 = true;
//		}
//		else if (keypress == '`')
//		{
//			cout << "Filea Saved" << endl;
//			saveFileFlag = true;
//		}
//		else if (keypress == 'A')
//		{
//			autonomousFlag = true;
//		}
//	}
//}
//
///******************************************************************************
// Initialize Haptic Device
// ******************************************************************************/
//int InitHapticDevice()
//{
//	// Initialize the default haptic device.
//	hHD = hdInitDevice(HD_DEFAULT_DEVICE);
//	if (HD_DEVICE_ERROR(error = hdGetError()))
//	{
//		cout << "E    R    R    O    R" << endl;
//		//hduPrintError(stderr, &error, "Failed to initialize haptic device");
//		fprintf(stderr, "\nPress any key to quit.\n");
//		_getch();
//		return -1;
//	}
//	
//	return 1;
//}
//
///******************************************************************************
// * Scheduler callback for reading the robot position.
// *****************************************************************************/
//HDCallbackCode HDCALLBACK Controller(void *pUserData)
//{
//	//Sleep(1);
//	
//	HDErrorInfo error;
//	
//	HDdouble noap[16];
//		
//	double abg_set[3];
//	double temp_x=0.0, temp_y=0.0, temp_z=0.0, temp_wx=0.0, temp_wy=0.0, temp_wz=0.0;
//
//	HDint nCurrentButtons;
//
//	hdBeginFrame(hHD);
//
//	hdGetIntegerv(HD_CURRENT_BUTTONS, &nCurrentButtons);
//
//	if ((nCurrentButtons & HD_DEVICE_BUTTON_1) != 0)
//		gOmni.buttDown = 1;
//	else if ((nCurrentButtons & HD_DEVICE_BUTTON_2) != 0)
//		gOmni.buttDown = 2;
//	else
//		gOmni.buttDown = -1;
// 
//	//fprintf(fid_trans, "%f %f %f %f \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n\n", noap[0], noap[4], noap[8], noap[12], noap[1], noap[5], noap[9], noap[13], noap[2], noap[6], noap[10], noap[14], noap[3], noap[7], noap[11], noap[15]);
//	//printf("%f %f %f\n", gOmni.noap[9], gOmni.noap[10], gOmni.noap[11]);
//
//	if (gOmni.buttDown == 2)
//	{
//		if (ButtPress_FirstTime)
//		{
//			ButtPress_FirstTime=false;
//
//			hdGetDoublev(HD_CURRENT_TRANSFORM, noap);
//			nx1 = noap[0];	ox1 = noap[4];	ax1 = noap[8];	px1 = noap[12];
//			ny1 = noap[1];	oy1 = noap[5];	ay1 = noap[9];	py1 = noap[13];
//			nz1 = noap[2];	oz1 = noap[6];	az1 = noap[10];	pz1 = noap[14];
//			ni1 = noap[3];	oi1 = noap[7];	ai1 = noap[11];	pi1 = noap[15];
//		}
//
//		count_skip++;
//
//		if (count_skip==5)
//		{
//			count_skip=0;
//
//			hdGetDoublev(HD_CURRENT_TRANSFORM, noap);
//			nx2 = noap[0]; ox2 = noap[4]; ax2 = noap[8];	px2 = noap[12];
//			ny2 = noap[1]; oy2 = noap[5]; ay2 = noap[9];	py2 = noap[13];
//			nz2 = noap[2]; oz2 = noap[6]; az2 = noap[10];	pz2 = noap[14];
//			ni2 = noap[3]; oi2 = noap[7]; ai2 = noap[11];	pi2 = noap[15];
//
//			temp_x = (px2 - px1);
//			temp_y = (py2 - py1);
//			temp_z = (pz2 - pz1);
//
//			//fprintf(fid_trans, "%f %f %f %f %f %f %f %f %f\n", px2, py2, pz2, px1, py1, pz1, wmraCtrl[0], wmraCtrl[1], wmraCtrl[2]);
//
//			temp_wx = 0.5*(oy1*oz2-oz1*oy2 + ny1*nz2-nz1*ny2 + ay1*az2-az1*ay2);
//			temp_wy = 0.5*(oz1*ox2-ox1*oz2 + nz1*nx2-nx1*nz2 + az1*ax2-ax1*az2);
// 			temp_wz = 0.5*(ox1*oy2-oy1*ox2 + nx1*ny2-ny1*nx2 + ax1*ay2-ay1*ax2);
//
//			fprintf(fid_trans,"%f %f %f %f %f %f \n", temp_x, temp_y, temp_z, temp_wx*180/PI, temp_wy*180/PI, temp_wz*180/PI);
//
//			nx1 = nx2;	ox1 = ox2;	ax1 = ax2;	px1 = px2;
//			ny1 = ny2;	oy1 = oy2;	ay1 = ay2;	py1 = py2;
//			nz1 = nz2;	oz1 = oz2;	az1 = az2;	pz1 = pz2;
//			ni1 = ni2;	oi1 = oi2;	ai1 = ai2;	pi1 = pi2;
//		}		
//	}
//	else  // gOmni.buttDown != 2
//	{
//		ButtPress_FirstTime=true;	//true-button pressed first time
//
//		temp_x = 0;
//		temp_y = 0;
//		temp_z = 0;
//		temp_wx = 0;
//		temp_wy = 0;
//		temp_wz = 0;
//
//		//fprintf(fid_trans, "%f %f %f %f %f %f %f %f %f\n", px2, py2, pz2, px1, py1, pz1, wmraCtrl[0], wmraCtrl[1], wmraCtrl[2]);
//	}
//
//	wmraCtrl[0] = -temp_z;
//	wmraCtrl[1] = -temp_x;
//	wmraCtrl[2] = temp_y;
//	wmraCtrl[3] = -temp_wz;
//	wmraCtrl[4] = -temp_wx;
//	wmraCtrl[5] = temp_wy;
//
//	hdEndFrame(hHD);
//
//	/* Check if an error occurred while attempting to render the force */
//    if (HD_DEVICE_ERROR(error = hdGetError()))
//    {
//        /*if (hduIsSchedulerError(&error))
//		{
//            return HD_CALLBACK_DONE;
//        }*/
//    }
//
//	return HD_CALLBACK_CONTINUE;
//}