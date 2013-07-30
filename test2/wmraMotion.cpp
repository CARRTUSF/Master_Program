/*
 * wmraMotion.cpp
 *
 *  Created on: August 2, 2010
 *      Author: USF CARRT (WMRA Project)
 *
 *      This wmraMotion.cpp file includes all of the functions that
 *		are used to control the motion of the wheelchair via
 *		PC using the Lab Jack U6 device through USB. The 
 *		Lab Jack device must be connected to the computer through
 *		USB. The Lab Jack driver files must be installed in 
 *		order for this library to work. The driver files are 
 *		included and used in this library. 
 *		This is where we do stuff! Prototypes are in wmraMotion.h.
 *		Please see wmraMotion.h for additional required notes!
 *
 *		DACA -> blue wire (gray internal)
 *		DACB -> yellow wire (white internal)
 *		AIN0 -> green wire
 *		AIN1 -> blue wire
 *		AIN2 -> yellow wire
 *		AIN3 -> I1 proximity sensor
 *		AIN4 -> I2 proximity sensor
 *		AIN5 -> I3 proximity sensor
 *		AIN6 -> I4 proximity sensor
 *		FIO2 -> Encoder1 CHA
 *		FIO3 -> Encoder1 CHB
 *		FIO4 -> Encoder2 CHA
 *		FIO5 -> Encoder2 CHB
 *		FIO6 -> Encoder1 Index
 *		FIO7 -> Encoder2 Index
 *		EIO0 -> S1 sonar sensor
 *		EIO1 -> S2 sonar sensor
 *
 *		encoder1 is on the right side (as viewed from sitting in the chair)
 *		encoder2 is on the left side
 */

#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <math.h>
#include "wmraMotion.h"
// SEE THE wmraMotion.h FILE FOR INFORMATION ON ADDING THE REQUIRED
// DRIVER FILES AND LIBRARY!!!
#include "c:\program files\labjack\drivers\LabJackUD.h"

#define SPEED 5 //speed in Go() for wheelchair manipulation (suggested 3-6)
#define PROXIMITY 0.75 //closeness of wheelchair to obstacles in autonomous motion in GoAvoid() (suggested 0-1)
#define AUTSPEED 0.5 //speed in autonomous motion in GoAvoid() (0-1, suggested 0.4-0.7)

LJ_HANDLE lngHandle=0; //global variable for LabJack handle

using namespace std; //we use namespace to keep things simple

//==========================================================
// ErrorHandler() is an error checking function built in. 
// We use this when we call a Lab Jack function just in case
// the Lab Jack has an issue during operation. The sample
// code uses this function, so we may as well to stay on the
// safe side. 
//==========================================================
//This is our simple error handling function that is called after every UD
//function call.  This function displays the errorcode and string description
//of the error.  It also has a line number input that can be used with the
//macro __LINE__ to display the line number in source code that called the
//error handler.  It also has an iteration input is useful when processing
//results in a loop (getfirst/getnext).
void ErrorHandler (LJ_ERROR lngErrorcode, long lngLineNumber, long lngIteration)
{
	char err[255];

	if (lngErrorcode != LJE_NOERROR)
	{
		ErrorToString(lngErrorcode,err);
		printf("Error number = %d\n",lngErrorcode);
		printf("Error string = %s\n",err);
		printf("Source line number = %d\n",lngLineNumber);
		printf("Iteration = %d\n\n",lngIteration);
		if(lngErrorcode > LJE_MIN_GROUP_ERROR)
		{
			//Quit if this is a group error.
			getchar();
			exit(0);
		}
   }
}

//==========================================================
// Initialize() goes through the rounds of finding and 
// opening the Lab Jack U6. You must uses this Initialize()
// function before using any other functions. Returns 1 for
// error condition, 0 for proper execution.
//==========================================================
int Initialize ()
{
	LJ_ERROR lngErrorcode;

	//Open the first found LabJack U6.
	lngErrorcode = OpenLabJack (LJ_dtU6, LJ_ctUSB, "1", 1, &lngHandle);
	ErrorHandler(lngErrorcode, __LINE__, 0);

	//I think this resets the U6 to the default settings??
	//Start by using the pin_configuration_reset IOType so that all
	//pin assignments are in the factory default condition.
	lngErrorcode = ePut (lngHandle, LJ_ioPIN_CONFIGURATION_RESET, 0, 0, 0);
	ErrorHandler(lngErrorcode, __LINE__, 0);

	return 0;
}

//==========================================================
// SetTD sets the analog output for DACA and DACB on the LJ
// Tick DAC (+/- 10V). Channel specifies the channel to set
// (A,B) and value is the analog voltage output. Value must 
// be 0-10 (volts). Returns 1 for error condition, 0 for 
// proper execution.
//==========================================================
int SetTD (char channel, double value)
{
	LJ_ERROR lngErrorcode;

	//Here we do some error checking to make sure value is
	//in range (0-10). 
	if ((value < 0) || (value > 10))
	{
		cerr << "In setting Tick DAC voltage, value is out of range. Program will terminate!" << endl;
		return 1;
	}

	//Tell the driver that SCL is on FIO0. The driver then assumes that SDA is on FIO1.
	//This is just setting a parameter in the driver, and not actually talking
	//to the hardware, and thus executes very fast.
	lngErrorcode = ePut (lngHandle, LJ_ioPUT_CONFIG, LJ_chTDAC_SCL_PIN_NUM, 0, 0);

	//Here we set the values.
	if ((channel == 'a') || (channel == 'A')) //DACA=value
	{
		lngErrorcode = ePut(lngHandle, LJ_ioTDAC_COMMUNICATION, LJ_chTDAC_UPDATE_DACA, value, 0);
		ErrorHandler(lngErrorcode, __LINE__, 0);
	}
	else if ((channel == 'b') || (channel == 'B')) //DACB=value
	{
		lngErrorcode = ePut(lngHandle, LJ_ioTDAC_COMMUNICATION, LJ_chTDAC_UPDATE_DACB, value, 0);
		ErrorHandler(lngErrorcode, __LINE__, 0);
	}
	else //Here we check to make sure channel is valid.
	{
		cerr << "In setting Tick DAC voltage, specified channel does not exist. Program will terminate!" << endl;
		return 1;
	}

	return 0;
}

