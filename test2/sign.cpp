#include <iostream>
#include <math.h>
#include "sign.h"
using namespace std;

int sign(float x){
	int y;
	if (x>0){
		y=1;
	}
	else if (x==0){
		y=0;
	}
	else {
		y=-1;
	}
	return y;
}
