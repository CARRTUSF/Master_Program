#include <stdio.h>

//Servo Module Command set:
#define	RESET_POS	  0x00	//Reset encoder counter to 0 (0 bytes)
#define	SET_ADDR	  0x01	//Set address and group address (2 bytes)
#define	DEF_STAT	  0x02	//Define status items to return (1 byte)
#define	READ_STAT	  0x03	//Read value of current status items
#define	LOAD_TRAJ  	  0x04	//Load trahectory date (1 - 14 bytes)
#define START_MOVE	  0x05	//Start pre-loaded trajectory (0 bytes)
#define SET_GAIN	  0x06  //Set servo gain and control parameters (13 or 14)
#define	STOP_MOTOR 	  0x07	//Stop motor (1 byte)
#define	IO_CTRL		  0x08	//Define bit directions and set output (1 byte)
#define SET_HOMING	  0x09  //Define homing mode (1 byte)
#define	SET_BAUD	  0x0A 	//Set the baud rate (1 byte)
#define CLEAR_BITS	  0x0B  //Save current pos. in home pos. register (0 bytes)
#define SAVE_AS_HOME  0x0C	//Store the current home position in home position register
#define ADD_PATHPOINT 0x0D  //Adds path points for path mode
#define	NOP			  0x0E	//No operation - returns prev. defined status (0 bytes)
#define HARD_RESET	  0x0F	//RESET - no status is returned

//Servo Module RESET_POS control byte bit definitions:
//(if no control byte is used, reset is absolute)
#define REL_HOME 	  0x01	//Reset position relative to current home position
#define SET_POS 	  0x02	//Set the position to a specific value (v10 & >)

//Servo Module STATUSITEMS bit definitions (for DEF_STAT and READ_STAT):
#define	SEND_POS	  0x01	//4 bytes data
#define	SEND_AD		  0x02	//1 byte
#define	SEND_VEL	  0x04	//2 bytes
#define SEND_AUX	  0x08	//1 byte
#define SEND_HOME	  0x10	//4 bytes
#define SEND_ID		  0x20	//2 bytes
#define SEND_PERROR	  0x40  //2 bytes
#define SEND_NPOINTS  0x80  //1 byte

//Servo Module LOAD_TRAJ control byte bit definitions:
#define	LOAD_POS	  0x01	//+4 bytes
#define LOAD_VEL	  0x02	//+4 bytes
#define	LOAD_ACC	  0x04	//+4 bytes
#define LOAD_PWM	  0x08	//+1 byte
#define ENABLE_SERVO  0x10  //1 = servo mode, 0 = PWM mode
#define VEL_MODE	  0x20	//1 = velocity mode, 0 = trap. position mode
#define REVERSE		  0x40  //1 = command neg. PWM or vel, 0 = positive
#define MOVE_REL	  0x40  //1 = move relative, 0 = move absolute
#define START_NOW	  0x80  //1 = start now, 0 = wait for START_MOVE command

//Servo Module STOP_MOTOR control byte bit definitions:
#define	AMP_ENABLE	  0x01	//1 = raise amp enable output, 0 = lower amp enable
#define MOTOR_OFF	  0x02	//set to turn motor off
#define STOP_ABRUPT   0x04	//set to stop motor immediately
#define STOP_SMOOTH	  0x08  //set to decellerate motor smoothly
#define STOP_HERE	  0x10	//set to stop at position (4 add'l data bytes required)
#define ADV_FEATURE   0x20  //enable features in ver. CMC

//Servo Module IO_CTRL control byte bit definitions:
#define SET_OUT1	  0x01	//1 = set limit 1 output, 0 = clear limit 1 output
#define SET_OUT2	  0x02	//1 = set limit 2 output, 0 = clear limit 1 output
#define IO1_IN		  0x04	//1 = limit 1 is an input, 0 = limit 1 is an output
#define IO2_IN		  0x08	//1 = limit 2 is an input, 0 = limit 1 is an output
#define LIMSTOP_OFF   0x04   //turn off motor on limit
#define LIMSTOP_ABRUPT 0x08  //stop abruptly on limit
#define THREE_PHASE	  0x10  //1 = 3-phase mode, 0 = single PWM channel
#define ANTIPHASE	  0x20  //1 = antiphase (0 = 50% duty cycle), 0 = PWM & dir
#define	FAST_PATH	  0x40  //0 = 30 or 60 Hz path execution, 1 = 60 or 120 Hz
#define STEP_MODE	  0x80  //0 = normal operation, 1 = Step & Direction enabled

//Servo Module SET_HOMING control byte bit definitions:
#define ON_LIMIT1	  0x01	//home on change in limit 1
#define ON_LIMIT2	  0x02	//home on change in limit 2
#define HOME_MOTOR_OFF  0x04  //turn motor off when homed
#define ON_INDEX	  0x08	//home on change in index
#define HOME_STOP_ABRUPT 0x10 //stop abruptly when homed
#define HOME_STOP_SMOOTH 0x20 //stop smoothly when homed
#define ON_POS_ERR	  0x40	//home on excessive position error
#define	ON_CUR_ERR	  0x80  //home on overcurrent error

//IO Module Command set:
#define	SET_IO_DIR	  0x00	//Set direction of IO bits (2 data bytes)
#define	SET_ADDR	  0x01	//Set address and group address (2 bytes)
#define	DEF_STAT	  0x02	//Define status items to return (1 byte)
#define	READ_STAT	  0x03	//Read value of current status items
#define	SET_PWM   	  0x04	//Immediatley set PWM1 and PWM2 (2 bytes)
#define SYNCH_OUT	  0x05	//Output prev. stored PWM & output bytes (0 bytes)
#define SET_OUTPUT	  0x06  //Immediately set output bytes
#define	SET_SYNCH_OUT 0x07	//Store PWM & outputs for synch'd output (4 bytes)
#define	SET_TMR_MODE  0x08	//Set the counter/timer mode (1 byte)
//Not used			  0x09
#define	SET_BAUD	  0x0A 	//Set the baud rate (1 byte)
//Not used			  0x0B
#define SYNCH_INPUT	  0x0C	//Store the input bytes and timer val (0 bytes)
//Not used			  0x0D
#define	NOP			  0x0E	//No operation - returns prev. defined status (0 bytes)
#define HARD_RESET	  0x0F	//RESET - no status is returned

//IO Module STATUSITEMS bit definitions
#define	SEND_INPUTS	  0x01	//2 bytes data
#define	SEND_AD1	  0x02	//1 byte
#define	SEND_AD2	  0x04	//1 byte
#define SEND_AD3	  0x08	//1 byte
#define SEND_TIMER	  0x10	//4 bytes
#define SEND_ID		  0x20	//2 bytes
#define	SEND_SYNC_IN  0x40	//2 bytes
#define	SEND_SYNC_TMR 0x80	//4 bytes

//IO Module Timer mode definitions
//Timer mode and resolution may be OR'd together
#define	OFFMODE		  0x00
#define	COUNTERMODE	  0x03
#define	TIMERMODE	  0x01
#define	RESx1		  0x00
#define RESx2		  0x10
#define RESx4		  0x20
#define RESx8		  0x30

//Servo Module ADD_PATHPOINT frequency definitions
#define P_30HZ		  30	//30 hz path resolution
#define P_60HZ		  60    //60 hs path resolution
#define P_120HZ		  120   //120 hs path resolution

//Servo Module HARD_RESET control byte bit definitions (v.10 and higher only):
#define	SAVE_DATA	  0x01	//save config. data in EPROM
#define RESTORE_ADDR  0x02  //restore addresses on power-up
#define EPU_AMP	      0x04  	//enable amplifier on power-up
#define EPU_SERVO     0x08  //enable servo
#define EPU_STEP	  0x10  //enable step & direction mode
#define EPU_LIMITS    0x20  //enable limit switch protection
#define EPU_3PH	      0x40  //enable 3-phase commutation
#define EPU_ANTIPHASE 0x80 //enable antiphase PWM

//Servo Module Status byte bit definitions:
#define MOVE_DONE	  0x01	//set when move done (trap. pos mode), when goal
							//vel. has been reached (vel mode) or when not servoing