//==========================================================
// SetDAC sets the analog output for DAC0 and DAC1 (+/- 5V). 
// Channel specifies the channel to set (0,1) and value is 
// the analog voltage output. Value must be 0-5 (volts). 
// Returns 1 for error condition, 0 for proper execution.
//==========================================================
int SetDAC (int channel, double value)
{
	LJ_ERROR lngErrorcode;

	//Here we do some error checking to make sure value is
	//in range (0-5). 
	if ((value < 0) || (value > 5))
	{
		cerr << "In setting Tick DAC voltage, value is out of range. Program will terminate!" << endl;
		return 1;
	}

	//Here we set the values.
	if (channel == 0) //DAC0=value
	{
		lngErrorcode = ePut(lngHandle, LJ_ioPUT_DAC, 0, value, 0);
		ErrorHandler(lngErrorcode, __LINE__, 0);
	}
	else if (channel == 1) //DAC1=value
	{
		lngErrorcode = ePut(lngHandle, LJ_ioPUT_DAC, 1, value, 0);
		ErrorHandler(lngErrorcode, __LINE__, 0);
	}
	else //Here we check to make sure channel is valid.
	{
		cerr << "In setting DAC voltage, specified channel does not exist. Program will terminate!" << endl;
		return 1;
	}

	return 0;
}

//==========================================================
// GetAIN gets the analog input for AIN0 through AIN13 
// (+/- 10V). Channel specifies the channel to read (0-13) 
// and value (analog input) is passed and updated. Returns 1 
// for error condition, 0 for proper execution.
//==========================================================
int GetAIN (int channel, double &value)
{
	LJ_ERROR lngErrorcode;

	//Here we do some error checking to make sure channel is
	//valid (0-13). 
	if ((channel < 0) || (channel > 13))
	{
		cerr << "In reading AIN voltage, specified channel does not exist. Program will terminate!" << endl;
		return 1;
	}

	//Here we read the values.
	//Take a single-ended measurement from AIN[channel].
	lngErrorcode = eAIN (lngHandle, channel, 199, &value, -1, -1, -1, 0, 0, 0);
	ErrorHandler(lngErrorcode, __LINE__, 0);

	return 0;
}

//==========================================================
// SetDO sets the digital output for FIO0 through MIO2 
// (3.3/5V). Channel specifies the channel to read (0-22) 
// and value is the desired output (1=high, 0=low). Returns 
// 1 for error condition, 0 for proper execution.
//==========================================================
int SetDO (int channel, long value)
{
	LJ_ERROR lngErrorcode;

	//Here we do some error checking to make sure channel is
	//valid (0-22). 
	if ((channel < 0) || (channel > 22))
	{
		cerr << "In setting DO, specified channel does not exist. Program will terminate!" << endl;
		return 1;
	}

	//Here we do some error checking to make sure value is
	//valid (0 or 1).
	if ((value < 0) || (value > 1))
	{
		cerr << "In setting DO, value is out of range. Program will terminate!" << endl;
		return 1;
	}

	//Here we set the value.
	lngErrorcode = eDO (lngHandle, channel, value);
	ErrorHandler(lngErrorcode, __LINE__, 0);

	return 0;
}

//==========================================================
// GetDI gets the digital input for FIO0 through MIO2 
// (3.3/5V). Channel specifies the channel to read (0-22) 
// and value is the digital input (1=high, 0=low). Returns 
// 1 for error condition, 0 for proper execution.
//==========================================================
int GetDI (int channel, long &value)
{
	LJ_ERROR lngErrorcode;

	//Here we do some error checking to make sure channel is
	//valid (0-22). 
	if ((channel < 0) || (channel > 22))
	{
		cerr << "In getting DI, specified channel does not exist. Program will terminate!" << endl;
		return 1;
	}

	//Here we set the value.
	lngErrorcode = eDI (lngHandle, channel, &value);
	ErrorHandler(lngErrorcode, __LINE__, 0);

	return 0;
}

//==========================================================
// StartStream sets up the stream to read the sonar 
// proximity sensor distance. Stream is configured and 
// started, then GetStreamDistance() is used to get the 
// distance. Returns 1 for error condition, 0 for proper
// execution.
//==========================================================
int StartStream ()
{
	LJ_ERROR lngErrorcode;
	long lngGetNextIteration;
	long lngIOType = 0, lngChannel = 0;
	double dblValue = 0;
	double scanRate = 50000;

	//cout << "Starting stream..." << endl;

	//Start by using the pin_configuration_reset IOType so that all
	//pin assignments are in the factory default condition.
	//lngErrorcode = ePut (lngHandle, LJ_ioPIN_CONFIGURATION_RESET, 0, 0, 0);
	//ErrorHandler(lngErrorcode, __LINE__, 0);
	//This is done in Initialize(), so we comment it out here

	//Configure FIO0 and FIO1 as analog, all else as digital.  That means we
	//will start from channel 0 and update all 16 flexible bits.  We will
	//pass a value of b0000000000000011 or d3.
	lngErrorcode = ePut (lngHandle, LJ_ioPUT_ANALOG_ENABLE_PORT, 0, 3, 16);
	ErrorHandler(lngErrorcode, __LINE__, 0);

	//Configure the stream:
    //Set the scan rate.
    lngErrorcode = AddRequest(lngHandle, LJ_ioPUT_CONFIG, LJ_chSTREAM_SCAN_FREQUENCY, scanRate, 0, 0);
    ErrorHandler(lngErrorcode, __LINE__, 0);
    //Give the driver a 5 second buffer (scanRate * 2 channels * 5 seconds).
    lngErrorcode = AddRequest(lngHandle, LJ_ioPUT_CONFIG, LJ_chSTREAM_BUFFER_SIZE, scanRate*2*5, 0, 0);
    ErrorHandler(lngErrorcode, __LINE__, 0);
    //Configure reads to retrieve whatever data is available without waiting (wait mode LJ_swNONE).
	//See comments below to change this program to use LJ_swSLEEP mode.
    lngErrorcode = AddRequest(lngHandle, LJ_ioPUT_CONFIG, LJ_chSTREAM_WAIT_MODE, LJ_swNONE, 0, 0);
    ErrorHandler(lngErrorcode, __LINE__, 0);
    //Define the scan list as AIN0 then FIOEIO.
    lngErrorcode = AddRequest(lngHandle, LJ_ioCLEAR_STREAM_CHANNELS, 0, 0, 0, 0);
    ErrorHandler(lngErrorcode, __LINE__, 0);
    lngErrorcode = AddRequest(lngHandle, LJ_ioADD_STREAM_CHANNEL, 193, 0, 0, 0);
    ErrorHandler(lngErrorcode, __LINE__, 0);
    
    //Execute the list of requests.
    lngErrorcode = GoOne(lngHandle);
    ErrorHandler(lngErrorcode, __LINE__, 0);
    
	//Get all the results just to check for errors.
	lngErrorcode = GetFirstResult(lngHandle, &lngIOType, &lngChannel, &dblValue, 0, 0);
	ErrorHandler(lngErrorcode, __LINE__, 0);
	lngGetNextIteration=0;	//Used by the error handling function.

	while (lngErrorcode < LJE_MIN_GROUP_ERROR)
	{
		lngErrorcode = GetNextResult(lngHandle, &lngIOType, &lngChannel, &dblValue, 0, 0);

		if (lngErrorcode != LJE_NO_MORE_DATA_AVAILABLE)
		{
			ErrorHandler (lngErrorcode, __LINE__, lngGetNextIteration);
		}

		lngGetNextIteration++;
	}
    
	//Start the stream.
    lngErrorcode = eGet(lngHandle, LJ_ioSTART_STREAM, 0, &dblValue, 0);
    ErrorHandler(lngErrorcode, __LINE__, 0);

	//cout << "Stream started" << endl;

	return 0;
}

