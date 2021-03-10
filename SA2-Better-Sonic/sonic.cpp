#include "stdafx.h"


Trampoline* Sonic_Main_t;
Trampoline* Sonic_RunsAction_t;


void RestorePhysic(CharObj2Base* co2) {
	co2->PhysData.CollisionSize = PhysicsArray[0].CollisionSize;
	co2->PhysData.RippleSize = PhysicsArray[0].RippleSize;
	co2->PhysData.FloorGrip = PhysicsArray[0].FloorGrip;
	co2->PhysData.YOff = PhysicsArray[0].YOff;
	return;
}

//Apply Somersault physics/collision 
void SetPlayerSomersaultPhysics(CharObj2Base* co2, EntityData1* v1) {
	co2->PhysData.CollisionSize = PhysicsArray[0].CollisionSize * 0.4000000059604645;
	co2->PhysData.RippleSize = PhysicsArray[0].RippleSize * 0.4000000059604645;
	co2->PhysData.FloorGrip = PhysicsArray[0].FloorGrip * 0.4000000059604645;
	co2->PhysData.YOff = PhysicsArray[0].YOff * 0.4000000059604645;
	v1->Collision->CollisionArray->push &= ~0x4000u;
	return;
}


void __cdecl Sonic_runsAction_r(EntityData1* data1, EntityData2* data2, CharObj2Base* co2, SonicCharObj2* co2Sonic) {
	FunctionPointer(void, original, (EntityData1 * data1, EntityData2 * data2, CharObj2Base * co2, SonicCharObj2 * co2Sonic), Sonic_RunsAction_t->Target());
	original(data1, data2, co2, co2Sonic);

	if (data1->Action <= 1 || data1->Action > 4 && data1->Action < 61 || data1->Action == 64 || data1->Action == 65 || data1->Action > 68)
		RestorePhysic(co2);

}

void Sonic_Main_r(ObjectMaster* obj)
{
	ObjectFunc(origin, Sonic_Main_t->Target());
	origin(obj);

	CharObj2Base* co2 = MainCharObj2[obj->Data2.Character->PlayerNum];
	EntityData1* data1 = MainCharObj1[obj->Data2.Character->PlayerNum];

	//Add somersault physics to spin dash, this is very hacky, but that's how SA2 does it, so... lol.
	if (data1->Action == 3 || data1->Action == Action_SpinRelease || data1->Action >= 61 && data1->Action <= 63 || data1->Action >= 66 && data1->Action <= 68)
	{
		SetPlayerSomersaultPhysics(co2, data1);
	}
}



void Init_BetterSonic() {
	Sonic_Main_t = new Trampoline((int)Sonic_Main, (int)Sonic_Main + 0x6, Sonic_Main_r);
	Sonic_RunsAction_t = new Trampoline((int)Sonic_ChecksForDamage, (int)Sonic_ChecksForDamage + 0x8, Sonic_runsAction_r);

	Init_NewAnimation();

	WriteData<48>((int*)0x717a75, 0x90); //remove "restore physics" every frame, we will manually do it.


	if (!amyGrunt)
		WriteData<5>((int*)0x71AF71, 0x90);

	if (!sonicGrunt)
		WriteData<5>((int*)0x71AF97, 0x90);

}