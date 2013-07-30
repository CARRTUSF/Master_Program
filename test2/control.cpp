/*
 * control.cpp
 *
 *  Created on: May 19, 2009
 *      Author:Punya Basnayaka
 *
 *      This control.cpp file includes the source code that
 *      is prototyped in the control.h file. These files
 *      work together to provide functions that will create
 *      commands for the object 'Galil g' when a .cpp file
 *      includes this control.h library.
 */

#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <ctype.h>
#include "control.h"
#include "Galil.h"

using namespace std;

//==========================================================
// Accelerate() takes in a specified motor (A-H) and a
// specified value of the acceleration. Value must be an
// unsigned value between 1024 and 67107840 as specified by
// the control documentation. We include basic error
// checking in case the user specifies a value outside the
// working range of the controller. Accelerate() assumes a
// Galil object of name 'g' has been created. We will
// construct 'Galil g' in the control.cpp file.
//==========================================================
//////////////////////////////////////////////////////////////////////
/////////////////MOTION PROFILING FUNCTIONS///////////////////////////
/////////////////==========================///////////////////////////



void Decceleration_all (long int dvalue, Galil &g)
{
	string svalue, command = "DC";
	
	if ((dvalue < 1024) || (dvalue > 67107840))
	{
		cerr << "The decceleration value is outside the range" << endl;
	}
		
	command = command + "*=";
		
	ostringstream sval;
	sval << dvalue;
	svalue = sval.str();

	command = command + svalue;
	
	//cout << "The string command contains: " << command << endl;

	g.command(command);
}


//////Functions to run echach axix in position tracking mode///////
//////======================================================///////


//////This function will place the controller in the position tracking mode 
//and will allow user to issue absolute position on the fly////////////////

void Pos_track (Galil &g)
	{
	
	string svalue, command = "PT";
			
	command = command + "*=1";
	
	//cout << "The string command contains: " << command << endl;

	//g.command(command);
	g.command("PTA=1");
	g.command("PTB=1");
	g.command("PTC=1");
	g.command("PTD=1");
	g.command("PTE=1");
	g.command("PTF=1");
	g.command("PTG=1");
	g.command("PTH=1");
		
	}
void Position_control_2(long int aP, long int bP, long int cP, long int dP, long int eP, long int fP, long int gP, long int v_value, int choice, Galil &g)
{
	if(choice == 0)
	{
		g.command("LM ABCDEFG");

	}
	else if(choice == 1) // Send Linear Segment to Galil Board
	{
		ostringstream buffer1;
		string str1;

		string command1 = "LI ";
		buffer1 << aP;
		str1 = buffer1.str();
		command1 = command1 + str1;
		command1 = command1 + ",";
		buffer1 << bP;
		str1 = buffer1.str();
		command1 = command1 + str1;
		command1 = command1 + ",";
		buffer1 << cP;
		str1 = buffer1.str();
		command1 = command1 + str1;
		command1 = command1 + ",";
		buffer1 << dP;
		str1 = buffer1.str();
		command1 = command1 + str1;
		command1 = command1 + ",";
		buffer1 << eP;
		str1 = buffer1.str();
		command1 = command1 + str1;
		command1 = command1 + ",";
		buffer1 << fP;
		str1 = buffer1.str();
		command1 = command1 + str1;
		command1 = command1 + ",";
		buffer1 << gP;
		str1 = buffer1.str();
		command1 = command1 + str1;
		command1 = command1 + "<";
		buffer1 << v_value;
		str1 = buffer1.str();
		command1 = command1 + str1;
		g.command(command1);
	}
	else if(choice == 2)
	{
		g.command("LE"); // End linear segments
		g.command("BGS");// Begin Motion Sequence
	}
}

/////////////Position control function to use with position tracking mode///////////////

