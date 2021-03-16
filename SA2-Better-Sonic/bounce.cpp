#include "stdafx.h"

float bounceSpeed = 4.0000001;
float bounceSpeed2 = 4.5999999;

float* bouncespeedPtr = &bounceSpeed;
float* bouncespeed2Ptr = &bounceSpeed2;


signed int Sonic_CheckBounceAttack_r(CharObj2Base* a1, EntityData1* a2, SonicCharObj2* a3)
{

	if (!Action_Pressed[a1->PlayerNum] || a1->CharID2 == Characters_MetalSonic && !isBlackShield())
		return 0;

	if ((a1->Upgrades & Upgrades_SonicBounceBracelet) == 0)
	{
		if (!shBounce || shBounce && a1->CharID != Characters_Shadow)
			return 0;
	}

	a1->AnimInfo.Next = 30;
	a2->Action = 69;
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
		push[esp + 04h] // a3
		push ecx // a2
		push eax // a1

		// Call your __cdecl function here:
		call Sonic_CheckBounceAttack_r

		add esp, 4 // a1<eax> is also used for return value
		pop ecx // a2
		add esp, 4 // a3
		retn
	}
}


signed int Sonic_PerformBounce_r(CharObj2Base* a1, EntityData1* a2)
{
	if (!Action_Pressed[a1->PlayerNum] || a1->CharID2 == Characters_MetalSonic && !isBlackShield())
		return 0;

	if ((a1->Upgrades & Upgrades_SonicBounceBracelet) == 0)
	{
		if (!shBounce || shBounce && a1->CharID != Characters_Shadow)
			return 0;
	}

	a1->AnimInfo.Next = 30;
	a2->Status |= Status_Attack | Status_Ball;
	a2->Action = 69;
	a1->Speed.y = -7.0;
	PlaySoundProbably(8202, 0, 0, 0);
	return 1;
}

static void __declspec(naked) Sonic_PerformBounceASM()
{
	__asm
	{
		push ecx // a2
		push eax // a1

		// Call your __cdecl function here:
		call Sonic_PerformBounce_r

		add esp, 4 // a1<eax> is also used for return value
		pop ecx // a2
		retn
	}
}


void Init_Bounce() {

	if (superBounce)
	{
		WriteData((float**)0x71b0da, bouncespeedPtr);
		WriteData((float**)0x71b0f6, bouncespeed2Ptr);
	}

	if (superBounce || shBounce)
	{
		WriteJump((void*)0x725270, Sonic_CheckBounceAttackASM);
		WriteJump((void*)0x7252D0, Sonic_PerformBounceASM);
	}
}