#include "stdafx.h"


bool CheckChara() {

	if (MainCharObj2[0]->CharID == Characters_Sonic && MainCharObj2[0]->CharID2 != Characters_Amy && sonicBall)
		return true;


	return false;
}