#ifndef MAINTENANCE_H
#define MAINTENANCE_H
#include <malloc.h>
#include <iostream>
#include <sstream>
#include "Galil.h"
#include "control.h"
#include <fstream>
#include <tchar.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "conio.h"
#include <vector>
//#include "ArmMotion2.h"

using namespace std;

//extern Galil g("192.168.1.22");
long vel_m = 200000, acc_m = 30000;

void manualControl(Galil &g);
void setAccVel();
void setPID(Galil &g);
void zeroPos(Galil &g);
void setPoseToZero(Galil &g);
void adjustVelocity(Galil &g);
void moveForward(char, Galil &g);
void moveBackward(char, Galil &g);
void moveStop(Galil &g);
void velocityMove(char motor, long int v_value, long int a_value, long int d_value, Galil &g);
int velocityAllStop(Galil &g);
void testLI(Galil &g);

void maintenance(Galil &g){
	int menuChoice;
	cout << "1: Manual Joint Control (Multi Joints)" << endl;
	cout << "2: Change the default Acceleration and Velocity values" << endl;
	cout << "3: Change the PID gains" << endl;
	cout << "4: Return all joints to 0 position" << endl;
	cout << "5: Turn motors off" << endl;
	cout << "6: Turn motors on" << endl;
	cout << "7: Set current encoder values to zero" << endl;
	cout << "8: Test Linear Interpolation\n" << endl;
	cout << "Choice: ";
	cin >> menuChoice;
	switch(menuChoice)
	{
	case 1: manualControl(g); break;
	case 2: setAccVel(); break;
	case 3: setPID(g); break;
	case 4: zeroPos(g); break;
	case 5: g.command("MO"); break;
	case 6: g.command("SH"); break;
	case 7: setPoseToZero(g); break;
	case 8: testLI(g);break;
	}
}

void testLI(Galil &g)
{
	int count = 0;
	while(count < 10)
	{
		if (count == 0)
		{
			Position_control_2(0, 0, 0, 0, 0, 0, 0, 0, 0, g);
		}
		else
		{
			Position_control_2(count*100000, 0, count*100000, 0, 0, 0, 0, 30000, 1, g);
		}
	}
	Position_control_2(0, 0, 0, 0, 0, 0, 0, 0, 2, g);
}


void zeroPos(Galil &g)
{
		Position_control('a', 0, vel_m, acc_m, g);
		Position_control('b', 0, vel_m, acc_m, g);
		Position_control('c', 0, vel_m, acc_m, g);
		Position_control('d', 0, vel_m, acc_m, g);
		Position_control('e', 0, vel_m, acc_m, g);
		Position_control('f', 0, vel_m, acc_m, g);
		Position_control('g', 0, vel_m, acc_m, g);

		cout << "\nJoints 1 through 7 are moving to the 0 encoder position\n" << endl; 
}

int adjustEncoder(Galil &g){
	char selection;
	long encoderPosition;
	bool encoderFlag = 1;
	while(encoderFlag){
		cout << "\n------------------------------------------" << endl;
		cout << "What joint?  ex: 1  (joints are '1' through '8') --- '0' to go to beginning" << endl;
		cin >> selection;

		if(selection == '0')
		{
			cout << "\nReturning to main menu\n" << endl;
			encoderFlag = 0;
			return 1;
		}
		else if(selection != '1' && selection != '2' && selection != '3' && selection != '4' && selection != '5' && selection != '6' && selection != '7' && selection != '8')
		{
			cout << "ERROR: User selected wrong joint number" << endl;
			return 0;
		}
		else if(selection == '1')
			selection = 'A';
		else if(selection == '2')
			selection = 'B';
		else if(selection == '3')
			selection = 'C';
		else if(selection == '4')
			selection = 'D';
		else if(selection == '5')
			selection = 'E';
		else if(selection == '6')
			selection = 'F';
		else if(selection == '7')
			selection = 'G';
		else if(selection == '8')
			selection = 'H';
		cout << "\nWhat encoder position? (small move = 100000)" << endl;
		cin >> encoderPosition;
		cout << endl;
		encoderPosition = Tell_pos(selection, g) + encoderPosition;
		Position_control(selection, encoderPosition, vel_m, acc_m, g);
	}
}
void setPoseToZero(Galil &g)
{
	g.command("DP 0,0,0,0,0,0,0,0");
}