#define CKSUM_ERROR	  0x02	//checksum error in received command
#define OVERCURRENT	  0x04	//set on overcurrent condition (sticky bit)
#define POWER_ON	  0x08	//set when motor power is on
#define POS_ERR		  0x10	//set on excess pos. error (sticky bit)
#define LIMIT1		  0x20	//value of limit 1 input
#define LIMIT2		  0x40	//value of limit 2 input
#define HOME_IN_PROG  0x80  //set while searching for home, cleared when home found

//Servo Module Auxilliary status byte bit definitions:
#define INDEX		  0x01	//value of the encoder index signal
#define POS_WRAP	  0x02	//set when 32 bit position counter wraps around
							//  (sticky bit)
#define SERVO_ON	  0x04	//set when position servo is operating
#define ACCEL_DONE	  0x08	//set when acceleration portion of a move is done
#define SLEW_DONE	  0x10  //set when slew portion of a move is done
#define SERVO_OVERRUN 0x20  //set if servo takes longer than the specified
							//servo period to execute
#define PATH_MODE	  0x40  //path mode is enabled (v.5)

#define MAXSIOERROR 2

//Define PIC baud rate divisors
#define	PB19200		64
#define	PB57600		21
#define	PB115200	10
#define	PB230400	5

//Module type definitions:
#define	SERVOMODTYPE	0
#define	ADCMODTYPE		1
#define	IOMODTYPE		2
#define	STEPMODTYPE		3

#define CKSUM_ERROR		0x02	//Checksum error bit in status byte

#define MAXNUMMOD	33

//Step Module Command set:
#define	RESET_POS	  0x00	//Reset encoder counter to 0 (0 bytes)
#define	SET_ADDR	  0x01	//Set address and group address (2 bytes)
#define	DEF_STAT	  0x02	//Define status items to return (1 byte)
#define	READ_STAT	  0x03	//Read value of current status items
#define	LOAD_TRAJ  	  0x04	//Load trajectory data
#define START_MOVE	  0x05	//Start pre-loaded trajectory (0 bytes)
#define SET_PARAM	  0x06  //Set operating parameters (6 bytes)
#define	STOP_MOTOR 	  0x07	//Stop motor (1 byte)
#define	SET_OUTPUTS	  0x08	//Set output bits (1 byte)
#define SET_HOMING	  0x09  //Define homing mode (1 byte)
#define	SET_BAUD	  0x0A 	//Set the baud rate (1 byte)
#define RESERVED	  0x0B  //
#define SAVE_AS_HOME  0x0C	//Store the current home position in home position register
#define NOT_USED	  0x0D
#define	NOP			  0x0E	//No operation - returns prev. defined status (0 bytes)
#define HARD_RESET	  0x0F	//RESET - no status is returned

//Step Module STATUSITEMS bit definitions:
#define	SEND_POS	  0x01	//4 bytes data
#define	SEND_AD		  0x02	//1 byte
#define	SEND_ST		  0x04	//2 bytes
#define SEND_INBYTE	  0x08	//1 byte
#define SEND_HOME	  0x10	//4 bytes
#define SEND_ID		  0x20	//2 bytes

//Step Module LOAD_TRAJ control byte bit definitions:
#define	LOAD_POS	  0x01	//+4 bytes
#define LOAD_SPEED	  0x02	//+1 bytes
#define	LOAD_ACC	  0x04	//+1 bytes
#define LOAD_ST		  0x08	//+3 bytes
#define STEP_REV      0x10  //reverse dir
#define START_NOW	  0x80  //1 = start now, 0 = wait for START_MOVE command

//Step Module SET_PARAM operating mode byte bit definitions:
#define	SPEED_8X	  0x00	//use 8x timing
#define	SPEED_4X	  0x01	//use 4x timing
#define	SPEED_2X	  0x02	//use 2x timing
#define	SPEED_1X	  0x03	//use 1x timing
#define IGNORE_LIMITS 0x04	//Do not stop automatically on limit switches
#define	IGNORE_ESTOP  0x08  //Do not stop automatically on e-stop
#define ESTOP_OFF  0x10  //Stop abrupt on estop or limit switch

//Step Module STOP_MOTOR control byte bit definitions:
#define	AMP_ENABLE	  0x01	//1 = raise amp enable output, 0 = lower amp enable
#define STOP_ABRUPT   0x04	//set to stop motor immediately
#define STOP_SMOOTH	  0x08  //set to decellerate motor smoothly

//Step Module SET_HOMING control byte bit definitions:
#define ON_LIMIT1	  0x01	//home on change in limit 1
#define ON_LIMIT2	  0x02	//home on change in limit 2
#define HOME_MOTOR_OFF 0x04  //turn motor off when homed
#define ON_HOMESW	  0x08	//home on change in index
#define HOME_STOP_ABRUPT 0x10 //stop abruptly when homed
#define HOME_STOP_SMOOTH 0x20 //stop smoothly when homed

//Step Module Status byte bit definitions:
#define MOTOR_MOVING  0x01	//Set when motor is moving
#define CKSUM_ERROR	  0x02	//checksum error in received command
#define AMP_ENABLED	  0x04	//set amplifier is enabled
#define POWER_ON	  0x08	//set when motor power is on
#define AT_SPEED	  0x10	//set when at command speed
#define VEL_MODE 	  0x20	//set when in velocity profile mode
#define TRAP_MODE	  0x40	//set when in trap. profile mode
#define HOME_IN_PROG  0x80  //set while searching for home, cleared when home found

//Step Module Input byte bit definitions:
#define	ESTOP		0x01	//emergency stop input
#define	AUX_IN1		0x02	//auxilliary input #1
#define	AUX_IN2		0x04	//auxilliary input #2
#define FWD_LIMIT	0x08	//forward limit switch
#define REV_LIMIT  	0x10	//reverse limit switch
#define HOME_SWITCH 0x20	//homing limit switch

typedef struct _NMCMOD
{
	byte	modtype;		//module type
    byte	modver;			//module version number
	byte	statusitems;	//definition of items to be returned
	byte	stat;  			//status byte
    byte	groupaddr;		//current group address
    BOOL	groupleader;	//TRUE if group leader
    void *	p;				//pointer to specific module's data structure
} NMCMOD;

typedef	struct _GAINVECT
{
    short int	kp;				//gain values
    short int	kd;
    short int	ki;
    short int	il;
    byte		ol;
    byte		cl;
    short int	el;
    byte		sr;
    byte		dc;
    byte		sm;
} GAINVECT;

typedef struct _SERVOMOD
{
    long		pos;     		//current position
    byte		ad;				//a/d value
    short int	vel;            //current velocity
    byte		aux;            //auxilliary status byte
    long		home;           //home position
    short int	perror;			//position error
    byte		npoints;		//number of points in path buffer

    //The following data is stored locally for reference
    long		cmdpos;			//last commanded position
    long		cmdvel;			//last commanded velocity
    long		cmdacc;			//last commanded acceleration
    byte		cmdpwm;			//last commanded PWM value
    GAINVECT    gain;
    long		stoppos;		//motor stop position (used by stop command)
    byte		stopctrl;		//stop control byte
    byte		movectrl;		//load_traj control byte
    byte		ioctrl;			//I/O control byte
    byte		homectrl;		//homing control byte
    long		last_ppoint;	//last path point specified
} SERVOMOD;

typedef struct _IOMOD
{
    short int	inbits;			//input bits
    byte		ad1;			//A/D input bytes
    byte		ad2;
    byte		ad3;
    unsigned long timer; 		//timer value
    short int	inbits_s;		//synchronized input bytes
    unsigned long timer_s;		//synchronized timer value
    //The following data is stored locally for reference
    byte		pwm1;			//current PWM output values
    byte		pwm2;
    byte		timermode;		//current timer mode
    short int	bitdir;			//current bit direction values
    short int	outbits;		//current output byte values
} IOMOD;

typedef struct _STEPMOD
{
    long		pos;     		//current position
    byte		ad;				//a/d value
    unsigned short int	st;          	//current step time
    byte		inbyte;         //input bits
    long		home;           //home position

    //The following data is stored locally for reference
    long		cmdpos;			//last commanded position
    byte		cmdspeed;		//last commanded speed
    byte		cmdacc;			//last commanded acceleration
    short int	cmdst;			//last commanded step time
    byte		min_speed;		//minimum running speed
    byte		stopctrl;		//stop control byte
    byte		outbyte;		//output bits
    byte		homectrl;		//homing control byte
    byte		ctrlmode;		//operating control mode byte
    byte		run_pwm;		//pwm for running current limit
    byte		hold_pwm;		//pwm for holding current limit
    byte		therm_limit;		//thermal limit
} STEPMOD;

