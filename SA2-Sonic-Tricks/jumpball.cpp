#include "stdafx.h"

static UsercallFunc(signed int, Sonic_CheckJump_t, (EntityData1* a1, CharObj2Base* a2, SonicCharObj2* a3), (a1, a2, a3), 0x721C00, rEAX, rEAX, stack4, stack4);

signed int Sonic_CheckJump_r(EntityData1* data, CharObj2Base* CO2, SonicCharObj2* a3)
{
	signed int isJump = Sonic_CheckJump_t.Original(data, CO2, a3);

	if (isJump != 0 && data->Action == Action_Jump && CO2->CharID2 <= Characters_Shadow)
	{
		data->Status |= Status_Ball;
	}

	return isJump;
}

void Init_JumpBall()
{
	if (!sonicBall)
		return;

	Sonic_CheckJump_t.Hook(Sonic_CheckJump_r);
}