void Position_control(char motor,long int p_value,long int v_value, long int a_value, Galil &g)
{
	
	string command1 = "PA";
	
	if ((p_value < -2147483646) || (p_value > 2147483647))
	{
		cerr << "The position value is outside the range" << endl;
	}

	motor = toupper(motor);

	if ((motor != 'A') && (motor != 'B') && (motor != 'C') && (motor != 'D') && (motor != 'E') && (motor != 'F') && (motor != 'G') && (motor != 'H'))
	{
		cerr << "The motor specified is not valid" << endl;
	}

	command1 = command1 + motor;
	
	command1 = command1 + "=";

	ostringstream buffer1;

	buffer1 << p_value;
	string str1 = buffer1.str();

	command1 = command1 + str1;

	//cout << "The string command contains: " << command1 << endl;
	
	g.command(command1);
	//cout<<" Position "<<endl;

	string command2 = "SP";
	
	if ((v_value < 0) || (v_value > 12000000))
	{
		cerr << "The velocity is outside the range" << endl;
	}
	command2 = command2 + motor;
	
	command2 = command2 + "=";
	
	ostringstream buffer2;

	buffer2 << v_value;
	string str2 = buffer2.str();
	
	command2 = command2 + str2;

	//cout << "The string command contains: " << command2 << endl;

	g.command(command2);

	string command3 = "AC";

	if ((a_value < 1024) || (a_value > 67107840))
	{
		cerr << "The acceleration  is outside the range" << endl;
	}

	command3 = command3 + motor;
	
	command3 = command3 + "=";

	std::ostringstream buffer3;

	buffer3 << a_value;
	string str3 = buffer3.str();
	
	command3 = command3 + str3;
	
	//cout << "The string command contains: " << command3 << endl;

	g.command(command3);
}

//This function filters the acceletration and decceleration to make smooth velocity profile//
void Time_constant (float tvalue, Galil &g)
{
	string svalue, command = "IT";
	
	if ((tvalue < 0.004) || (tvalue > 1))
	{
		cerr << "The  value is outside the range" << endl;
	}
		
	command = command + "*=";
		
	ostringstream sval;
	sval << tvalue;
	svalue = sval.str();

	command = command + svalue;
	
	//cout << "The string command contains: " << command << endl;

	g.command(command);
}
	
///////////////////////////////////////////////////////////////////////////
/////////////////////////PARAMETRE INITIALIZATION FUNCTIONS////////////////
/////////////////////////==================================////////////////

///////// PID gain assignment functions //////////

void PID(char PID_motor, float KP_value,float KD_value,float KI_value, Galil &g)
{
	cout << "Here" << endl;
	string commandp = "KP";
		
	if ((KP_value < 0) || (KP_value > 4095.875)) {
		cerr << "The KP value is outside the range" << endl;
	}

		PID_motor = toupper(PID_motor);

	if ((PID_motor != 'A') && (PID_motor != 'B') && (PID_motor != 'C') && (PID_motor != 'D') && (PID_motor != 'E') && (PID_motor != 'F') && (PID_motor != 'G') && (PID_motor != 'H')) {
		cerr << "The motor specified is not valid" << endl;
	}

	commandp = commandp + PID_motor;
	
	commandp = commandp + "=";

    ostringstream bufferPIDP;

	bufferPIDP << KP_value;
	string strp = bufferPIDP.str();
	
	commandp =  commandp + strp;
	
	//cout << "The string command contains: " << commandp << endl;

	g.command(commandp);
			
	string commandd = "KD";
		
	if ((KD_value < 0) || (KD_value > 4095.875))
	{
		cerr << "The KD value is outside the range" << endl;
	}


	commandd = commandd + PID_motor;
	
	commandd = commandd + "=";

    ostringstream bufferPIDd;

	bufferPIDd << KD_value;
	string strd = bufferPIDd.str();
	
	
	commandd =  commandd + strd;
	
	//cout << "The string command contains: " << commandd << endl;

	g.command(commandd);


	string commandi = "KI";
		
	if ((KI_value < 0) || (KI_value > 4095.875)) {
		cerr << "The KI value is outside the range" << endl;
	}


	commandi = commandi + PID_motor;
	
	commandi = commandi + "=";

    ostringstream bufferPIDi;

	bufferPIDi << KD_value;
	string stri = bufferPIDi.str();
	
	
	commandi = commandi + stri;
	
	//cout << "The string command contains: " << commandi << endl;

	g.command(commandi);
}

