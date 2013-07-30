/*
 * wmraMotion.h
 *
 *  Created on: August 2, 2010
 *      Author: USF CARRT (WMRA Project)
 *
 *      This wmraMotion.h file includes all of the functions that
 *		are used to control the motion of the wheelchair via
 *		PC using the Lab Jack U6 device through USB. The 
 *		Lab Jack device must be connected to the computer through
 *		USB. The Lab Jack driver files must be installed in 
 *		order for this library to work. The driver files are 
 *		included and used in this library. 
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

#ifndef WMRAMOTION_H //these statements define wmraMotion.h
#define WMRAMOTION_H // if wmraMotion.h has not been defined

// YOU MUST HAVE THE FILE BELOW INSTALLED ON THE PC RUNNING THIS
// PROGRAM. YOU MUST ALSO HAVE THE labjackud.lib FILE INCLUDED IN
// THE PROJECT BY SPECIFYING THE LOCATION ON YOUR PC. RIGHT-CLICK
// THE PROJECT AND GO TO ADD->EXISTING ITEM. BROWSE TO THE 
// labjackud.lib FILE WHICH SHOULD BE LOCATED AT 
// C:\Program Files\LabJack\Drivers BY DEFAULT WHEN THE LAB JACK
// DRIVER IS INSTALLED. THE FILE LabJackUD.h MUST BE LOCATED AT 
// THE DIRECTORY AS SPECIFIED DIRECTLY BELOW. 
// If Visual Studio complains (it does this a lot) after you add 
// the labjackud.lib file to this project, just click No. We are
// only referencing the library file as per LabJackUD.h, not 
// editing or building it. 
#include "c:\program files\labjack\drivers\LabJackUD.h"
// It is probably a good idea to go ahead and right-click Header
// Files and Add->Existing Item and then add LabJackUD.h as well.
// LabJackUD.h is located in the same Drivers directory mentioned
// above.

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
void ErrorHandler (LJ_ERROR lngErrorcode, long lngLineNumber, long lngIteration);

//==========================================================
// Initialize() goes through the rounds of finding and 
// opening the Lab Jack U6. You must uses this Initialize()
// function before using any other functions. Returns 1 for
// error condition, 0 for proper execution.
//==========================================================
int Initialize ();

//==========================================================
// SetTD sets the analog output for DACA and DACB on the LJ
// Tick DAC (+/- 10V). Channel specifies the channel to set
// (A,B) and value is the analog voltage output. Value must 
// be 0-10 (volts). Returns 1 for error condition, 0 for 
// proper execution.
//==========================================================
int SetTD (char channel, double value);

//==========================================================
// SetDAC sets the analog output for DAC0 and DAC1 (+/- 5V). 
// Channel specifies the channel to set (0,1) and value is 
// the analog voltage output. Value must be 0-5 (volts). 
// Returns 1 for error condition, 0 for proper execution.
//==========================================================
int SetDAC (int channel, double value);

//==========================================================
// GetAIN gets the analog input for AIN0 through AIN13 
// (+/- 10V). Channel specifies the channel to read (0-13) 
// and value (analog input) is passed and updated. Returns 1 
// for error condition, 0 for proper execution.
//==========================================================
int GetAIN (int channel, double &value);

//==========================================================
// SetDO sets the digital output for FIO0 through MIO2 
// (3.3/5V). Channel specifies the channel to set (0-22) 
// and value is the desired output (1=high, 0=low). Returns 
// 1 for error condition, 0 for proper execution.
//==========================================================
int SetDO (int channel, long value);

//==========================================================
// GetDI gets the digital input for FIO0 through MIO2 
// (3.3/5V). Channel specifies the channel to read (0-22) 
// and value is the digital input (1=high, 0=low). Returns 
// 1 for error condition, 0 for proper execution.
//==========================================================
int GetDI (int channel, long &value);

//==========================================================
// StartStream sets up the stream to read the sonar 
// proximity sensor distance. Stream is configured and 
// started, then GetStreamDistance() is used to get the 
// distance. Returns 1 for error condition, 0 for proper
// execution.
//==========================================================
int StartStream ();

//==========================================================
// GetStreamDistance reads the sonar proximity sensor 
// distance in cm. Sonar sensor signal wire is on EIO0 for
// S1 and EIO1 for S2. StartStream() must be called before 
// GetStreamDistance. Updates s1 and s2 with distances in
// cm. Returns 1 for error condition, 0 for proper 
// execution.
//==========================================================
int GetStreamDistance (double &s1, double &s2);

//==========================================================
// StopStream stops the stream reading the sonar proximity
// sensors. Stream should be stopped after the application 
// is done reading distance values. Returns 1 for error 
// condition, 0 for proper execution.
//==========================================================
int StopStream ();

//==========================================================
// InitializeTimers configures and starts the timers for 
// encoder 1 and 2. Note that when this function is called,
// timers are reset. Returns 1 for error condition, 0 for 
// proper execution.
//==========================================================
int InitializeTimers ();

//==========================================================
// GetTimers gets the timer values for encoder 1 and 2. The
// parameters are passed into the function and updated. 
// Returns 1 for error condition, 0 for proper execution.
//==========================================================
int GetTimers (double &encoder1, double &encoder2);

//==========================================================
// GetReference() reads in the analog input value for 
// reference from the joystick (green wire). This will give
// us the proper value to set the analog outputs for an idle
// situation. It returns the value of the reference, which
// should be close to 6V. Returns 6 on error condition.
//==========================================================
double GetReference ();

//==========================================================
// Stop() is a function that stops the wheelchair motion and
// sets it to an idle condition. It uses GetReference() to
// find the reference value (~6V) and then sets the analog
// outputs to this value for the idle condition. Returns 1
// for error condition, 0 for proper execution.
//==========================================================
int Stop ();

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
int Move (string direction, double speed, double duration);

//==========================================================
// GetDistance() takes in the x and y coordinates and then
// returns the distance that should be travelled by the
// WMRA. 
//==========================================================
double GetDistance (double x, double y);

//==========================================================
// GetAngle() takes in the x and y coordinates and then
// returns the angle that the WMRA needs to rotate in order
// to orient itself in a straight line for the destination.
//==========================================================
double GetAngle (double x, double y);

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
int Go (double x, double y, double finalAngle);

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
int GoAvoid (double x, double y, double finalAngle);

#endif /* WMRAMOTION_H_ */