int adjustAngle(Galil &g){
	long anglePosition, selection = 100;
	float angleVal;
	char motorLetter;
	bool angleFlag;
	vector<char> motorL;
	vector<long> motorP;
	
manualAngleCorrection:
	angleFlag = 1;
	while(angleFlag){
		cout << "\n------------------------------------------" << endl;
		//while(selection != 9 || selection != 0)
		//{
			cout << "What joint?  ex: 1  (joints are '1' through '7') --- '0' to go to beginning, '9' to start moving" << endl;
			cin >> selection;
			if(selection == 0)
			{
				angleFlag = 0;
				return 1;
			}
			else if(selection > 0 && selection < 9)
			{
				cout << "\nWhat angle position?" << endl;
				cin >> angleVal;
				cout << endl;

				switch (selection){
					case 1: anglePosition = (angleVal/360)*13190000; motorLetter ='A'; break;
					case 2: anglePosition = (angleVal/360)*10200000; motorLetter ='B'; break;
					case 3: anglePosition = (angleVal/360)*3600000; motorLetter ='C'; break;
					case 4: anglePosition = (angleVal/360)*4600000; motorLetter ='D'; break;
					case 5: anglePosition = (angleVal/360)*4000000; motorLetter ='E'; break;
					case 6: anglePosition = (angleVal/360)*-4000000; motorLetter ='F'; break;
					case 7: anglePosition = (angleVal/360)*3000000; motorLetter ='G'; break;
					// case 8: anglePosition = (angleVal/360)*100000; motorP.push_back(anglePosition); motorL.push_back('H'); break;  // Encoder value may need updating
					default: cout << "Error in the manual Angle Correction method" << endl; goto manualAngleCorrection; break;
				}
			}
		//}
			anglePosition = Tell_pos(motorLetter, g) + anglePosition;
			cout << anglePosition << endl;
			Position_control(motorLetter, anglePosition, vel_m, acc_m, g);
			string command1;
			command1 = "BG ";
			command1 = command1 + motorLetter;
			g.command(command1);
		//}
		//selection = 100;
		
	}
	return 0;
}

void manualControl(Galil &g){
	int selection;
	cout << "adjust by joint encoder position, joint angle (Multi Joints), or using velocity control?\n\t1 = encoder, 2=angle, 3= velocity control, 0=main menu\n" << endl;
	cin >> selection;
	if(selection == 1)
		adjustEncoder(g);
	else if(selection == 2)
		adjustAngle(g);
	else if(selection == 3)
		adjustVelocity(g);
	else if(selection == 0)
		cout << "Exiting" << endl;
	else
		cout << "value not valid" << endl;
}

void setAccVel()
{
	long int savedValue;
	cout << "\nWhat would you like to set the max Acceleration to? Currently set to: " << acc_m << endl;
	cin >> savedValue;
	acc_m = savedValue;

	cout << "\nWhat would you like to set the max Velocity to? Currently set to: " << vel_m << endl;
	cin >> savedValue;
	vel_m = savedValue;
}