typedef unsigned char byte;
NMCMOD mod[MAXNUMMOD];
char cmdstr[20];			//use global command string
unsigned int BaudRate;
int nummod;				//start off with no modules
HANDLE ComPort;
int SioError;
int IOBusy;
int printerrors;

/**************************************************************************************************************************************/

BOOL NmcHardReset(byte addr);
BOOL NmcChangeBaud(byte groupaddr, unsigned int baudrate);
void InitVars(void);
int NmcInit(char *portname, unsigned int baudrate);
BOOL NmcSendCmd(byte addr, byte cmd, char *datastr, byte n, byte stataddr);
void FixSioError(void);
BOOL NmcSetGroupAddr(byte addr, byte groupaddr, BOOL leader);
BOOL NmcSynchOutput(byte groupaddr, byte leaderaddr);
BOOL NmcSynchInput(byte groupaddr, byte leaderaddr);
BOOL NmcNoOp(byte addr);
BOOL NmcReadStatus(byte addr, byte statusitems);
BOOL NmcDefineStatus(byte addr, byte statusitems);
byte NmcGetStat(byte addr);
byte NmcGetStatItems(byte addr);
byte NmcGetModType(byte addr);
byte NmcGetModVer(byte addr);
byte NmcGetGroupAddr(byte addr);
BOOL NmcGroupLeader(byte addr);
void NmcShutdown(void);

/***************************************************************************************************************************************************************/

SERVOMOD * ServoNewMod();
BOOL ServoGetStat(byte addr);
long ServoGetPos(byte addr);
byte ServoGetAD(byte addr);
short int ServoGetVel(byte addr);
byte ServoGetAux(byte addr);
long ServoGetHome(byte addr);
short int ServoGetPError(byte addr);
byte ServoGetNPoints(byte addr);
long ServoGetCmdPos(byte addr);
long ServoGetCmdVel(byte addr);
long ServoGetCmdAcc(byte addr);
long ServoGetStopPos(byte addr);
byte ServoGetCmdPwm(byte addr);
byte ServoGetIoCtrl(byte addr);
byte ServoGetHomeCtrl(byte addr);
byte ServoGetStopCtrl(byte addr);
byte ServoGetMoveCtrl(byte addr);
void ServoGetGain(byte addr, short int * kp, short int * kd, short int * ki, short int * il, byte * ol, byte * cl, short int * el, byte * sr, byte * dc);
void ServoGetGain2(byte addr, short int * kp, short int * kd, short int * ki, short int * il, byte * ol, byte * cl, short int * el, byte * sr, byte * dc, byte * sm);
BOOL ServoSetGain(byte addr, short int kp, short int kd, short int ki, short int il, byte ol, byte cl, short int el, byte sr, byte dc);
BOOL ServoSetGain2(byte addr, short int kp, short int kd, short int ki, short int il, byte ol, byte cl, short int el, byte sr, byte dc, byte sm);
BOOL ServoLoadTraj(byte addr, byte mode, long pos, long vel, long acc, byte pwm);
void ServoInitPath(byte addr);
BOOL ServoAddPathpoints(byte addr, int npoints, long *path, int freq);
BOOL ServoStartPathMode(byte groupaddr, byte groupleader);
BOOL ServoStartMove(byte groupaddr, byte groupleader);
BOOL ServoResetPos(byte addr);
BOOL ServoResetRelHome(byte addr);
BOOL ServoSetPos(byte addr, long pos);
BOOL ServoClearBits(byte addr);
BOOL ServoStopMotor(byte addr, byte mode);
BOOL ServoStopHere(byte addr, byte mode, long pos);
BOOL ServoSetIoCtrl(byte addr, byte mode);
BOOL ServoSetHoming(byte addr, byte mode);
BOOL ServoHardReset(byte addr, byte mode);

/***************************************************************************************************************************************************************/

int SimpleMsgBox(char *msgstr);
void ErrorPrinting(int f);
int ErrorMsgBox(char *msgstr);
HANDLE SioOpen(char *name, unsigned int baudrate);
BOOL SioChangeBaud(HANDLE ComPort, unsigned int baudrate);
BOOL SioPutChars(HANDLE ComPort, char *stuff, int n);
DWORD SioGetChars(HANDLE ComPort, char *stuff, int n);
DWORD SioTest(HANDLE ComPort);
BOOL SioClrInbuf(HANDLE ComPort);
BOOL SioClose(HANDLE ComPort);
IOMOD * IoNewMod();
BOOL IoGetStat(byte addr);
BOOL IoInBitVal(byte addr, int bitnum);
BOOL IoInBitSVal(byte addr, int bitnum);
BOOL IoOutBitVal(byte addr, int bitnum);
BOOL IoSetOutBit(byte addr, int bitnum);
BOOL IoClrOutBit(byte addr, int bitnum);
BOOL IoGetBitDir(byte addr, int bitnum);
BOOL IoBitDirOut(byte addr, int bitnum);
BOOL IoBitDirIn(byte addr, int bitnum);
byte IoGetADCVal(byte addr, int channel);
BOOL IoSetPWMVal(byte addr, byte pwm1, byte pwm2);
BOOL IoSetSynchOutput(byte addr, short int outbits, byte pwm1, byte pwm2);
byte IoGetPWMVal(byte addr, int channel);
BOOL IoSetTimerMode(byte addr, byte tmrmode);
byte IoGetTimerMode(byte addr);
unsigned long IoGetTimerVal(byte addr);
unsigned long IoGetTimerSVal(byte addr);
STEPMOD * StepNewMod();
BOOL StepGetStat(byte addr);
int ErrorMsgBox1(char *msgstr);
DWORD SioGetChars1(HANDLE ComPort, char *stuff, int n);

/**************************************************************************************************************************************/

BOOL NmcHardReset(byte addr)
{
		int i;
		char cstr[5];

		//Send string of 0's to flush input buffers
		cstr[0] = 0;
		for (i=0; i<20; i++) SioPutChars(ComPort,cstr, 1);

		//Send out reset command string
		cstr[0] = 0xAA;			//Header
		cstr[1] = addr;
		cstr[2] = HARD_RESET;
		cstr[3] = (byte)(cstr[1] + cstr[2]);  //checksum
		SioPutChars(ComPort, cstr, 4);//send reset

		Sleep(100);		//wait for reset to execute

		for (i=0; i<nummod; i++) delete mod[i].p;  //delete and prev. declared modules
		nummod = 0;
		SioError = 0;	//reset the number of errors to 0

		SioChangeBaud(ComPort, 19200);	//Reset the baud rate to the default

		SioClrInbuf(ComPort);   //clear out any random crap left in buffer

		return TRUE;
}


BOOL NmcChangeBaud(byte groupaddr, unsigned int baudrate)
{
		char cstr[6];

		cstr[0] = 0xAA;			//Header
		cstr[1] = groupaddr;
		cstr[2] = 0x10 | SET_BAUD;
		switch (baudrate) {
			case 19200:		cstr[3] = PB19200; break;
			case 57600:		cstr[3] = PB57600; break;
			case 115200:	cstr[3] = PB115200; break;
			case 230400:	cstr[3] = PB230400; break;
			default:	ErrorMsgBox("Baud rate not supported - using default of 19200");
						cstr[3] = PB19200;
						baudrate = 19200;
			}
		cstr[4] = (byte)(cstr[1] + cstr[2] + cstr[3]);  //checksum
		SioPutChars(ComPort, cstr, 5);//send command

		Sleep(100);		//Wait for command to execute

		SioChangeBaud(ComPort, baudrate);	//Reset the baud rate to the default

		SioClrInbuf(ComPort);   //clear out any random crap left in buffer
		Sleep(100);

		BaudRate = baudrate;
		return true;
}


