#include "stdafx.h"

static UsercallFunc(signed int, Sonic_CheckJump_t, (EntityData1* a1, CharObj2Base* a2, SonicCharObj2* a3), (a1, a2, a3), 0x721C00, rEAX, rEAX, stack4, stack4);
TaskHook sub_756AA0_t(0x756AA0);

void sub_756AA0_r(ObjectMaster* obj)
{
	if (isSonicOrShadow())
	{
		obj->MainSub = DeleteObject_;
		obj->DisplaySub_Delayed1 = 0;
		return;
	}

	sub_756AA0_t.Original(obj);
}

signed int Sonic_CheckJump_r(EntityData1* data, CharObj2Base* CO2, SonicCharObj2* a3)
{
	signed int isJump = Sonic_CheckJump_t.Original(data, CO2, a3);

	if (isJump != 0 && data->Action == Action_Jump && CO2->CharID2 <= Characters_Shadow)
	{
		data->Status |= Status_Ball;
	}

	return isJump;
}


void FixUpgradeDisplay(NJS_CNK_MODEL* a1) {
	if (isBallForm())
		return;

	return DrawChunkModel(a1);
}

static void __declspec(naked) CheckDrawUpgradeModel()
{
	__asm
	{
		push eax
		call FixUpgradeDisplay
		pop eax
		retn
	}
}

void FixUpgradeDisplay2(NJS_OBJECT* a1) {
	if (isBallForm())
		return;

	return DrawObject(a1);
}

static void DoSonTexEffect(ObjectMaster* a1)
{
	if (!isBallForm())
		DoSonicTextureEffectStuff(a1);
}

static void __declspec(naked) DoSonicTextureEffectStuffASM()
{
	__asm
	{
		push edi
		call DoSonTexEffect
		pop edi
		retn
	}
}

void Init_JumpBall()
{
	if (!jumpBall)
		return;

	Sonic_CheckJump_t.Hook(Sonic_CheckJump_r);

	//Remove upgrade display when ball form
	WriteCall((void*)0x72080B, CheckDrawUpgradeModel);
	WriteCall((void*)0x72086C, CheckDrawUpgradeModel);
	WriteCall((void*)0x7208F1, CheckDrawUpgradeModel);
	WriteCall((void*)0x720991, CheckDrawUpgradeModel);

	WriteCall((void*)0x7209E2, FixUpgradeDisplay2);
	WriteCall((void*)0x720A0C, FixUpgradeDisplay2);
	WriteCall((void*)0x720A2C, FixUpgradeDisplay2);

	WriteCall((void*)0x720A59, FixUpgradeDisplay2);
	WriteCall((void*)0x720A7A, FixUpgradeDisplay2);

	sub_756AA0_t.Hook(sub_756AA0_r); //prevent aura to show up when jumping

	//make sonic and shadow still spinning while falling with the jump ball
	SonicAnimList[67] = SonicAnimList[66];
	SonicAnimList[67].NextAnimation = 68;
	SonicAnimList[68] = SonicAnimList[66];
	SonicAnimList[68].NextAnimation = 68;

	ShadowAnimList[67] = ShadowAnimList[66];
	ShadowAnimList[67].NextAnimation = 68;
	ShadowAnimList[68] = ShadowAnimList[66];
	ShadowAnimList[68].NextAnimation = 68;
}