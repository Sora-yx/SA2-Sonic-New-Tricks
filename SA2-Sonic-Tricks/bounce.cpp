#include "stdafx.h"

float bounceSpeed = 4.0000001f;
float bounceSpeed2 = 4.5999999f;

float* bouncespeedPtr = &bounceSpeed;
float* bouncespeed2Ptr = &bounceSpeed2;

signed int Sonic_CheckBounceAttack_r(CharObj2Base* a1, EntityData1* a2, SonicCharObj2* a3)
{
	if (!Action_Pressed[a1->PlayerNum] && BounceButton == buttons_XB || BounceButton != buttons_XB && ((Controllers[a1->PlayerNum].press & BounceButton) == 0) || a1->CharID2 == Characters_MetalSonic && !isBlackShield())
		return 0;

	if ((a1->Upgrades & Upgrades_SonicBounceBracelet) == 0)
	{
		if (!shBounce || shBounce && a1->CharID != Characters_Shadow)
			return 0;
	}

	a1->AnimInfo.Next = 30;
	a2->Action = Action_BounceDown;
	a2->Status |= 0x500u;
	if (superBounce)
		a1->Speed.y = -7.0;
	else
		a1->Speed.y = -5.0;
	a3->SomersaultTime = 0;
	PlaySoundProbably(8202, 0, 0, 0);
	return 1;
}

static void __declspec(naked) Sonic_CheckBounceAttackASM()
{
	__asm
	{
		push[esp + 04h]
		push ecx
		push eax
		call Sonic_CheckBounceAttack_r
		add esp, 4
		pop ecx
		add esp, 4
		retn
	}
}

signed int Sonic_PerformBounce_r(CharObj2Base* a1, EntityData1* a2)
{
	if (!Action_Pressed[a1->PlayerNum] && BounceButton == buttons_XB || BounceButton != buttons_XB && ((Controllers[a1->PlayerNum].press & BounceButton) == 0) || a1->CharID2 == Characters_MetalSonic && !isBlackShield())
		return 0;

	if ((a1->Upgrades & Upgrades_SonicBounceBracelet) == 0)
	{
		if (!shBounce || shBounce && a1->CharID != Characters_Shadow)
			return 0;
	}

	a1->AnimInfo.Next = 30;
	a2->Status |= Status_Attack | Status_Ball;
	a2->Action = Action_BounceDown;
	a1->Speed.y = -7.0;
	PlaySoundProbably(8202, 0, 0, 0);
	return 1;
}

static void __declspec(naked) Sonic_PerformBounceASM()
{
	__asm
	{
		push ecx
		push eax
		call Sonic_PerformBounce_r
		add esp, 4
		pop ecx
		retn
	}
}

void Init_Bounce() {
	if (superBounce)
	{
		WriteData((float**)0x71b0da, bouncespeedPtr);
		WriteData((float**)0x71b0f6, bouncespeed2Ptr);
	}

	WriteJump((void*)0x725270, Sonic_CheckBounceAttackASM);
	WriteJump((void*)0x7252D0, Sonic_PerformBounceASM);
	return;
}