void InitVars(void)
{
		int i;

		mod[0].modtype = 0;   //Default to a known module type for module 0
		mod[0].modver = 0;
		mod[0].stat = 0;
		mod[0].statusitems = 0;
		mod[0].groupaddr = 0xFF;
		mod[0].groupleader = false;

		for (i=1; i<MAXNUMMOD; i++)
			{
			mod[i].modtype = 0xFF;
			mod[i].modver = 0;
			mod[i].stat = 0;
			mod[i].statusitems = 0;
			mod[i].groupaddr = 0xFF;
			mod[i].groupleader = false;
			}
}


int NmcInit(char *portname, unsigned int baudrate)
{
		DWORD numread;
		byte addr;
		char cstr[20];
		SERVOMOD *p;
		//char mess[40];

		BaudRate = baudrate;

		InitVars();

		ComPort = SioOpen(portname, 19200);    //Open with default rate of 19200
		if (ComPort == INVALID_HANDLE_VALUE) return 0;

		NmcHardReset(0xFF);
		NmcHardReset(0xFF);

		for (addr = 1; addr < MAXNUMMOD; addr++)
			{
			//First set the address to a unique value:
			cstr[0] = 0xAA;		//Header
			cstr[1] = 0;  		//Send to default address of 0
			cstr[2] = 0x20 | SET_ADDR;
			cstr[3] = addr;  	//Set new address sequentially
			cstr[4] = 0xFF;		//Set group address to 0xFF
			cstr[5] = (byte)(cstr[1] + cstr[2] + cstr[3] + cstr[4]);  //checksum
			SioPutChars(ComPort, cstr, 6);  //Send command

			numread = SioGetChars(ComPort, cstr, 2);  //get back status, cksum
			if (numread!=2) break;	//if no response, punt out of loop
			if (cstr[0] != cstr[1])
				{
				ErrorMsgBox("Status checksum error - please reset the Network");
				SioClose(ComPort);
				return(0);
				}

			//Read the device type
			cstr[0] = 0xAA;		//Header
			cstr[1] = addr;  		//Send to addr
			cstr[2] = 0x10 | READ_STAT;
			cstr[3] = SEND_ID;  //send ID only
			cstr[4] = (byte)(cstr[1] + cstr[2] + cstr[3]);  //checksum
			SioPutChars(ComPort, cstr, 5);  //Send command

			numread = SioGetChars(ComPort, cstr, 4);  //get back stat, type, ver, cksum
			if (numread!=4)
				{
				ErrorMsgBox("Could not read device type");
				SioClose(ComPort);
				return(0);
				}
			mod[addr].stat = cstr[0];
			mod[addr].modtype = cstr[1];
			mod[addr].modver = cstr[2];
			mod[addr].statusitems = 0;
			mod[addr].groupaddr = 0xFF;
			mod[addr].groupleader = false;
			switch (mod[addr].modtype)
				{
				case SERVOMODTYPE: 	mod[addr].p = ServoNewMod();
									//fix for change in I/O CTRL command
									if ((mod[addr].modver<10) || (mod[addr].modver>20))
									  {
									  p = (SERVOMOD *)(mod[addr].p);
									  p->ioctrl = IO1_IN | IO2_IN;
									  }
									break;
				case IOMODTYPE: 	mod[addr].p = IoNewMod(); break;
				case STEPMODTYPE: 	mod[addr].p = StepNewMod(); break;
				}

			}

		nummod = addr-1;

		if (nummod > 0)
			{
			NmcChangeBaud(0xFF, baudrate);
			//if (nummod>1) sprintf(mess,"%d Modules found", nummod);
			//else sprintf(mess,"%d Module found", nummod);
			//ErrorMsgBox(mess);
			}
		else
			{
			//ErrorMsgBox("No modules found on the network.\nPlease check power and connections.");
			SioClose(ComPort);
			}

		return(nummod);	//Return number of modules
}


BOOL NmcSendCmd(byte addr, byte cmd, char *datastr, byte n, byte stataddr)
{
		byte cksum;
		char mess[40];
		byte outstr[20];
		int i;
		BOOL iostat;

		//If too many SIO errors, punt
		if (SioError > MAXSIOERROR) return(false);

		IOBusy = true;

		//Check if data is to be returned from to a known module type
		if ( !( ( mod[stataddr].modtype==SERVOMODTYPE) ||
				( mod[stataddr].modtype==ADCMODTYPE) ||
				( mod[stataddr].modtype==IOMODTYPE)  ||
				( mod[stataddr].modtype==STEPMODTYPE) ) )
			{
			sprintf(mess,"Module type %d not supported",mod[stataddr].modtype);
			ErrorMsgBox(mess);
			return (false);
			}

		//Calculate the adjust command byte, calculate checksum and send the command
		cksum = 0;
		outstr[0] = 0xAA;	//start with header byte
		outstr[1] = addr;	//add on address byte
		cksum += outstr[1];
		outstr[2] = (byte)(((n<<4) & 0xF0) | cmd);
		cksum += outstr[2];
		for (i=0; i<n; i++)
			{
			outstr[i+3] = datastr[i];
			cksum += outstr[i+3];
			}
		outstr[n+3] = cksum;

		SioClrInbuf(ComPort);  //Get rid of any old input chars
		SioPutChars(ComPort, (char *)outstr, n+4); //Send the command string

		if (stataddr == 0)		//If a group command w/ no leader, add delay then exit
			{
			Sleep(60);
			IOBusy = false;
			return true;
			}

		switch (mod[stataddr].modtype) {
			case SERVOMODTYPE:	iostat = ServoGetStat(stataddr); break;
			case IOMODTYPE:		iostat = IoGetStat(stataddr); break;
			//case STEPMODTYPE:	iostat = StepGetStat(stataddr); break;
			}

		if (iostat == false)
			{
			SioError++;
			FixSioError();
			}
		else SioError = 0;

		IOBusy = false;
		return iostat;
}


void FixSioError(void)
{
		int i;
		char teststr[2];

		//If too many errors - prompt to reset
		if (SioError >= MAXSIOERROR)
			{
			ErrorMsgBox("Multiple communication errors - please reset the Network");
			return;
			}

		//Otherwise, send out a null string to re-synch
		teststr[0] = 0;
		for (i=0; i<30; i++) SioPutChars(ComPort, teststr, 1);
		Sleep(100);
		SioClrInbuf(ComPort);
}


BOOL NmcSetGroupAddr(byte addr, byte groupaddr, BOOL leader)
{
		char cmdstr[2];

		if (!(groupaddr & 0x80)) return false;  //punt if not valid group address

		cmdstr[0] = addr;
		cmdstr[1] = groupaddr;
		if (leader) cmdstr[1] &= 0x7F;  //clear upper bit if a group leader
		mod[addr].groupaddr = groupaddr;
		mod[addr].groupleader = leader;
		return NmcSendCmd(addr, SET_ADDR, cmdstr, 2, addr);
}


BOOL NmcSynchOutput(byte groupaddr, byte leaderaddr)
{
		return NmcSendCmd(groupaddr, SYNCH_OUT, NULL, 0, leaderaddr);
}


BOOL NmcSynchInput(byte groupaddr, byte leaderaddr)
{
		return NmcSendCmd(groupaddr, SYNCH_INPUT, NULL, 0, leaderaddr);
}


BOOL NmcNoOp(byte addr)
{
		return NmcSendCmd(addr, NOP, NULL, 0, addr);
}


BOOL NmcReadStatus(byte addr, byte statusitems)
{
		char cmdstr[2];
		byte oldstat;
		BOOL retval;

		cmdstr[0] = statusitems;
		oldstat = mod[addr].statusitems;
		mod[addr].statusitems = statusitems;
		retval = NmcSendCmd(addr, READ_STAT, cmdstr, 1, addr);
		mod[addr].statusitems = oldstat;

		return retval;
}


BOOL NmcDefineStatus(byte addr, byte statusitems)
{
		char cmdstr[2];

		cmdstr[0] = statusitems;
		mod[addr].statusitems = statusitems;
		return NmcSendCmd(addr, DEF_STAT, cmdstr, 1, addr);
}


byte NmcGetStat(byte addr)
{
		return mod[addr].stat;
}


byte NmcGetStatItems(byte addr)
{
		return mod[addr].statusitems;
}


byte NmcGetModType(byte addr)
{
		return mod[addr].modtype;
}


byte NmcGetModVer(byte addr)
{
		return mod[addr].modver;
}

byte NmcGetGroupAddr(byte addr)
{
		return mod[addr].groupaddr;
}

