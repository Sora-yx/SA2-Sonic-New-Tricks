#include "stdafx.h"

double SA1dashspeed = 0.65000001; //0.40000001;
double* SA1dashspeedptr = &SA1dashspeed;


void init_SpinDash() {

	if (sa1dash) {
		WriteData<2>((int*)0x723E19, 0x90); //remove spin dash delay
	}

	if (sa1speed) {
		WriteData((double**)0x725227, SA1dashspeedptr); //increase spin dash speed
	}
}