//==========================================================
// GetStreamDistance reads the sonar proximity sensor 
// distance in cm. Sonar sensor signal wire is on EIO0 for
// S1 and EIO1 for S2. StartStream() must be called before 
// GetStreamDistance. Updates s1 and s2 with distances in
// cm. Returns 1 for error condition, 0 for proper 
// execution.
//==========================================================
int GetStreamDistance (double &s1, double &s2)
{
	LJ_ERROR lngErrorcode;
	int k = 0;
	long value = 0;
	long delayms = 500; //default 500
	double numScans = 50000;  //2x the expected # of scans (2*scanRate*delayms/1000)
	double numScansRequested;
	double adblData[50000] = {0};  //Max buffer size (#channels*numScansRequested)
	int count1 = 0, flag1 = 0, count2 = 0, flag2 = 0;
	double distance1, distance2;

	//Output data can be printed to file, uncomment outfile statements to print
	//FILE *results;

	//Make a long parameter which holds the address of the data array.  We do this
	//so the compiler does not generate a warning in the eGet call that retrieves
	//stream data.  Note that the x1 parameter  in eGet (and AddRequest) is fairly
	//generic, in that sometimes it could just be a write parameter, and sometimes
	//it has the address of an array.  Since x1 is not declared as a pointer, the
	//compiler will complain if you just pass the array pointer without casting
	//it to a long as follows.
	long padblData = (long)&adblData[0];

	//Send pulse signal to EIO0
	if (SetDO(8, 1))
	{
		cerr << "There was a problem sending the pulse. Program will terminate!" << endl;
		return 1;
	}

	//Send pulse signal to EIO1
	if (SetDO(9, 1))
	{
		cerr << "There was a problem sending the pulse. Program will terminate!" << endl;
		return 1;
	}

	//Set EIO0 back to digital input
	if (GetDI(8, value))
	{
		cerr << "There was a problem getting DI. Program will terminate!" << endl;
		return 1;
	}

	//Set EIO1 back to digital input
	if (GetDI(9, value))
	{
		cerr << "There was a problem getting DI. Program will terminate!" << endl;
		return 1;
	}

	//Read data
	//Since we are using wait mode LJ_swNONE, we will wait a little, then
	//read however much data is available.  Thus this delay will control how
	//fast the program loops and how much data is read each loop.  An
	//alternative common method is to use wait mode LJ_swSLEEP where the
	//stream read waits for a certain number of scans.  In such a case
	//you would not have a delay here, since the stream read will actually
	//control how fast the program loops.
	//
	//To change this program to use sleep mode,
	//	-change numScans to the actual number of scans desired per read,
	//	-change wait mode addrequest value to LJ_swSLEEP,
	//	-comment out the following Sleep command.
	Sleep(delayms);	//Remove if using LJ_swSLEEP.

	//Read the data.  We will request twice the number we expect, to
	//make sure we get everything that is available.
	//Note that the array we pass must be sized to hold enough SAMPLES, and
	//the Value we pass specifies the number of SCANS to read.
	numScansRequested=numScans;
	lngErrorcode = eGet(lngHandle, LJ_ioGET_STREAM_DATA, LJ_chALL_CHANNELS, &numScansRequested, padblData);

	//if ((results = fopen("results.txt", "w")) == NULL)
	//{
	//	cerr << "Cannot open output file!" << endl;
	//}

	for (k=0; k<numScans; k++)
	{
		//fprintf(results,"Scan = %.3f\n",adblData[k]); //Print results to file

		//Results for sonar1 (EIO0)
		if ((flag1 == 0) && ((adblData[k] == 65023) || (adblData[k] == 65535))) //When we reach initial pulse, set flag to 1
		{
			flag1 = 1;
		}
		else if ((flag1 == 1) && ((adblData[k] == 64767) || (adblData[k] == 65279))) //When we go back to hold period, set flag to 2
		{
			flag1 = 2;
		}
		else if ((flag1 == 2) && ((adblData[k] == 65023) || (adblData[k] == 65535))) //When we reach the response pulse, measure delay
		{
			count1++;
		}

		//Results for sonar2 (EIO1)
		if ((flag2 == 0) && ((adblData[k] == 65279) || (adblData[k] == 65535))) //When we reach initial pulse, set flag to 1
		{
			flag2 = 1;
		}
		else if ((flag2 == 1) && ((adblData[k] == 64767) || (adblData[k] == 65023))) //When we go back to hold period, set flag to 2
		{
			flag2 = 2;
		}
		else if ((flag2 == 2) && ((adblData[k] == 65279) || (adblData[k] == 65535))) //When we reach the response pulse, measure delay
		{
			count2++;
		}
	}

	//fclose(results);

	//We divide by the calibration factor (2.95) to find distance in cm
	distance1 = count1 / 2.95;
	distance2 = count2 / 2.95;
	//cout << "Count is " << count << ", distance is " << distance << "cm" << endl;

	return 0;
}

//==========================================================
// StopStream stops the stream reading the sonar proximity
// sensors. Stream should be stopped after the application 
// is done reading distance values. Returns 1 for error 
// condition, 0 for proper execution.
//==========================================================
int StopStream ()
{
	LJ_ERROR lngErrorcode;

	//cout << "Stopping stream..." << endl;

    //Stop the stream
    lngErrorcode = eGet(lngHandle, LJ_ioSTOP_STREAM, 0, 0, 0);
    ErrorHandler(lngErrorcode, __LINE__, 0);

	//cout << "Stream stopped" << endl;

	return 0;
}