BOOL NmcGroupLeader(byte addr)
{
		return mod[addr].groupleader;
}

void NmcShutdown(void)
{
		if (ComPort!=INVALID_HANDLE_VALUE && ComPort!=NULL) NmcHardReset(0xFF);

		nummod = 0;
		SioClose(ComPort);
}

/***************************************************************************************************************************************************************/

SERVOMOD * ServoNewMod()
{
		SERVOMOD *p;

		p = new SERVOMOD;
		p->pos = 0;
		p->ad = 0;
		p->vel = 0;
		p->aux = 0;
		p->home = 0;
		p->perror = 0;
		p->cmdpos = 0;
		p->cmdvel = 0;
		p->cmdacc = 0;
		p->cmdpwm = 0;

		(p->gain).kp = 0;
		(p->gain).kd = 0;
		(p->gain).ki = 0;
		(p->gain).il = 0;
		(p->gain).ol = 0;
		(p->gain).cl = 0;
		(p->gain).el = 0;
		(p->gain).sr = 1;
		(p->gain).dc = 0;
		(p->gain).sm = 1;

		p->stoppos = 0;
		p->ioctrl = 0;
		p->homectrl = 0;
		p->movectrl = 0;
		p->stopctrl = 0;
		return p;
}

BOOL ServoGetStat(byte addr)
{
		int numbytes, numrcvd;
		int i, bytecount;
		byte cksum;
		byte inbuf[20];
		SERVOMOD *p;
		char msgstr[80];

		p = (SERVOMOD *)(mod[addr].p);  //cast the data pointer to the right type

		//Find number of bytes to read:
		numbytes = 2;       //start with stat & cksum
		if ( (mod[addr].statusitems) & SEND_POS )	numbytes +=4;
		if ( (mod[addr].statusitems) & SEND_AD ) 	numbytes +=1;
		if ( (mod[addr].statusitems) & SEND_VEL ) 	numbytes +=2;
		if ( (mod[addr].statusitems) & SEND_AUX ) 	numbytes +=1;
		if ( (mod[addr].statusitems) & SEND_HOME )	numbytes +=4;
		if ( (mod[addr].statusitems) & SEND_ID ) 	numbytes +=2;
		if ( (mod[addr].statusitems) & SEND_PERROR ) numbytes +=2;
		if ( (mod[addr].statusitems) & SEND_NPOINTS ) numbytes +=1;
		numrcvd = SioGetChars(ComPort, (char *)inbuf, numbytes);

		//Verify enough data was read
		if (numrcvd != numbytes)
			{
			sprintf(msgstr,"ServoGetStat (%d) failed to read chars",addr);
			ErrorMsgBox(msgstr);
			return false;
			}

		//Verify checksum:
		cksum = 0;
		for (i=0; i<numbytes-1; i++) cksum = (byte)(cksum + inbuf[i]);
		if (cksum != inbuf[numbytes-1])
			{
			sprintf(msgstr,"ServoGetStat(%d): checksum error",addr);
			ErrorMsgBox(msgstr);
			return false;
			}

		//Verify command was received intact before updating status data
		mod[addr].stat = inbuf[0];
		if (mod[addr].stat & CKSUM_ERROR)
			{
			ErrorMsgBox("Command checksum error!");
			return false;
			}

		//Finally, fill in status data
		bytecount = 1;
		if ( (mod[addr].statusitems) & SEND_POS )
			{
			p->pos = *( (long *)(inbuf + bytecount) );
			bytecount +=4;
			}
		if ( (mod[addr].statusitems) & SEND_AD )
			{
			p->ad = inbuf[bytecount];
			bytecount +=1;
			}
		if ( (mod[addr].statusitems) & SEND_VEL )
			{
			p->vel = *( (short int *)(inbuf + bytecount) );
			bytecount +=2;
			}
		if ( (mod[addr].statusitems) & SEND_AUX )
			{
			p->aux = inbuf[bytecount];
			bytecount +=1;
			}
		if ( (mod[addr].statusitems) & SEND_HOME )
			{
			p->home = *( (unsigned long *)(inbuf + bytecount) );
			bytecount +=4;
			}
		if ( (mod[addr].statusitems) & SEND_ID )
			{
			mod[addr].modtype = inbuf[bytecount];
			mod[addr].modver = inbuf[bytecount+1];
			bytecount +=2;
			}
		if ( (mod[addr].statusitems) & SEND_PERROR )
			{
			p->perror = *( (short int *)(inbuf + bytecount) );
			bytecount +=2;
			}
		if ( (mod[addr].statusitems) & SEND_NPOINTS )
			{
			p->npoints = inbuf[bytecount];
			//bytecount +=1;
			}

		return TRUE;
}

long ServoGetPos(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->pos;
}

byte ServoGetAD(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->ad;
}

short int ServoGetVel(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->vel;
}

byte ServoGetAux(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->aux;
}

long ServoGetHome(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->home;
}

short int ServoGetPError(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->perror;
}

byte ServoGetNPoints(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->npoints;
}

long ServoGetCmdPos(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->cmdpos;
}

long ServoGetCmdVel(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->cmdvel;
}

long ServoGetCmdAcc(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->cmdacc;
}

long ServoGetStopPos(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->stoppos;
}

byte ServoGetCmdPwm(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->cmdpwm;
}

byte ServoGetIoCtrl(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->ioctrl;
}

byte ServoGetHomeCtrl(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->homectrl;
}

byte ServoGetStopCtrl(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->stopctrl;
}

byte ServoGetMoveCtrl(byte addr)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		return p->movectrl;
}

void ServoGetGain(byte addr, short int * kp, short int * kd, short int * ki,
				  short int * il, byte * ol, byte * cl, short int * el,
                  byte * sr, byte * dc)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		*kp = (p->gain).kp;
		*kd = (p->gain).kd;
		*ki = (p->gain).ki;
		*il = (p->gain).il;
		*ol = (p->gain).ol;
		*cl = (p->gain).cl;
		*el = (p->gain).el;
		*sr = (p->gain).sr;
		*dc = (p->gain).dc;
}

void ServoGetGain2(byte addr, short int * kp, short int * kd, short int * ki,
				  short int * il, byte * ol, byte * cl, short int * el,
                  byte * sr, byte * dc, byte * sm)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		*kp = (p->gain).kp;
		*kd = (p->gain).kd;
		*ki = (p->gain).ki;
		*il = (p->gain).il;
		*ol = (p->gain).ol;
		*cl = (p->gain).cl;
		*el = (p->gain).el;
		*sr = (p->gain).sr;
		*dc = (p->gain).dc;
		*sm = (p->gain).sm;
}

BOOL ServoSetGain(byte addr, short int kp, short int kd, short int ki,
				  short int il, byte ol, byte cl, short int el,
                  byte sr, byte dc)
{
		SERVOMOD * p;
		char cmdstr[16];

		p = (SERVOMOD *)(mod[addr].p);
		(p->gain).kp = kp;
		(p->gain).kd = kd;
		(p->gain).ki = ki;
		(p->gain).il = il;
		(p->gain).ol = ol;
		(p->gain).cl = cl;
		(p->gain).el = el;
		(p->gain).sr = sr;
		(p->gain).dc = dc;

		*( (short int *)(cmdstr) ) = kp;
		*( (short int *)(cmdstr+2) ) = kd;
		*( (short int *)(cmdstr+4) ) = ki;
		*( (short int *)(cmdstr+6) ) = il;
		*( (byte *)(cmdstr+8) ) = ol;
		*( (byte *)(cmdstr+9) ) = cl;
		*( (short int *)(cmdstr+10) ) = el;
		*( (byte *)(cmdstr+12) ) = sr;
		*( (byte *)(cmdstr+13) ) = dc;

		return NmcSendCmd(addr, SET_GAIN, cmdstr, 14, addr);
}