void setPID(Galil &g)
{
	float Kp, Ki, Kd; // PID gains
	int savedValue;

continueSettingValues:;
	cout << "\nWhich Joint PID value do you want to edit? (1 through 7)" << endl;
	cin >> savedValue;
	switch(savedValue)
	{
	case 1: 
		cout << "\nKp = " << endl;
		cin >> Kp;
		cout << "Ki = " << endl;
		cin >> Ki;
		cout << "Kd = " << endl;
		cin >> Kd;
		PID('A', Kp, Kd, Ki, g);
		break;
	case 2: 
		cout << "\nKp = " << endl;
		cin >> Kp;
		cout << "Ki = " << endl;
		cin >> Ki;
		cout << "Kd = " << endl;
		cin >> Kd;
		PID('B', Kp, Kd, Ki, g);
		break;
	case 3: 
		cout << "\nKp = " << endl;
		cin >> Kp;
		cout << "Ki = " << endl;
		cin >> Ki;
		cout << "Kd = " << endl;
		cin >> Kd;
		PID('C', Kp, Kd, Ki, g);
		break;
	case 4: 
		cout << "\nKp = " << endl;
		cin >> Kp;
		cout << "Ki = " << endl;
		cin >> Ki;
		cout << "Kd = " << endl;
		cin >> Kd;
		PID('D', Kp, Kd, Ki, g);
		break;
	case 5: 
		cout << "\nKp = " << endl;
		cin >> Kp;
		cout << "Ki = " << endl;
		cin >> Ki;
		cout << "Kd = " << endl;
		cin >> Kd;
		PID('E', Kp, Kd, Ki, g);
		break;
	case 6: 
		cout << "\nKp = " << endl;
		cin >> Kp;
		cout << "Ki = " << endl;
		cin >> Ki;
		cout << "Kd = " << endl;
		cin >> Kd;
		PID('F', Kp, Kd, Ki, g);
		break;
	case 7: 
		cout << "\nKp = " << endl;
		cin >> Kp;
		cout << "Ki = " << endl;
		cin >> Ki;
		cout << "Kd = " << endl;
		cin >> Kd;
		PID('G', Kp, Kd, Ki, g);
		break;
	case 8: 
		cout << "\nKp = " << endl;
		cin >> Kp;
		cout << "Ki = " << endl;
		cin >> Ki;
		cout << "Kd = " << endl;
		cin >> Kd;
		PID('H', Kp, Kd, Ki, g);
		break;
	}
	cout << "Do you wish to change more values? 1 = yes, 0 = no" << endl;
	cin >> savedValue;
	if(savedValue)
		goto continueSettingValues;
}

