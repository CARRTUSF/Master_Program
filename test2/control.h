/*
 * control.h
 *
 *  Created on: May 19, 2009
 *      Author: USF WMRA
 *
 *      This control.h file includes the function prototypes
 *      that are described in control.cpp. We include
 *      control.h in the control.cpp file, this way we
 *      can include control.h in another .cpp file and use
 *      these functions.
 */

#ifndef CONTROL_H //these statements define control.h
#define CONTROL_H // if control.h has not been defined

#include <string> //string library includes string types and functions
#include <iostream> //iostream for input/output
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Galil.h"
//#include "Galil.h" //we include the Galil library to use g.connection() f'ns

using namespace std; //we use namespace to keep things simple

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


//individual assignment of motor parameters

void Decceleration_all (long int dvalue, Galil &g);
void Define_Position (char motor, long int defvalue,Galil &g);

//position controll
void Pos_track (Galil &g);
void Time_constant (float tvalue, Galil &g);
void Position_control(char motor,long int p_value,long int v_value, long int a_value,Galil &g);
void Position_control_2(long int aP, long int bP, long int cP, long int dP, long int eP, long int fP, long int gP, long int v_value, int choice, Galil &g);

// velocity control
void Velocity_stop(char motor, Galil &g);
void Velocity_all_stop(Galil &g);
void Velocity_control(char motor, long int v_value, long int a_value, long int d_value, Galil &g);


//PID gain assignment


void PID(char PID_motor, float KP_value,float KD_value,float KI_value, Galil &g);
//void PID_all(float KP_all_value,float KD_all_value,float KI_all_value, Galil &g);


//delay untill motion complete
//void Wait_to_complete(char motor, Galil &g);

//AMP-20540 setings
void AMP20540_Current_loop_gain(char motor,double n, Galil &g);
void AMP20540_current_loop_gain_return(char motor, Galil &g);
void AMP20540_gain(char motor, int gain, Galil &g);
void AMP20540_brush_axis(Galil &g);
void Brushed_Motor (char motor, int defvalue,Galil &g);

void Wait_tomove (char motor, long int defvalue,Galil &g);


//Data Acquisition from controller

float Tell_pos(char motor, Galil &g);
//float Tell_pos_all(Galil &g);

float Tell_pos_er(char motor, Galil &g);
//float Tell_pos_err_all(Galil &g);



#endif /* CONTROL_H_ */