BOOL ServoSetGain2(byte addr, short int kp, short int kd, short int ki,
				  short int il, byte ol, byte cl, short int el,
                  byte sr, byte dc, byte sm)
{
		SERVOMOD * p;
		char cmdstr[16];

		p = (SERVOMOD *)(mod[addr].p);
		(p->gain).kp = kp;
		(p->gain).kd = kd;
		(p->gain).ki = ki;
		(p->gain).il = il;
		(p->gain).ol = ol;
		(p->gain).cl = cl;
		(p->gain).el = el;
		(p->gain).sr = sr;
		(p->gain).dc = dc;
		(p->gain).sm = sm;

		*( (short int *)(cmdstr) ) = kp;
		*( (short int *)(cmdstr+2) ) = kd;
		*( (short int *)(cmdstr+4) ) = ki;
		*( (short int *)(cmdstr+6) ) = il;
		*( (byte *)(cmdstr+8) ) = ol;
		*( (byte *)(cmdstr+9) ) = cl;
		*( (short int *)(cmdstr+10) ) = el;
		*( (byte *)(cmdstr+12) ) = sr;
		*( (byte *)(cmdstr+13) ) = dc;
		*( (byte *)(cmdstr+14) ) = sm;

		return NmcSendCmd(addr, SET_GAIN, cmdstr, 15, addr);
}

BOOL ServoLoadTraj(byte addr, byte mode, long pos, long vel, long acc, byte pwm)
{
		SERVOMOD * p;
		char cmdstr[16];
		int count;

		p = (SERVOMOD *)(mod[addr].p);
		p->movectrl = mode;
		p->cmdpos = pos;
		p->cmdvel = vel;
		p->cmdacc = acc;
		p->cmdpwm = pwm;

		count = 0;
		*( (byte *)(cmdstr + count) ) = mode;  count += 1;
		if (mode & LOAD_POS) { *( (long *)(cmdstr + count) ) = pos; count += 4; }
		if (mode & LOAD_VEL) { *( (long *)(cmdstr + count) ) = vel; count += 4; }
		if (mode & LOAD_ACC) { *( (long *)(cmdstr + count) ) = acc; count += 4; }
		if (mode & LOAD_PWM) { *( (byte *)(cmdstr + count) ) = pwm; count += 1; }

		return NmcSendCmd(addr, LOAD_TRAJ, cmdstr, (byte)count, addr);
}

void ServoInitPath(byte addr)
{
		SERVOMOD * p;

		NmcReadStatus(addr, SEND_POS | SEND_PERROR);

		p = (SERVOMOD *)(mod[addr].p);
		p->last_ppoint = p->pos + p->perror;
}

BOOL ServoAddPathpoints(byte addr, int npoints, long *path, int freq)
{
		SERVOMOD * p;
		char cmdstr[16];
		long diff;
		int rev;
		int i;

		//npoints must be greater than 0
		if (npoints <= 0) return 0;

		p = (SERVOMOD *)(mod[addr].p);

		for (i=0; i<npoints; i++)
		  {
		  diff = path[i] - p->last_ppoint;
		  if (diff<0)
			{
			rev = 0x01;
			diff = -diff;
			}
		  else rev = 0x00;

		  //Scale the difference appropriately for path freq. used
		  if (p->ioctrl & FAST_PATH)  //scale for 60/120 Hz fast path
			{
			if (freq == P_60HZ)
			  {
			  diff *= (256/32);
			  diff |= 0x02;     //60 Hz -> set bit 1 = 1
			  }
			else if (freq == P_120HZ) diff *= (256/16);
			else return(false);
			}
		  else  //scale for 30/60 Hz slow path
			{
			if (freq == P_30HZ)
			  {
			  diff *= (256/64);
			  diff |= 0x02;     //30 Hz -> set bit 1 = 1
			  }
			else if (freq == P_60HZ) diff *= (256/32);
			else return(false);
			}

		  diff |= rev;  //bit 0 = reverse bit

		  *( (short int *)(cmdstr + 2*i) ) = (short int)diff;

		  p->last_ppoint = path[i];
		  }


		return NmcSendCmd(addr, ADD_PATHPOINT, cmdstr, (byte)(npoints*2), addr);
}

BOOL ServoStartPathMode(byte groupaddr, byte groupleader)
{
		return NmcSendCmd(groupaddr, ADD_PATHPOINT, NULL, 0, groupleader);
}

BOOL ServoStartMove(byte groupaddr, byte groupleader)
{
		return NmcSendCmd(groupaddr, START_MOVE, NULL, 0, groupleader);
}

BOOL ServoResetPos(byte addr)
{
		return NmcSendCmd(addr, RESET_POS, NULL, 0, addr);
}

BOOL ServoResetRelHome(byte addr)
{
		byte mode;

		mode = REL_HOME;
		return NmcSendCmd(addr, RESET_POS, (char *)(&mode), 1, addr);
}

BOOL ServoSetPos(byte addr, long pos)
{
		char cmdstr[6];

		cmdstr[0] = SET_POS;          //mode byte for reset pos
		*( (long *)(cmdstr + 1) ) = pos;

		return NmcSendCmd(addr, RESET_POS, cmdstr, 5, addr);
}

BOOL ServoClearBits(byte addr)
{
		return NmcSendCmd(addr, CLEAR_BITS, NULL, 0, addr);
}

BOOL ServoStopMotor(byte addr, byte mode)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);

		mode &= (byte)(~STOP_HERE);
		p->stopctrl = mode;       //make sure STOP_HERE bit is cleared

		return NmcSendCmd(addr, STOP_MOTOR, (char *)(&mode), 1, addr);
}

BOOL ServoStopHere(byte addr, byte mode, long pos)
{
		SERVOMOD * p;
		char cmdstr[6];

		p = (SERVOMOD *)(mod[addr].p);

		p->stopctrl = mode;

		cmdstr[0] = mode;
		*( (long *)(cmdstr + 1) ) = pos;

		return NmcSendCmd(addr, STOP_MOTOR, cmdstr, 5, addr);
}

BOOL ServoSetIoCtrl(byte addr, byte mode)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);

		p->ioctrl = mode;

		return NmcSendCmd(addr, IO_CTRL, (char *)(&mode), 1, addr);
}

BOOL ServoSetHoming(byte addr, byte mode)
{
		SERVOMOD * p;

		p = (SERVOMOD *)(mod[addr].p);
		p->homectrl = mode;

		return NmcSendCmd(addr, SET_HOMING, (char *)(&mode), 1, addr);
}

BOOL ServoHardReset(byte addr, byte mode)
{
		return NmcSendCmd(addr, HARD_RESET, (char *)(&mode), 1, 0);
}


/***************************************************************************************************************************************************************/

int SimpleMsgBox(char *msgstr)
{
		return MessageBox(NULL, msgstr,"",MB_TASKMODAL | MB_SETFOREGROUND);
}

void ErrorPrinting(int f)
{
		printerrors = f;
}

int ErrorMsgBox(char *msgstr)
{
		if (printerrors)
		  return MessageBox(NULL, msgstr,"",MB_TASKMODAL | MB_SETFOREGROUND);
		else return(0);
}

