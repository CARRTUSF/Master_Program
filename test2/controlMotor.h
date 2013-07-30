extern "C" __declspec(dllexport) int init (int CommPort, long BaudRate, int fileOpen);
extern "C" __declspec(dllexport) int close ();
extern "C" __declspec(dllexport) int changeBaudRate (long BaudRate);
extern "C" __declspec(dllexport) int changeCommPort (int CommPort);

extern "C" __declspec(dllexport) char * lastErrorString ();
extern "C" __declspec(dllexport) int lastErrorCode ();

extern "C" __declspec(dllexport) int enable (int module);
extern "C" __declspec(dllexport) int off (int module);

extern "C" __declspec(dllexport) int enableAll ();
extern "C" __declspec(dllexport) int offAll ();

extern "C" __declspec(dllexport) int enableSelect (int module[]);
extern "C" __declspec(dllexport) int offSelect (int module[]);

extern "C" __declspec(dllexport) int pos (int module, unsigned long pos, unsigned long vel, unsigned long acc, int rev);
extern "C" __declspec(dllexport) int vel (int module, unsigned long vel, unsigned long acc, int rev);
extern "C" __declspec(dllexport) int pwm (int module, unsigned char pwm, int rev);

extern "C" __declspec(dllexport) int posAll (unsigned long pos, unsigned long vel, unsigned long acc, int rev);
extern "C" __declspec(dllexport) int velAll (unsigned long vel, unsigned long acc, int rev);
extern "C" __declspec(dllexport) int pwmAll (unsigned char pwm, int rev);

extern "C" __declspec(dllexport) int posSelect (int module[], unsigned long pos[], unsigned long vel[], unsigned long acc[], int rev[]);
extern "C" __declspec(dllexport) int velSelect (int module[], unsigned long vel[], unsigned long acc[], int rev[]);
extern "C" __declspec(dllexport) int pwmSelect (int module[], unsigned char pwm[], int rev[]);

extern "C" __declspec(dllexport) int posSelectAll (int module[], unsigned long pos, unsigned long vel, unsigned long acc, int rev);
extern "C" __declspec(dllexport) int velSelectAll (int module[], unsigned long vel, unsigned long acc, int rev);
extern "C" __declspec(dllexport) int pwmSelectAll (int module[], unsigned char pwm, int rev);

extern "C" __declspec(dllexport) int stop (int module);
extern "C" __declspec(dllexport) int brk (int module);

extern "C" __declspec(dllexport) int stopAll ();
extern "C" __declspec(dllexport) int brkAll ();

extern "C" __declspec(dllexport) int stopSelect (int module[]);
extern "C" __declspec(dllexport) int brkSelect (int module[]);

extern "C" __declspec(dllexport) long getPos (int module);
extern "C" __declspec(dllexport) long getVel (int module);
extern "C" __declspec(dllexport) unsigned char getAd (int module);
extern "C" __declspec(dllexport) long getHm (int module);

extern "C" __declspec(dllexport) int getPosAll (long * x);
extern "C" __declspec(dllexport) int getVelAll (long * x);
extern "C" __declspec(dllexport) int getAdAll (long * x);
extern "C" __declspec(dllexport) int getHmAll (long * x);

extern "C" __declspec(dllexport) int status (int module, long * x);
extern "C" __declspec(dllexport) int statusAll (long * x);

extern "C" __declspec(dllexport) int setPos (int module, unsigned long pos, int rev);
extern "C" __declspec(dllexport) int setPosAll (unsigned long pos, int rev);
extern "C" __declspec(dllexport) int setPosSelect (int module[], unsigned long pos[], int rev[]);
extern "C" __declspec(dllexport) int setPosSelectAll (int module[], unsigned long pos, int rev);

extern "C" __declspec(dllexport) int reset (int module);
extern "C" __declspec(dllexport) int clear (int module);

extern "C" __declspec(dllexport) int resetAll ();
extern "C" __declspec(dllexport) int clearAll ();

extern "C" __declspec(dllexport) int resetSelect (int module[]);
extern "C" __declspec(dllexport) int clearSelect (int module[]);

extern "C" __declspec(dllexport) int setParams (int module, short int tkp, short int tkd, short int tki, short int til, short int tel, unsigned char tol, unsigned char tcl, unsigned char tsr, unsigned char tdc, unsigned char tsm);
extern "C" __declspec(dllexport) int setParamsPID (int module, short int tkp, short int tkd, short int tki);
extern "C" __declspec(dllexport) int setParamsILEL (int module, short int til, short int tel);
extern "C" __declspec(dllexport) int setParamsLIMIT (int module, unsigned char tol, unsigned char tcl);
extern "C" __declspec(dllexport) int setParamsRCM (int module, unsigned char tsr, unsigned char tdc, unsigned char tsm);

extern "C" __declspec(dllexport) int setParamsAll (short int tkp, short int tkd, short int tki, short int til, short int tel, unsigned char tol, unsigned char tcl, unsigned char tsr, unsigned char tdc, unsigned char tsm);
extern "C" __declspec(dllexport) int setParamsPIDAll (short int tkp, short int tkd, short int tki);
extern "C" __declspec(dllexport) int setParamsILELAll (short int til, short int tel);
extern "C" __declspec(dllexport) int setParamsLIMITAll (unsigned char tol, unsigned char tcl);
extern "C" __declspec(dllexport) int setParamsRCMAll (unsigned char tsr, unsigned char tdc, unsigned char tsm);