//==========================================================
// InitializeTimers configures and starts the timers for 
// encoder 1 and 2. Note that when this function is called,
// timers are reset. Returns 1 for error condition, 0 for 
// proper execution.
//==========================================================
int InitializeTimers ()
{
	LJ_ERROR lngErrorcode;
	long lngIOType, lngChannel, lngGetNextIteration;
	double dblValue;

	//TIMER SETUP---->
	//Set pin offset to 2
	AddRequest (lngHandle, LJ_ioPUT_CONFIG, LJ_chTIMER_COUNTER_PIN_OFFSET, 2, 0, 0);

	//Enable 4 timers. They will use FIO2-FIO5
	AddRequest (lngHandle, LJ_ioPUT_CONFIG, LJ_chNUMBER_TIMERS_ENABLED, 4, 0, 0);

	//Make sure Counters are disabled.
	AddRequest (lngHandle, LJ_ioPUT_COUNTER_ENABLE, 0, 0, 0, 0);
	AddRequest (lngHandle, LJ_ioPUT_COUNTER_ENABLE, 1, 0, 0, 0);

	//Configure Timers as quadrature.
	AddRequest(lngHandle, LJ_ioPUT_CONFIG, LJ_chTIMER_CLOCK_BASE, LJ_tc1MHZ_DIV, 0, 0);
	AddRequest(lngHandle, LJ_ioPUT_CONFIG, LJ_chTIMER_CLOCK_DIVISOR, 1, 0, 0);
	AddRequest(lngHandle, LJ_ioPUT_TIMER_MODE, 0, LJ_tmQUAD, 0, 0); 
	AddRequest(lngHandle, LJ_ioPUT_TIMER_MODE, 1, LJ_tmQUAD, 0, 0);
	AddRequest(lngHandle, LJ_ioPUT_TIMER_MODE, 2, LJ_tmQUAD, 0, 0); 
	AddRequest(lngHandle, LJ_ioPUT_TIMER_MODE, 3, LJ_tmQUAD, 0, 0);

	//Reset value
	AddRequest(lngHandle, LJ_ioPUT_TIMER_VALUE, 0, 0, 0, 0);
	
	lngErrorcode = GoOne (lngHandle);
	ErrorHandler(lngErrorcode, __LINE__, 0);

	//Get all the results.  The input measurement results are stored.  All other
	//results are for configuration or output requests so we are just checking
	//whether there was an error.
	lngErrorcode = GetFirstResult(lngHandle, &lngIOType, &lngChannel, &dblValue, 0, 0);
	ErrorHandler(lngErrorcode, __LINE__, 0);
	lngGetNextIteration=0;	//Used by the error handling function.
	while(lngErrorcode < LJE_MIN_GROUP_ERROR)
	{
		lngErrorcode = GetNextResult(lngHandle, &lngIOType, &lngChannel, &dblValue, 0, 0);

		if(lngErrorcode != LJE_NO_MORE_DATA_AVAILABLE)
		{
			ErrorHandler(lngErrorcode, __LINE__, lngGetNextIteration);
		}

		lngGetNextIteration++;
	}
	//END TIMER SETUP---->

	return 0;
}

//==========================================================
// GetTimers gets the timer values for encoder 1 and 2. The
// parameters are passed into the function and updated. 
// Returns 1 for error condition, 0 for proper execution.
//==========================================================
int GetTimers (double &encoder1, double &encoder2)
{
	LJ_ERROR lngErrorcode;

	//Read values from encoders
	//ENCODER1 (read FIO2 : FIO2+FIO3 quad1)
	lngErrorcode = eGet (lngHandle, LJ_ioGET_TIMER, 0, &encoder1, 0);
	ErrorHandler(lngErrorcode, __LINE__, 0);
	//ENCODER2 (read FIO4 : FIO4+FIO5 quad2)
	lngErrorcode = eGet (lngHandle, LJ_ioGET_TIMER, 2, &encoder2, 0);
	ErrorHandler(lngErrorcode, __LINE__, 0);

	return 0;
}

//==========================================================
// GetReference() reads in the analog input value for 
// reference from the joystick (green wire). This will give
// us the proper value to set the analog outputs for an idle
// situation. It returns the value of the reference, which
// should be close to 6V. 
//==========================================================
double GetReference ()
{
	double green = 6;
	
	//Take a single-ended measurement from AIN0. AIN0 is the green wire!
	if (GetAIN(0, green))
	{
		cerr << "There was a problem reading reference. Program will terminate!" << endl;
		return 6;
	}

	return green;
}

//==========================================================
// Stop() is a function that stops the wheelchair motion and
// sets it to an idle condition. It uses GetReference() to
// find the reference value (~6V) and then sets the analog
// outputs to this value for the idle condition.
//==========================================================
int Stop ()
{
	double green = 6;
	green = GetReference();

	if ((SetTD('a', green)) || (SetTD('b', green)))
	{
		cerr << "There was a problem setting reference. Program will terminate!" << endl;
		return 1;
	}

	return 0;
}