HANDLE SioOpen(char *name, unsigned int baudrate)
{
		BOOL RetStat;
		COMMCONFIG cc;
		COMMTIMEOUTS ct;
		HANDLE ComHandle;
		DWORD winrate;
		char msgstr[50];

		//Open COM port as a file
		ComHandle = CreateFile(name, GENERIC_READ | GENERIC_WRITE ,0, NULL,
							   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL);


		while (TRUE)
			{
			if (ComHandle == INVALID_HANDLE_VALUE)
				{
				sprintf(msgstr,"%s failed to open",name);
				ErrorMsgBox(msgstr);
				break;
				}

			switch (baudrate) {
				case 9600: 		winrate = CBR_9600; break;
				case 19200: 	winrate = CBR_19200; break;
				case 38400: 	winrate = CBR_38400; break;
				case 57600: 	winrate = CBR_57600; break;
				case 115200: 	winrate = CBR_115200; break;
				case 230400: 	winrate = 230400; break;
				default:		ErrorMsgBox("Baud rate not supported - using default of 19200");
								winrate = CBR_19200;
				}

			//Fill in COM port config. structure & set config.
			cc.dwSize = sizeof(DCB) + sizeof(WCHAR) + 20;
			cc.wVersion = 1;

			cc.dcb.DCBlength = sizeof(DCB);
			cc.dcb.BaudRate = winrate;
			cc.dcb.fBinary = 1;
			cc.dcb.fParity = 0;
			cc.dcb.fOutxCtsFlow = 0;
			cc.dcb.fOutxDsrFlow = 0;
			cc.dcb.fDtrControl = DTR_CONTROL_DISABLE;
			cc.dcb.fDsrSensitivity = 0;
			cc.dcb.fTXContinueOnXoff = 0;
			cc.dcb.fOutX = 0;
			cc.dcb.fInX = 0;
			cc.dcb.fErrorChar = 0;
			cc.dcb.fNull = 0;
			cc.dcb.fRtsControl = RTS_CONTROL_DISABLE;
			cc.dcb.fAbortOnError = 0;
			cc.dcb.XonLim = 100;
			cc.dcb.XoffLim = 100;
			cc.dcb.ByteSize = 8;
			cc.dcb.Parity = NOPARITY;
			cc.dcb.StopBits = ONESTOPBIT;
			cc.dcb.XonChar = 'x';
			cc.dcb.XoffChar = 'y';
			cc.dcb.ErrorChar = 0;
			cc.dcb.EofChar = 0;
			cc.dcb.EvtChar = 0;

			cc.dwProviderSubType = PST_RS232;
			cc.dwProviderOffset = 0;
			cc.dwProviderSize = 0;

			RetStat = SetCommConfig(ComHandle, &cc, sizeof(cc));
			if (RetStat == 0)
				{
				ErrorMsgBox("Failed to set COMM configuration");
				break;
				}

			//Set read/write timeout values for the file
			ct.ReadIntervalTimeout = 0;  		//ignore interval timing
			ct.ReadTotalTimeoutMultiplier = 2; 	//2 msec per char
			ct.ReadTotalTimeoutConstant = 50;  		//plus add'l 50 msec
			ct.WriteTotalTimeoutMultiplier = 2;	//Set max time per char written
			ct.WriteTotalTimeoutConstant = 50;	//plus additional time

			RetStat = SetCommTimeouts(ComHandle, &ct);
			if (RetStat == 0)
				{
				ErrorMsgBox("Failed to set Comm timeouts");
				break;
				}

			break;
			}

		return(ComHandle);
}

BOOL SioChangeBaud(HANDLE ComPort, unsigned int baudrate)
{
		BOOL RetStat;
		DWORD winrate;
		DCB cs;

		RetStat = GetCommState(ComPort, &cs);
		if (RetStat == false) return RetStat;
		switch (baudrate) {
			case 9600: 		winrate = CBR_9600; break;
			case 19200: 	winrate = CBR_19200; break;
			case 38400: 	winrate = CBR_38400; break;
			case 57600: 	winrate = CBR_57600; break;
			case 115200: 	winrate = CBR_115200; break;
			case 230400: 	winrate = 230400; break;
			default:		ErrorMsgBox("Baud rate not supported");
							return false;
			}
		cs.BaudRate = winrate;
		RetStat = SetCommState(ComPort, &cs);
		if (RetStat == false) return RetStat;
		return true;
}

BOOL SioPutChars(HANDLE ComPort, char *stuff, int n)
{
		BOOL RetStat;
		DWORD nums;

		RetStat = WriteFile(ComPort, stuff,n, &nums, NULL);
		if (RetStat == 0) ErrorMsgBox("SioPutChars failed");
		return RetStat;
}

DWORD SioGetChars(HANDLE ComPort, char *stuff, int n)
{
		BOOL RetStat;
		DWORD numread;

		RetStat = ReadFile(ComPort, stuff, n, &numread, NULL);
		if (RetStat == 0) ErrorMsgBox("SioReadChars failed");

		return numread;
}

DWORD SioTest(HANDLE ComPort)
{
		COMSTAT cs;
		DWORD Errors;
		BOOL RetStat;

		RetStat = ClearCommError(ComPort, &Errors, &cs);
		if (RetStat == 0) ErrorMsgBox("SioTest failed");
		return cs.cbInQue;
}

BOOL SioClrInbuf(HANDLE ComPort)
{
		BOOL RetStat;

		RetStat = PurgeComm(ComPort, PURGE_RXCLEAR);
		if (RetStat == 0) ErrorMsgBox("SioClrInbuf failed");

		return RetStat;
}

BOOL SioClose(HANDLE ComPort)
{
		return(CloseHandle(ComPort));
}

IOMOD * IoNewMod()
{
	IOMOD *p;

	p = new IOMOD;
	p->inbits = 0;
	p->ad1 = 0;
	p->ad2 = 0;
	p->ad3 = 0;
	p->timer = 0;
	p->inbits_s = 0;
	p->timer_s = 0;
	p->pwm1 = 0;
	p->pwm2 = 0;
	p->bitdir = 0x0FFF;
	p->outbits = 0;
	p->timermode = 0;
	return p;
}


BOOL IoGetStat(byte addr)
{
	int numbytes, numrcvd;
	int i, bytecount;
	byte cksum;
	byte inbuf[20];
	IOMOD *p;

	p = (IOMOD *)(mod[addr].p);  //cast the data pointer to the right type

	//Find number of bytes to read:
	numbytes = 2;       //start with stat & cksum
	if ( (mod[addr].statusitems) & SEND_INPUTS )	numbytes +=2;
	if ( (mod[addr].statusitems) & SEND_AD1 ) 		numbytes +=1;
	if ( (mod[addr].statusitems) & SEND_AD2 ) 		numbytes +=1;
	if ( (mod[addr].statusitems) & SEND_AD3 ) 		numbytes +=1;
	if ( (mod[addr].statusitems) & SEND_TIMER )		numbytes +=4;
	if ( (mod[addr].statusitems) & SEND_ID ) 		numbytes +=2;
	if ( (mod[addr].statusitems) & SEND_SYNC_IN )	numbytes +=2;
	if ( (mod[addr].statusitems) & SEND_SYNC_TMR )	numbytes +=4;
	numrcvd = SioGetChars(ComPort, (char *)inbuf, numbytes);

	//Verify enough data was read
	if (numrcvd != numbytes)
		{
	    ErrorMsgBox("IoGetStat failed to read chars");
	    return false;
	    }

	//Verify checksum:
	cksum = 0;
	for (i=0; i<numbytes-1; i++) cksum = (byte)(cksum + inbuf[i]);
	if (cksum != inbuf[numbytes-1])
		{
	    ErrorMsgBox("IoGetStat: checksum error");
	    return false;
	    }

	//Verify command was received intact before updating status data
	mod[addr].stat = inbuf[0];
	if (mod[addr].stat & CKSUM_ERROR)
		{
	    ErrorMsgBox("Command checksum error!");
	    return false;
	    }

	//Finally, fill in status data
	bytecount = 1;
	if ( (mod[addr].statusitems) & SEND_INPUTS )
		{
		p->inbits = *( (short int *)(inbuf + bytecount) );
	    bytecount +=2;
	    }
	if ( (mod[addr].statusitems) & SEND_AD1 )
		{
	    p->ad1 = inbuf[bytecount];
	    bytecount +=1;
	    }
	if ( (mod[addr].statusitems) & SEND_AD2 )
		{
	    p->ad2 = inbuf[bytecount];
	    bytecount +=1;
	    }
	if ( (mod[addr].statusitems) & SEND_AD3 )
		{
	    p->ad3 = inbuf[bytecount];
	    bytecount +=1;
	    }
	if ( (mod[addr].statusitems) & SEND_TIMER )
		{
	    p->timer = *( (unsigned long *)(inbuf + bytecount) );
	    bytecount +=4;
	    }
	if ( (mod[addr].statusitems) & SEND_ID )
		{
	    mod[addr].modtype = inbuf[bytecount];
	    mod[addr].modver = inbuf[bytecount+1];
	    bytecount +=2;
	    }
	if ( (mod[addr].statusitems) & SEND_SYNC_IN )
		{
	    p->inbits_s = *( (short int *)(inbuf + bytecount) );
	    bytecount +=2;
	    }
	if ( (mod[addr].statusitems) & SEND_SYNC_TMR )
		{
	    p->timer_s = *( (unsigned long *)(inbuf + bytecount) );
	    //bytecount +=4;
	    }

	return TRUE;
}


BOOL IoInBitVal(byte addr, int bitnum)
{
	IOMOD * p;

	p = (IOMOD *)(mod[addr].p);
	return ((p->inbits >> bitnum) & 1);
}

