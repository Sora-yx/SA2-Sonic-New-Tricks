#include "stdafx.h"

TaskHook Sonic_Exec_t(Sonic_Main);
FunctionHook<void, EntityData1*, EntityData2*, CharObj2Base*, SonicCharObj2*> Sonic_runsActions_t((intptr_t)Sonic_runsActions);

void RestorePhysic(CharObj2Base* co2) {
	co2->PhysData.Height = PhysicsArray[0].Height;
	co2->PhysData.Radius = PhysicsArray[0].Radius;
	co2->PhysData.FloorGrip = PhysicsArray[0].FloorGrip;
	co2->PhysData.CenterHeight = PhysicsArray[0].CenterHeight;
	return;
}

//Apply Somersault physics/collision
void SetPlayerSomersaultPhysics(CharObj2Base* co2, EntityData1* v1) {
	co2->PhysData.Height = PhysicsArray[0].Height * 0.399888888888645f; //0.4000000059604645;
	co2->PhysData.Radius = PhysicsArray[0].Radius * 0.399888888888645f; //0.4000000059604645;
	co2->PhysData.FloorGrip = PhysicsArray[0].FloorGrip * 0.399888888888645f; //0.4000000059604645;
	co2->PhysData.CenterHeight = PhysicsArray[0].CenterHeight * 0.399888888888645f; //0.4000000059604645;
	v1->Collision->CollisionArray->push &= ~0x4000u;
	return;
}

void Sonic_Main_r(ObjectMaster* obj)
{
	if (obj && obj->Data1.Entity) {
		CharObj2Base* co2 = MainCharObj2[obj->Data2.Character->PlayerNum];
		EntityData1* data1 = MainCharObj1[obj->Data2.Character->PlayerNum];
		SonicCharObj2* sCO2 = (SonicCharObj2*)obj->Data2.Undefined;

		if (SpinDashSomersault)
		{
			//Add somersault physics to spin dash, this is very hacky, but that's how SA2 does it, so... lol.
			if (data1->Action == Action_SpinCharge || data1->Action == Action_SpinRelease || data1->Action >= 61 && data1->Action <= 63 || data1->Action >= 66 && data1->Action <= 68)
			{
				SetPlayerSomersaultPhysics(co2, data1);
			}
			else
			{
				RestorePhysic(co2);
			}
		}
	}

	Sonic_Exec_t.Original(obj);
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

void __cdecl Sonic_runsActions_r(EntityData1* data1, EntityData2* data2, CharObj2Base* co2, SonicCharObj2* SonicCO2)
{
	if (data1->Action == 86) //turtle
	{
		if (Sonic_CheckNextAction(SonicCO2, data1, data2, co2))
		{
			return;
		}
	}

	Sonic_runsActions_t.Original(data1, data2, co2, SonicCO2);
}

void Init_SonicNewTricks() {
	Init_Bounce();
	init_SpinDash();

	if (!amyGrunt)
		WriteData<5>((int*)0x71AF71, 0x90);

	if (!shGrunt)
		WriteData<5>((int*)0x71B3EC, 0x90);

	if (!sonicGrunt)
		WriteData<5>((int*)0x71AF97, 0x90);

	if (sonicBall) {
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

		//make sonic still spinning while falling with the jump ball
		SonicAnimList[67] = SonicAnimList[66];
		SonicAnimList[67].NextAnimation = 68;
		SonicAnimList[68] = SonicAnimList[66];
		SonicAnimList[68].NextAnimation = 68;


		WriteCall((void*)0x7185b5, DoSonicTextureEffectStuffASM);
	}

	Sonic_runsActions_t.Hook(Sonic_runsActions_r);

	if (SpinDashSomersault) {
		WriteData<48>((int*)0x717a75, 0x90); //remove "restore physics" every frame, we will manually do it.
		Sonic_Exec_t.Hook(Sonic_Main_r);
	}
}