//==========================================================
// Move() takes in specified values to control the motion of
// the wheelchair by varying the analog output values away
// from the reference value...
//
//	direction = "forward", "reverse", "left", "right"
//		string direction is converted tolower() which means
//		it is not case sensitive (ex. LeFt = left)
//	speed = int value from 0 to 10
//		values of 0-1 may not move wheelchair
//	duration = int value in seconds of duration of movement
//		Stop() is called after sleep(duration) to return to
//		idle
// Returns 1 for error condition, 0 for proper execution.
//==========================================================
int Move (string direction, double speed, double duration)
{
	double reference = 6;
	double encoder1 = 0;
	double encoder2 = 0;
	double daca, dacb, offset;

	//Here we convert direction to all lower-case. This way
	//the input for direction is not case-sensitive. 
	//FoRWarD == forward == FORWARD
	for (unsigned int i=0; i<direction.length(); i++)
	{
		direction[i] = tolower(direction[i]);
	}

	//Here we do some error-checking on the speed input
	if (speed > 10 || speed < 0)
	{
		cerr << "Speed is greater than max allowed, program will terminate!" << endl;
		Stop(); //Set the wheelchair to idle
		return 1; //Break from the program
	}

	//Manipulate speed input to offset value
	offset = speed / 10;

	//Retrieve current reference value
	reference = GetReference();

	//TIMER SETUP
	if (InitializeTimers())
	{
		cerr << "There was a problem initializing the timers, program will terminate!" << endl;
		Stop();
		return 1;
	}

	//Here we have a massive if statement that does everything
	if (direction == "forward")
	{
		//FORWARD motion
		//DACA (blue) stays reference
		//DACB (yellow) is reference incremented by offset
		dacb = reference + offset;

		if ((SetTD('a', reference)) || (SetTD('b', dacb)))
		{
			cerr << "There was a problem setting reference. Program will terminate!" << endl;
			return 1;
		}

		while ((duration > encoder1) && (-duration < encoder2))
		{
			//Read values from encoders
			if (GetTimers(encoder1, encoder2))
			{
				cerr << "There was a problem reading the encoders, program will terminate!" << endl;
				Stop();
				return 1;
			}

			//double-check loop condition
			if ((duration < encoder1) && (-duration > encoder2))
			{
				break;
			}
		}
	}
	else if (direction == "reverse")
	{
		//REVERSE motion
		//DACA (blue) stays reference
		//DACB (yellow) is reference decremented by offset
		dacb = reference - offset;

		if ((SetTD('a', reference)) || (SetTD('b', dacb)))
		{
			cerr << "There was a problem setting reference. Program will terminate!" << endl;
			return 1;
		}

		while ((-duration < encoder1) && (duration > encoder2))
		{
			//Read values from encoders
			if (GetTimers(encoder1, encoder2))
			{
				cerr << "There was a problem reading the encoders, program will terminate!" << endl;
				Stop();
				return 1;
			}

			//double-check loop condition
			if ((-duration > encoder1) && (duration < encoder2))
			{
				break;
			}
		}
	}
	else if (direction == "left")
	{
		//LEFT motion
		//DACA (blue) is reference decremented by offset
		daca = reference - offset;
		//DACB (yellow) stays reference

		if ((SetTD('a', daca)) || (SetTD('b', reference)))
		{
			cerr << "There was a problem setting reference. Program will terminate!" << endl;
			return 1;
		}

		while (duration > encoder1)
		{
			//Read values from encoder
			if (GetTimers(encoder1, encoder2))
			{
				cerr << "There was a problem reading the encoders, program will terminate!" << endl;
				Stop();
				return 1;
			}

			//double-check loop condition
			if (duration < encoder1)
			{
				break;
			}
		}
	}
	else if (direction == "right")
	{
		//RIGHT motion
		//DACA (blue) is reference incremented by offset
		daca = reference + offset;
		//DACB (yellow) stays reference

		if ((SetTD('a', daca)) || (SetTD('b', reference)))
		{
			cerr << "There was a problem setting reference. Program will terminate!" << endl;
			return 1;
		}

		while (-duration < encoder2)
		{
			//Read values from encoder
			if (GetTimers(encoder1, encoder2))
			{
				cerr << "There was a problem reading the encoders, program will terminate!" << endl;
				Stop();
				return 1;
			}

			//double-check loop condition
			if (-duration > encoder2)
			{
				break;
			}
		}
	}
	else
	{
		//Undefined direction
		Stop();
		cerr << "Undefined direction, program will terminate" << endl;
		return 1;
	}

	Stop();

	return 0;
}

//==========================================================
// GetDistance() takes in the x and y coordinates and then
// returns the distance that should be travelled by the
// WMRA. 
//==========================================================
double GetDistance (double x, double y)
{
	double distance; //Final distance value

	//Get the absolute value (in case values are negative)
	x = abs(x);
	y = abs(y);

	//Calculate x^2 and y^2
	x = pow(x, 2);
	y = pow(y, 2);

	//Distance formula
	distance = sqrt(x + y);

	return distance;
}

//==========================================================
// GetAngle() takes in the x and y coordinates and then
// returns the angle that the WMRA needs to rotate in order
// to orient itself in a straight line for the destination.
//==========================================================
double GetAngle (double x, double y)
{
	double angle; //Final angle value
	double distance = GetDistance (x, y);

	if (x == 0) //On x-axis axis
	{
		if (y < 0)
		{
			angle = 1.57;
		}
		else if (y > 0)
		{
			angle = -1.57;
		}
		else
		{
			angle = 0;
		}
	}
	else if (y == 0) //On y-axis
	{
		if (x < 0)
		{
			angle = 3.14;
		}
		else
		{
			angle = 0;
		}
	}
	else if (x > 0 && y < 0) //Quadrant I
	{
		angle = asin((y*-1) / distance);
	}
	else if (x > 0 && y > 0) //Quadrant II
	{
		angle = asin(y / distance);
		angle = angle * -1; //Go left
	}
	else if (x < 0 && y > 0) //Quadrant III
	{
		angle = 1.57 + asin((x*-1) / distance);
		angle = angle * -1; //Go left
	}
	else if (x < 0 && y < 0) //Quadrant IV
	{
		angle = 1.57 + asin((x*-1) / distance);
	}
	else
	{
		angle = 0;
		cerr << "There was an error in GetAngle() calculating the angle!" << endl;
	}

	//At this point, angle is in radians!!!
	angle = angle * 57.2957795; //Convert from radians to degrees

	return angle;
}

//==========================================================
// Go() takes in x and y coordinates and angle and then 
// moves the WMRA. The angle specifies the final orientation
// of the WMRA. It first orients it to the correct angle, 
// then moves straight for the distance, then orients to the
// specified final angle. Input for coordinates are in 
// encoder counts. Angle is in degrees, converted to encoder
// counts inside the function. Final angle is with respect
// to the initial WMRA position. Returns 0 for normal
// execution, 1 for error condition.
//==========================================================
int Go (double x, double y, double finalAngle)
{
	double angle, distance;

	//Get distance for (x,y)
	distance = GetDistance (x, y);

	//Get angle for (x,y)
	angle = GetAngle (x, y);

	//Convert angle from degrees to encoder count
	angle = angle * 40;

	//Convert finalAngle from degrees to encoder count
	finalAngle = finalAngle * 40;

	if (angle < 0) //Counter-clockwise motion
	{
		angle = angle * -1;
		//cout << "Angle is " << angle << endl;
		Move ("left", SPEED, angle);
	}
	else //(angle > 0) //Clockwise motion
	{
		//cout << "Angle is " << angle << endl;
		Move ("right", SPEED, angle);
	}

	//WMRA is now oriented correctly

	//Convert from mm distance to encoder counts
	distance = distance * 0.486;

	//Move forward for distance
	Move ("forward", SPEED, distance);

	//Move WMRA to final orientation specified by angle
	if (finalAngle < 0) //Counter-clockwise motion
	{
		finalAngle = finalAngle * -1;
		Move ("left", SPEED, finalAngle);
	}
	if (finalAngle > 0) //Clockwise motion
	{
		Move ("right", SPEED, finalAngle);
	}

	return 0;
}