BOOL IoInBitSVal(byte addr, int bitnum)
{
	IOMOD * p;

	p = (IOMOD *)(mod[addr].p);
	return ((p->inbits_s >> bitnum) & 1);
}

BOOL IoOutBitVal(byte addr, int bitnum)
{
	IOMOD * p;

	p = (IOMOD *)(mod[addr].p);
	return ((p->outbits >> bitnum) & 1);
}

BOOL IoSetOutBit(byte addr, int bitnum)
{
	IOMOD * p;

	p = (IOMOD *)(mod[addr].p);  			//Point to the IO data structure
	p->outbits = p->outbits | (short int)(1 << bitnum);

	return NmcSendCmd(addr, SET_OUTPUT, (char *)(&(p->outbits)), 2, addr);
}

BOOL IoClrOutBit(byte addr, int bitnum)
{
	IOMOD * p;

	p = (IOMOD *)(mod[addr].p);  			//Point to the IO data structure
	p->outbits = p->outbits & (short int)(~(1 << bitnum));

	return NmcSendCmd(addr, SET_OUTPUT, (char *)(&(p->outbits)), 2, addr);
}

BOOL IoGetBitDir(byte addr, int bitnum)
{
	IOMOD * p;

	p = (IOMOD *)(mod[addr].p);
	return ((p->bitdir >> bitnum) & 1);
}

BOOL IoBitDirOut(byte addr, int bitnum)
{
	IOMOD * p;

	p = (IOMOD *)(mod[addr].p);  			//Point to the IO data structure
	p->bitdir = p->bitdir & (short int)(~(1 << bitnum));

	return NmcSendCmd(addr, SET_IO_DIR, (char *)(&(p->bitdir)), 2, addr);
}

BOOL IoBitDirIn(byte addr, int bitnum)
{
	IOMOD * p;

	p = (IOMOD *)(mod[addr].p);  			//Point to the IO data structure
	p->bitdir = p->bitdir | (short int)(1 << bitnum);

	return NmcSendCmd(addr, SET_IO_DIR, (char *)(&(p->bitdir)), 2, addr);
}

byte IoGetADCVal(byte addr, int channel)
{
	IOMOD * p;
	p = (IOMOD *)(mod[addr].p);

	switch (channel) {
		case 0: return p->ad1;
		case 1: return p->ad2;
		case 2: return p->ad3;
	    }

	return 0;
}

BOOL IoSetPWMVal(byte addr, byte pwm1, byte pwm2)
{
	IOMOD * p;
	char cmdstr[4];

	p = (IOMOD *)(mod[addr].p);  			//Point to the IO data structure
	p->pwm1 = pwm1;
	p->pwm2 = pwm2;
	cmdstr[0] = pwm1;
	cmdstr[1] = pwm2;

	return NmcSendCmd(addr, SET_PWM, cmdstr, 2, addr);
}

BOOL IoSetSynchOutput(byte addr, short int outbits, byte pwm1, byte pwm2)
{
	IOMOD * p;
	char cmdstr[5];

	p = (IOMOD *)(mod[addr].p);  			//Point to the IO data structure
	p->outbits =  outbits;
	p->pwm1 = pwm1;
	p->pwm2 = pwm2;
	cmdstr[0] = ((char *)(&outbits))[0];
	cmdstr[1] = ((char *)(&outbits))[1];
	cmdstr[2] = pwm1;
	cmdstr[3] = pwm2;
	return NmcSendCmd(addr, SET_SYNCH_OUT, cmdstr, 4, addr);
}

byte IoGetPWMVal(byte addr, int channel)
{
	IOMOD * p;

	p = (IOMOD *)(mod[addr].p);  			//Point to the IO data structure
	if (channel == 0) return(p->pwm1);
	else return(p->pwm2);
}

BOOL IoSetTimerMode(byte addr, byte tmrmode)
{
	IOMOD * p;
	char cmdstr[2];

	p = (IOMOD *)(mod[addr].p);  			//Point to the IO data structure
	p->timermode = tmrmode;
	cmdstr[0] = tmrmode;

	return NmcSendCmd(addr, SET_TMR_MODE, cmdstr, 1, addr);
}

byte IoGetTimerMode(byte addr)
{
	IOMOD * p;

	p = (IOMOD *)(mod[addr].p);  			//Point to the IO data structure
	return p->timermode;
}

unsigned long IoGetTimerVal(byte addr)
{
	IOMOD * p;

	p = (IOMOD *)(mod[addr].p);  			//Point to the IO data structure
	return(p->timer);
}

unsigned long IoGetTimerSVal(byte addr)
{
	IOMOD * p;

	p = (IOMOD *)(mod[addr].p);  			//Point to the IO data structure
	return(p->timer_s);
}

STEPMOD * StepNewMod()
{
	STEPMOD *p;

	p = new STEPMOD;
	p->pos = 0;
	p->ad = 0;
	p->st = 0;
	p->inbyte = 0;
	p->home = 0;

	p->cmdpos = 0;
	p->cmdspeed = 1;
	p->cmdacc = 1;
	p->cmdst = 0;
	p->min_speed = 1;
	p->outbyte = 0;
	p->homectrl = 0;
	p->ctrlmode = SPEED_1X | ESTOP_OFF;
	p->stopctrl = 0;
	p->run_pwm = 0;
	p->hold_pwm = 0;
	p->therm_limit = 0;
	return p;
}

BOOL StepGetStat(byte addr)
{
	int numbytes, numrcvd;
	int i, bytecount;
	byte cksum;
	byte inbuf[20];
	STEPMOD *p;
	char msgstr[80];

	p = (STEPMOD *)(mod[addr].p);  //cast the data pointer to the right type

	//Find number of bytes to read:
	numbytes = 2;       //start with stat & cksum
	if ( (mod[addr].statusitems) & SEND_POS )	numbytes +=4;
	if ( (mod[addr].statusitems) & SEND_AD ) 	numbytes +=1;
	if ( (mod[addr].statusitems) & SEND_ST ) 	numbytes +=2;
	if ( (mod[addr].statusitems) & SEND_INBYTE ) numbytes +=1;
	if ( (mod[addr].statusitems) & SEND_HOME )	numbytes +=4;
	if ( (mod[addr].statusitems) & SEND_ID ) 	numbytes +=2;
	numrcvd = SioGetChars(ComPort, (char *)inbuf, numbytes);

	//Verify enough data was read
	if (numrcvd != numbytes)
		{
	    sprintf(msgstr,"StepGetStat (%d) failed to read chars",addr);
	    ErrorMsgBox(msgstr);
	    return false;
	    }

	//Verify checksum:
	cksum = 0;
	for (i=0; i<numbytes-1; i++) cksum = (byte)(cksum + inbuf[i]);
	if (cksum != inbuf[numbytes-1])
		{
	    sprintf(msgstr,"StepGetStat(%d): checksum error",addr);
	    ErrorMsgBox(msgstr);
	    return false;
	    }

	//Verify command was received intact before updating status data
	mod[addr].stat = inbuf[0];
	if (mod[addr].stat & CKSUM_ERROR)
		{
	    ErrorMsgBox("Command checksum error!");
	    return false;
	    }

	//Finally, fill in status data
	bytecount = 1;
	if ( (mod[addr].statusitems) & SEND_POS )
		{
		p->pos = *( (long *)(inbuf + bytecount) );
	    bytecount +=4;
	    }
	if ( (mod[addr].statusitems) & SEND_AD )
		{
	    p->ad = inbuf[bytecount];
	    bytecount +=1;
	    }
	if ( (mod[addr].statusitems) & SEND_ST )
		{
		p->st = *( (unsigned short int *)(inbuf + bytecount) );
	    bytecount +=2;
	    }
	if ( (mod[addr].statusitems) & SEND_INBYTE )
		{
	    p->inbyte = inbuf[bytecount];
	    bytecount +=1;
	    }
	if ( (mod[addr].statusitems) & SEND_HOME )
		{
	    p->home = *( (unsigned long *)(inbuf + bytecount) );
	    bytecount +=4;
	    }
	if ( (mod[addr].statusitems) & SEND_ID )
		{
	    mod[addr].modtype = inbuf[bytecount];
	    mod[addr].modver = inbuf[bytecount+1];
	    //bytecount +=2;
	    }

	return TRUE;
}

/**************************************************************************************************************************************/