////////DARA RECORD FUNCTIONS////////
////////=====================////////

float Tell_pos(char motor, Galil &g)
{
	string svalue, command = "TP";
	float position;
	
	motor = toupper(motor);

	if ((motor != 'A') && (motor != 'B') && (motor != 'C') && (motor != 'D') && (motor != 'E') && (motor != 'F') && (motor != 'G') && (motor != 'H')) {
		cerr << "The motor specified is not valid" << endl;
	}

	command = command + motor;
	
	istringstream stream(g.command(command));
	stream >> position;
	return position;
}

float Tell_pos_er(char motor, Galil &g)
{
	string svalue, command = "TE";
	float position;
	motor = toupper(motor);

	if ((motor != 'A') && (motor != 'B') && (motor != 'C') && (motor != 'D') && (motor != 'E') && (motor != 'F') && (motor != 'G') && (motor != 'H')) {
		cerr << "The motor specified is not valid" << endl;
	}

	command = command + motor;
		

	//cout << "The string command contains: " << command << endl;

	//cout<<"The position error of the "<< motor<<" is = "<<g.command(command)<<endl;

	istringstream stream(g.command(command));
	stream >> position;
	return position;
}

void Define_Position (char motor, long int defvalue, Galil &g)
{
	string svalue, command ="DP";
	//cout << "Def value = " << defvalue << endl;

	if ((defvalue < -2147483647) || (defvalue > 2147483646))
	{
		cerr << "The position value is outside the range" << endl;
	}

	motor = toupper(motor);

	if ((motor != 'A') && (motor != 'B') && (motor != 'C') && (motor != 'D') && (motor != 'E') && (motor != 'F') && (motor != 'G') && (motor != 'H'))
	{
		cerr << "The motor specified is not valid" << endl;
	}

	command = command + motor;
	command = command + "=";
	ostringstream sval;
	sval << defvalue;
	svalue = sval.str();
	command = command + svalue;

	//cout << "The string command contains: " << command << endl;

	g.command(command);
}

void Brushed_Motor (char motor, int defvalue,Galil &g)
{
	string svalue, command ="BR";
	//cout<<"Def value = "<< defvalue<<endl;

	if ((defvalue < -2147483647) || (defvalue > 2147483646))
	{
		//Uncomment Please	
		cerr << "The position value is outside the range" << endl;
	}

	motor = toupper(motor);

	if ((motor != 'A') && (motor != 'B') && (motor != 'C') && (motor != 'D') && (motor != 'E') && (motor != 'F') && (motor != 'G') && (motor != 'H')) {
		cerr << "The motor specified is not valid" << endl;
	}

	command = command + motor;
	command = command + "=";
	ostringstream sval;
	sval << defvalue;
	svalue = sval.str();
	command = command + svalue;

	//cout << "The string command contains: " << command << endl;

	g.command(command);
}

void Wait_tomove (char motor, long int defvalue,Galil &g)
{
	string svalue, command ="AD";
	//cout<<"Def value = "<< defvalue<<endl;

	if ((defvalue < -2147483647) || (defvalue > 2147483646))
	{
		cerr << "The position value is outside the range" << endl;
	}

	motor = toupper(motor);

	if ((motor != 'A') && (motor != 'B') && (motor != 'C') && (motor != 'D') && (motor != 'E') && (motor != 'F') && (motor != 'G') && (motor != 'H'))
	{
		cerr << "The motor specified is not valid" << endl;
	}

	command = command + motor;
	command = command + "=";
	ostringstream sval;
	sval << defvalue;
	svalue = sval.str();
	command = command + svalue;

	//cout << "The string command contains: " << command << endl;

	g.command(command);
}