//==========================================================
// GoAvoid() takes in x and y coordinates and angle and then 
// moves the WMRA. The angle specifies the final orientation
// of the WMRA. It first orients it to the correct angle, 
// then moves straight for the distance, then orients to the
// specified final angle. Input for coordinates are in 
// encoder counts. Angle is in degrees, converted to encoder
// counts inside the function. Final angle is with respect
// to the initial WMRA position. GoAvoid also reads the 
// proximity sensors and avoids obstacles. Returns 0 for 
// normal execution, 1 for error condition.
//==========================================================
int GoAvoid (double x, double y, double finalAngle)
{
	double reference = 6;
	double encoder1 = 0;
	double encoder2 = 0;
	double daca, dacb;
	double offset = AUTSPEED; //SPEED!
	double angle, distance;
	double i1, i2, i3, i4; //proximity sensors
	double skid1 = 0, skid2 = 0; //skid calculation for updating point in forward obstacle avoidance

	//Retrieve current reference value
	reference = GetReference();

	//TIMER SETUP
	if (InitializeTimers())
	{
		cerr << "There was a problem initializing the timers, program will terminate!" << endl;
		Stop();
		return 1;
	}

	encoder1 = 0;
	encoder2 = 0;
	cout << "Encoders initialized: " << encoder1 << ", " << encoder2 << endl;

	//Get distance for (x,y)
	distance = GetDistance (x, y);

	//Get angle for (x,y)
	angle = GetAngle (x, y);

	//Convert angle from degrees to encoder count
	angle = angle * 40;

	//Convert finalAngle from degrees to encoder count
	finalAngle = finalAngle * 40;

	if (angle < 0) //Counter-clockwise motion
	{
		angle = angle * -1;
		//cout << "Angle is " << angle << endl;

		//LEFT motion
		cout << "Moving to the left" << endl;
		//DACA (blue) is reference decremented by offset
		daca = reference - offset;
		//DACB (yellow) stays reference

		if ((SetTD('a', daca)) || (SetTD('b', reference)))
		{
			cerr << "There was a problem setting reference. Program will terminate!" << endl;
			return 1;
		}

		while (angle > encoder1)
		{
			//Read values from encoder
			if (GetTimers(encoder1, encoder2))
			{
				cerr << "There was a problem reading the encoders, program will terminate!" << endl;
				Stop();
				return 1;
			}

			//Get the analog voltage values from the proximity sensors
			if ((GetAIN(3, i1)) || (GetAIN(4, i2)) || (GetAIN(5, i3)) || (GetAIN(6, i4)))
			{
				cerr << "There was a problem reading the infrared proximity sensors. Program will terminate!" << endl;
				return 1;
			}

			//Monitor proximity sensors for obstacles
			if (i1 > PROXIMITY) //Obstacle on left side of wheelchair
			{
				cout << "Obstacle on the left!" << endl;
				Stop();

				//TIMER SETUP
				if (InitializeTimers())
				{
					cerr << "There was a problem initializing the timers, program will terminate!" << endl;
					Stop();
					return 1;
				}

				encoder1 = 0;
				encoder2 = 0;
				cout << "Encoders initialized: " << encoder1 << ", " << encoder2 << endl;

				if ((i2 > PROXIMITY) || (i3 > PROXIMITY)) //Check to see if obstacle in front
				{
					//move backwards until clear on left
					dacb = reference - offset;

					if ((SetTD('a', reference)) || (SetTD('b', dacb)))
					{
						cerr << "There was a problem setting motion. Program will terminate!" << endl;
						return 1;
					}

					cout << "Moving backwards until clear" << endl;

					while (i1 > (PROXIMITY * 0.7))
					{
						if (GetAIN(3, i1))
						{
							cerr << "There was a problem reading i1 proximity sensor. Program will terminate!" << endl;
							Stop();
							return 1;
						}

						//Read values from encoder
						if (GetTimers(encoder1, encoder2))
						{
							cerr << "There was a problem reading the encoders, program will terminate!" << endl;
							Stop();
							return 1;
						}
					}

					Stop();
				}
				else //Front region is clear
				{
					//move forward until clear on left
					dacb = reference + offset;

					if ((SetTD('a', reference)) || (SetTD('b', dacb)))
					{
						cerr << "There was a problem setting motion. Program will terminate!" << endl;
						return 1;
					}

					cout << "Moving forwards until clear" << endl;

					while (i1 > (PROXIMITY * 0.7))
					{
						if (GetAIN(3, i1))
						{
							cerr << "There was a problem reading i1 proximity sensor. Program will terminate!" << endl;
							Stop();
							return 1;
						}

						//Read values from encoder
						if (GetTimers(encoder1, encoder2))
						{
							cerr << "There was a problem reading the encoders, program will terminate!" << endl;
							Stop();
							return 1;
						}
					}

					Stop();
				}

				cout << "Obstacle has been cleared, breaking!" << endl;

				if (encoder1 < 0)
				{
					//backwards motion
					encoder1 = encoder1 * -1; //make e1 positive
					GoAvoid(x, y-(((encoder1+encoder2)/2)*2.0576), 0);
				}
				else if (encoder2 < 0)
				{
					//forward motion
					encoder2 = encoder2 * -1; //make e2 positive
					GoAvoid(x, y+(((encoder1+encoder2)/2)*2.0576), 0);
				}

				break;
			}

			//double-check loop condition
			if (angle < encoder1)
			{
				break;
			}
		}

	}
	else //(angle > 0) //Clockwise motion
	{
		//cout << "Angle is " << angle << endl;
		
		//RIGHT motion
		//DACA (blue) is reference incremented by offset
		daca = reference + offset;
		//DACB (yellow) stays reference

		if ((SetTD('a', daca)) || (SetTD('b', reference)))
		{
			cerr << "There was a problem setting reference. Program will terminate!" << endl;
			return 1;
		}

		while (-angle < encoder2)
		{
			//Read values from encoder
			if (GetTimers(encoder1, encoder2))
			{
				cerr << "There was a problem reading the encoders, program will terminate!" << endl;
				Stop();
				return 1;
			}

			//Get the analog voltage values from the proximity sensors
			if ((GetAIN(3, i1)) || (GetAIN(4, i2)) || (GetAIN(5, i3)) || (GetAIN(6, i4)))
			{
				cerr << "There was a problem reading the infrared proximity sensors. Program will terminate!" << endl;
				return 1;
			}

			//Monitor proximity sensors for obstacles
			if (i4 > PROXIMITY) //Obstacle on right side of wheelchair
			{
				cerr << "Obstacle on the right!" << endl;
				Stop();

				//TIMER SETUP
				if (InitializeTimers())
				{
					cerr << "There was a problem initializing the timers, program will terminate!" << endl;
					Stop();
					return 1;
				}

				encoder1 = 0;
				encoder2 = 0;
				cout << "Encoders initialized: " << encoder1 << ", " << encoder2 << endl;

				if ((i2 > PROXIMITY) || (i3 > PROXIMITY)) //Check to see if obstacle in front
				{
					//move backwards until clear on right
					dacb = reference - offset;

					if ((SetTD('a', reference)) || (SetTD('b', dacb)))
					{
						cerr << "There was a problem setting motion. Program will terminate!" << endl;
						return 1;
					}

					cout << "Moving backwards until clear" << endl;

					while (i4 > (PROXIMITY * 0.7))
					{
						if (GetAIN(6, i4))
						{
							cerr << "There was a problem reading i4 proximity sensor. Program will terminate!" << endl;
							Stop();
							return 1;
						}

						//Read values from encoder
						if (GetTimers(encoder1, encoder2))
						{
							cerr << "There was a problem reading the encoders, program will terminate!" << endl;
							Stop();
							return 1;
						}
					}

					Stop();
				}
				else //Front region is clear
				{
					//move forward until clear on right
					dacb = reference + offset;

					if ((SetTD('a', reference)) || (SetTD('b', dacb)))
					{
						cerr << "There was a problem setting motion. Program will terminate!" << endl;
						return 1;
					}

					cout << "Moving forward until clear" << endl;

					while (i4 > (PROXIMITY * 0.7))
					{
						if (GetAIN(6, i4))
						{
							cerr << "There was a problem reading i4 proximity sensor. Program will terminate!" << endl;
							Stop();
							return 1;
						}

						//Read values from encoder
						if (GetTimers(encoder1, encoder2))
						{
							cerr << "There was a problem reading the encoders, program will terminate!" << endl;
							Stop();
							return 1;
						}
					}

					Stop();
				}

				cout << "Obstacle has been cleared, breaking!" << endl;

				if (encoder1 < 0)
				{
					//backwards motion
					encoder1 = encoder1 * -1; //make e1 positive
					GoAvoid(x, y-(((encoder1+encoder2)/2)*2.0576), 0);
				}
				else if (encoder2 < 0)
				{
					//forward motion
					encoder2 = encoder2 * -1; //make e2 positive
					GoAvoid(x, y+(((encoder1+encoder2)/2)*2.0576), 0);
				}

				break;
			}

			//double-check loop condition
			if (-angle > encoder2)
			{
				break;
			}
		}

	}

	//WMRA is now oriented correctly
	cout << "Now oriented to correct angle, moving forward" << endl;

	//Reset timers for forward motion
	if (InitializeTimers())
	{
		cerr << "There was a problem initializing the timers, program will terminate!" << endl;
		Stop();
		return 1;
	}

	encoder1 = 0;
	encoder2 = 0;
	cout << "Encoders initialized: " << encoder1 << ", " << encoder2 << endl;

	//Convert from mm distance to encoder counts
	distance = distance * 0.486;

	//Move forward for distance
	//FORWARD motion
	//DACA (blue) stays reference
	//DACB (yellow) is reference incremented by offset
	dacb = reference + offset;

	if ((SetTD('a', reference)) || (SetTD('b', dacb)))
	{
		cerr << "There was a problem setting reference. Program will terminate!" << endl;
		return 1;
	}

	while ((distance > encoder1) && (-distance < encoder2))
	{
		//Read values from encoders
		if (GetTimers(encoder1, encoder2))
		{
			cerr << "There was a problem reading the encoders, program will terminate!" << endl;
			Stop();
			return 1;
		}

		//Get the analog voltage values from the proximity sensors
		if ((GetAIN(3, i1)) || (GetAIN(4, i2)) || (GetAIN(5, i3)) || (GetAIN(6, i4)))
		{
			cerr << "There was a problem reading the infrared proximity sensors. Program will terminate!" << endl;
			return 1;
		}

		//Monitor proximity sensors for obstacles
		if ((i2 > PROXIMITY) || (i3 > PROXIMITY)) //Obstacle in front of wheelchair
		{
			cerr << "Obstacle forward!" << endl;

			if (((i2 > PROXIMITY) && (i3 > PROXIMITY)) || ((i2 > PROXIMITY) || (i1 > PROXIMITY)))
			{
				//shift right, then forward

				//RIGHT motion
				//DACA (blue) is reference incremented by offset
				daca = reference + offset;
				//DACB (yellow) stays reference

				if ((SetTD('a', daca)) || (SetTD('b', reference)))
				{
					cerr << "There was a problem setting reference. Program will terminate!" << endl;
					return 1;
				}

				//Reset timers for forward motion
				if (InitializeTimers())
				{
					cerr << "There was a problem initializing the timers, program will terminate!" << endl;
					Stop();
					return 1;
				}

				encoder1 = 0;
				encoder2 = 0;
				cout << "Encoders initialized: " << encoder1 << ", " << encoder2 << endl;

				cout << "Rotating 90 degrees to the right" << endl;

				while (-3600 < encoder1)
				{
					//Read values from encoder
					if (GetTimers(encoder1, encoder2))
					{
						cerr << "There was a problem reading the encoders, program will terminate!" << endl;
						Stop();
						return 1;
					}
					//cout << "e1: " << encoder1 << ", e2: " << encoder2 << ", -3600<e1" << endl;
					if ((-3600 > encoder1) || (-3600 > encoder2))
					{
						break;
					}
				}

				Stop();

				//go forward until left side (i1) is clear of obstacle
				//move forward until clear on right
				dacb = reference + offset;

				if ((SetTD('a', reference)) || (SetTD('b', dacb)))
				{
					cerr << "There was a problem setting motion. Program will terminate!" << endl;
					return 1;
				}

				//Reset timers for forward motion
				if (InitializeTimers())
				{
					cerr << "There was a problem initializing the timers, program will terminate!" << endl;
					Stop();
					return 1;
				}

				encoder1 = 0;
				encoder2 = 0;
				cout << "Encoders initialized: " << encoder1 << ", " << encoder2 << endl;

				cout << "Moving forward until left side (i1) is clear of obstacle" << endl;

				while ((i1 > (PROXIMITY * 0.7)) || ((2000 > encoder1) && (-2000 < encoder2)))
				{
					if (GetAIN(3, i1))
					{
						cerr << "There was a problem reading i1 proximity sensor. Program will terminate!" << endl;
						Stop();
						return 1;
					}

					//Read values from encoder
					if (GetTimers(encoder1, encoder2))
					{
						cerr << "There was a problem reading the encoders, program will terminate!" << endl;
						Stop();
						return 1;
					}
				}

				Stop();

				//save the values of the encoders during the skid motion
				encoder1 = skid1;
				encoder2 = skid2;

				//rotate 90 degrees to the left for original orientation
				//LEFT motion
				//DACA (blue) is reference decremented by offset
				daca = reference - offset;
				//DACB (yellow) stays reference

				if ((SetTD('a', daca)) || (SetTD('b', reference)))
				{
					cerr << "There was a problem setting reference. Program will terminate!" << endl;
					return 1;
				}

				//Reset timers for forward motion
				if (InitializeTimers())
				{
					cerr << "There was a problem initializing the timers, program will terminate!" << endl;
					Stop();
					return 1;
				}

				encoder1 = 0;
				encoder2 = 0;
				cout << "Encoders initialized: " << encoder1 << ", " << encoder2 << endl;

				cout << "Rotating 90 degrees to the left" << endl;

				while (3600 > encoder1) //rotate left 90 degrees
				{
					//Read values from encoder
					if (GetTimers(encoder1, encoder2))
					{
						cerr << "There was a problem reading the encoders, program will terminate!" << endl;
						Stop();
						return 1;
					}
					//cout << "e1: " << encoder1 << ", e2: " << encoder2 << ", 3600>e1" << endl;
					if ((3600 < encoder1) || (3600 < encoder2))
					{
						break;
					}
				}

				Stop();

				cout << "Obstacle has been cleared, breaking!" << endl;

				skid2 = skid2 * -1; //make skid2 positive
				GoAvoid(x-(((skid1+skid2)/2)*2.0576), y, 0);
			}
			else if ((i3 > PROXIMITY) || (i4 > PROXIMITY))
			{
				//shift to the left, then forward

				//LEFT motion
				//DACA (blue) is reference decremented by offset
				daca = reference - offset;
				//DACB (yellow) stays reference

				if ((SetTD('a', daca)) || (SetTD('b', reference)))
				{
					cerr << "There was a problem setting reference. Program will terminate!" << endl;
					return 1;
				}

				//Reset timers for forward motion
				if (InitializeTimers())
				{
					cerr << "There was a problem initializing the timers, program will terminate!" << endl;
					Stop();
					return 1;
				}

				encoder1 = 0;
				encoder2 = 0;
				cout << "Encoders initialized: " << encoder1 << ", " << encoder2 << endl;

				cout << "Rotating 90 degrees to the left" << endl;

				while (3600 > encoder1) //rotate left 90 degrees
				{
					//Read values from encoder
					if (GetTimers(encoder1, encoder2))
					{
						cerr << "There was a problem reading the encoders, program will terminate!" << endl;
						Stop();
						return 1;
					}
					//cout << "e1: " << encoder1 << ", e2: " << encoder2 << ", 3600>e1" << endl;
					if ((3600 < encoder1) || (3600 < encoder2))
					{
						break;
					}
				}

				Stop();

				//go forward until right side (i4) is clear of obstacle
				//move forward until clear on right
				dacb = reference + offset;

				if ((SetTD('a', reference)) || (SetTD('b', dacb)))
				{
					cerr << "There was a problem setting motion. Program will terminate!" << endl;
					return 1;
				}

				//Reset timers for forward motion
				if (InitializeTimers())
				{
					cerr << "There was a problem initializing the timers, program will terminate!" << endl;
					Stop();
					return 1;
				}

				encoder1 = 0;
				encoder2 = 0;
				cout << "Encoders initialized: " << encoder1 << ", " << encoder2 << endl;

				cout << "Moving forward until right side (i4) is clear of obstacle" << endl;

				while ((i4 > (PROXIMITY * 0.7)) || ((2000 > encoder1) && (-2000 < encoder2)))
				{
					if (GetAIN(6, i4))
					{
						cerr << "There was a problem reading i4 proximity sensor. Program will terminate!" << endl;
						Stop();
						return 1;
					}

					//Read values from encoder
					if (GetTimers(encoder1, encoder2))
					{
						cerr << "There was a problem reading the encoders, program will terminate!" << endl;
						Stop();
						return 1;
					}
				}

				Stop();

				//save the values of the encoders during the skid motion
				encoder1 = skid1;
				encoder2 = skid2;

				//rotate 90 degrees to the right for original orientation
				//RIGHT motion
				//DACA (blue) is reference incremented by offset
				daca = reference + offset;
				//DACB (yellow) stays reference

				if ((SetTD('a', daca)) || (SetTD('b', reference)))
				{
					cerr << "There was a problem setting reference. Program will terminate!" << endl;
					return 1;
				}

				//Reset timers for forward motion
				if (InitializeTimers())
				{
					cerr << "There was a problem initializing the timers, program will terminate!" << endl;
					Stop();
					return 1;
				}

				encoder1 = 0;
				encoder2 = 0;
				cout << "Encoders initialized: " << encoder1 << ", " << encoder2 << endl;

				cout << "Rotating 90 degrees to the right" << endl;

				while (-3600 < encoder1)
				{
					//Read values from encoder
					if (GetTimers(encoder1, encoder2))
					{
						cerr << "There was a problem reading the encoders, program will terminate!" << endl;
						Stop();
						return 1;
					}
					//cout << "e1: " << encoder1 << ", e2: " << encoder2 << ", -3600<e2" << endl;
					if ((-3600 > encoder1) || (-3600 > encoder2))
					{
						break;
					}
				}

				Stop();

				cout << "Obstacle has been cleared, breaking!" << endl;

				skid2 = skid2 * -1; //make skid2 positive
				GoAvoid(x+(((skid1+skid2)/2)*2.0576), y, 0);
			}

			break;
		}

		//double-check loop condition
		if ((distance < encoder1) && (-distance > encoder2))
		{
			break;
		}
	}

	//Move WMRA to final orientation specified by angle
	if (finalAngle < 0) //Counter-clockwise motion
	{
		finalAngle = finalAngle * -1;

		//Move ("left", 4, finalAngle);

		//LEFT motion
		//DACA (blue) is reference decremented by offset
		daca = reference - offset;
		//DACB (yellow) stays reference

		if ((SetTD('a', daca)) || (SetTD('b', reference)))
		{
			cerr << "There was a problem setting reference. Program will terminate!" << endl;
			return 1;
		}

		while (finalAngle > encoder1)
		{
			//Read values from encoder
			if (GetTimers(encoder1, encoder2))
			{
				cerr << "There was a problem reading the encoders, program will terminate!" << endl;
				Stop();
				return 1;
			}

			//double-check loop condition
			if (finalAngle < encoder1)
			{
				break;
			}
		}

	}
	if (finalAngle > 0) //Clockwise motion
	{
		//Move ("right", 4, finalAngle);

		//RIGHT motion
		//DACA (blue) is reference incremented by offset
		daca = reference + offset;
		//DACB (yellow) stays reference

		if ((SetTD('a', daca)) || (SetTD('b', reference)))
		{
			cerr << "There was a problem setting reference. Program will terminate!" << endl;
			return 1;
		}

		while (-finalAngle < encoder2)
		{
			//Read values from encoder
			if (GetTimers(encoder1, encoder2))
			{
				cerr << "There was a problem reading the encoders, program will terminate!" << endl;
				Stop();
				return 1;
			}

			//double-check loop condition
			if (-finalAngle > encoder2)
			{
				break;
			}
		}

	}

	Stop();
	return 0;
}