void adjustVelocity(Galil &g)
{
	float PI = 3.14;
	float e2r1=2*PI/13200000;//a
	float e2r2=2*PI/13320000;//b
	float e2r3=2*PI/4720000;//c
	float e2r4=2*PI/4720000;//d
	float e2r5=2*PI/3840000;//e
	float e2r6=-(2*PI)/4000000;//f
	float e2r7=2*PI/1440000; //g
	float e2r8=1; // Redwan: change this to forward motion when wheelchair controllers are installed (Only when reading the encoders).
	float e2r9=1; // Redwan: change this to rotation motion when wheelchair controllers are installed (Only when reading the encoders).
	float e2d =1/100000;//h


	int keypress;
	char currentMotor = 'A';
	cout << "\n\nhit 'W' to rotate the joint forward.\nhit 'S' to rotate the joint backwards.\nhit 'D' to stop movement.\nhit 'E' to change joint motor (Default is joint A).\nhit 'Q' to exit." << endl;
	while (1)
    {
		keypress = ' ';
		// Check for keyboard input.
		if (_kbhit())
			keypress = _getch();
		
		keypress = toupper(keypress);

		if (keypress == 'Q')
		{
			moveStop(g);
			return;
		}
		else if (keypress == 'P')
		{
			cout << "Joint 1: " << Tell_pos('A', g)*e2r1 << endl;
			cout << "Joint 2: " << Tell_pos('B', g)*e2r2 << endl;
			cout << "Joint 3: " << Tell_pos('C', g)*e2r3 << endl;
			cout << "Joint 4: " << Tell_pos('D', g)*e2r4 << endl;
			cout << "Joint 5: " << Tell_pos('E', g)*e2r5 << endl;
			cout << "Joint 6: " << Tell_pos('F', g)*e2r6 << endl;
			cout << "Joint 7: " << Tell_pos('G', g)*e2r7 << endl;
			cout << "Joint 8: " << Tell_pos('H', g)*e2d << endl;
		}
		else if (keypress == 'W')
		{
			moveForward(currentMotor, g);
		}
		else if (keypress == 'S')
		{
			moveBackward(currentMotor, g);
		}
		else if (keypress == 'D')
		{
			moveStop(g);
		}
		else if (keypress == '1')
		{
			cout << "Joint 1 active" << endl;
			currentMotor = 'A';
		}
		else if (keypress == '2')
		{
			cout << "Joint 2 active" << endl;
			currentMotor = 'B';
		}
		else if (keypress == '3')
		{
			cout << "Joint 3 active" << endl;
			currentMotor = 'C';
		}
		else if (keypress == '4')
		{
			cout << "Joint 4 active" << endl;
			currentMotor = 'D';
		}
		else if (keypress == '5')
		{
			cout << "Joint 5 active" << endl;
			currentMotor = 'E';
		}
		else if (keypress == '6')
		{
			cout << "Joint 6 active" << endl;
			currentMotor = 'F';
		}
		else if (keypress == '7')
		{
			cout << "Joint 7 active" << endl;
			currentMotor = 'G';
		}
		else if (keypress == '8')
		{
			cout << "Joint 8 active" << endl;
			currentMotor = 'H';
		}
		/*else if(keypress == 'E')
		{
			int motorNum;
			cout << "Which joint would you like to control? 1 through 8" << endl;
			cin >> motorNum;
			switch (motorNum){
				case 1: currentMotor = 'A'; break;
				case 2: currentMotor = 'B'; break;
				case 3: currentMotor = 'C'; break;
				case 4: currentMotor = 'D'; break;
				case 5: currentMotor = 'E'; break;
				case 6: currentMotor = 'F'; break;
				case 7: currentMotor = 'G'; break;
				case 8: currentMotor = 'H'; break;
				default: cout << "Error in setting the motor letter" << endl; break;
			}
		}*/
	}
}

void moveForward(char motor, Galil &g)
{
	velocityMove(motor, 100000, 50000, 50000, g);
}

void moveBackward(char motor, Galil &g)
{
	velocityMove(motor, -100000, 50000, 50000, g);
}
void moveStop(Galil &g)
{
	velocityAllStop(g);
}

void velocityMove(char motor, long int v_value, long int a_value, long int d_value, Galil &g)
{
	string command1 = "#";

	motor = toupper(motor);

	if ((motor != 'A') && (motor != 'B') && (motor != 'C') && (motor != 'D') && (motor != 'E') && (motor != 'F') && (motor != 'G') && (motor != 'H')) {
			cerr << "The motor specified is not valid" << endl;
		}

	command1 = command1 + motor;

	g.command(command1);

	if (v_value != 0)
	{
		command1 = "JG";
		command1 = command1 + motor;
		command1 = command1 + "=";

		ostringstream buffer1;
		buffer1 << v_value;
		string str1 = buffer1.str();
		command1 = command1 + str1;
		g.command(command1);
	}

	if (a_value != 0)
	{
		command1 = "AC";
		command1 = command1 + motor;
		command1 = command1 + "=";
		ostringstream buffer1;
		buffer1 << a_value;
		string str1 = buffer1.str();
		command1 = command1 + str1;
		g.command(command1);
	}

	if (d_value != 0)
	{
		command1 = "DC";
		command1 = command1 + motor;
		command1 = command1 + "=";
		ostringstream buffer1;
		buffer1 << d_value;
		string str1 = buffer1.str();
		command1 = command1 + str1;
		g.command(command1);
	}

	command1 = "BG ";
	command1 = command1 + motor;
	g.command(command1);
}
int velocityAllStop(Galil &g)
{
		g.command("ST A");
		g.command("ST B");
		g.command("ST C");
		g.command("ST D");
		g.command("ST E");
		g.command("ST F");
		g.command("ST G");
		g.command("ST H");
		//sleep(1000);
		return 1;
}

#endif