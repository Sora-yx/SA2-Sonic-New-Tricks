#include "stdafx.h"

Trampoline* Sonic_Main_t;

void RestorePhysic(CharObj2Base* co2) {
	co2->PhysData.Height = PhysicsArray[0].Height;
	co2->PhysData.Radius = PhysicsArray[0].Radius;
	co2->PhysData.FloorGrip = PhysicsArray[0].FloorGrip;
	co2->PhysData.CenterHeight = PhysicsArray[0].CenterHeight;
	return;
}

//Apply Somersault physics/collision 
void SetPlayerSomersaultPhysics(CharObj2Base* co2, EntityData1* v1) {
	co2->PhysData.Height = PhysicsArray[0].Height * 0.399888888888645; //0.4000000059604645;
	co2->PhysData.Radius = PhysicsArray[0].Radius * 0.399888888888645; //0.4000000059604645;
	co2->PhysData.FloorGrip = PhysicsArray[0].FloorGrip * 0.399888888888645; //0.4000000059604645;
	co2->PhysData.CenterHeight = PhysicsArray[0].CenterHeight * 0.399888888888645; //0.4000000059604645;
	v1->Collision->CollisionArray->push &= ~0x4000u;
	return;
}

void Sonic_Main_r(ObjectMaster* obj)
{

	ObjectFunc(origin, Sonic_Main_t->Target());
	origin(obj);

	CharObj2Base* co2 = MainCharObj2[obj->Data2.Character->PlayerNum];
	EntityData1* data1 = MainCharObj1[obj->Data2.Character->PlayerNum];


	//Add somersault physics to spin dash, this is very hacky, but that's how SA2 does it, so... lol.
	if (data1->Action == Action_SpinCharge || data1->Action == Action_SpinRelease || data1->Action >= 61 && data1->Action <= 63 || data1->Action >= 66 && data1->Action <= 68)
	{
		SetPlayerSomersaultPhysics(co2, data1);
	}
	else {
		RestorePhysic(co2);
	}
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
		push eax // a1

		// Call your __cdecl function here:
		call FixUpgradeDisplay

		pop eax // a1
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
		push edi // a1

		// Call your __cdecl function here:
		call DoSonTexEffect

		pop edi // a1
		retn
	}
}

void Init_SonicNewTricks() {

	Init_NewAnimation();
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

		WriteCall((void*)0x7185b5, DoSonicTextureEffectStuffASM);
	}

	if (!SpinDashSomersault)
		return;

	Sonic_Main_t = new Trampoline((int)Sonic_Main, (int)Sonic_Main + 0x6, Sonic_Main_r);
	WriteData<48>((int*)0x717a75, 0x90); //remove "restore physics" every frame, we will manually do it.
}