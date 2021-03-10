#include "stdafx.h"

Trampoline* CheckBreakObject_t;
Trampoline* Dynamite_t;
Trampoline* DynamiteHiddenBase_t;
Trampoline* DynamiteSandOcean_t;
Trampoline* PrisonLaneDoor_t;
Trampoline* PrisonLaneDoor4_t;
Trampoline* DoorIG_t;
Trampoline* DoorIG2_t;

bool isSpeedCharacter() {
	if (MainCharObj2[0]->CharID == Characters_Sonic || MainCharObj2[0]->CharID == Characters_Shadow || MainCharObj2[0]->CharID2 == Characters_MetalSonic || MainCharObj2[0]->CharID2 == Characters_Amy)
		return true;

	return false;
}

bool isSonicAttacking() {

	if (!isSpeedCharacter())
		return false;

	EntityData1* data1 = MainCharObj1[0];

	if (data1->Action == Action_SpinRelease || data1->Action == Action_Jump || data1->Action == Action_SpinCharge || data1->Action == Action_HomingAttack || data1->Action >= Action_Somersault1 && data1->Action <= Action_MovingSomersault1)
		return true;

	return false;
}

Bool __cdecl CheckBreakObject_r(ObjectMaster* obj, ObjectMaster* other)
{

	if (isSonicAttacking() && GetCollidingPlayer(obj))
		return 1;

	FunctionPointer(Bool, original, (ObjectMaster * obj, ObjectMaster * other), CheckBreakObject_t->Target());
	return original(obj, other);
}

void CheckBreakDynamite(ObjectMaster* obj) {

	EntityData1* data = obj->Data1.Entity;

	if (obj) {
		if (data->Action == 0 && isSonicAttacking() && GetCollidingPlayer(obj)) {
			data->Status |= 4u;
			obj->EntityData2->gap_44[0] = 0;
		}
	}

	ObjectFunc(origin, Dynamite_t->Target());
	origin(obj);
}

void CheckBreakDynamiteHiddenBase(ObjectMaster* obj) {

	EntityData1* data = obj->Data1.Entity;

	if (obj) {
		if (data->NextAction != 7 && isSonicAttacking() && GetCollidingPlayer(obj)) {
			data->field_6 = 0;
			data->NextAction = 7;
		}
	}

	ObjectFunc(origin, DynamiteHiddenBase_t->Target());
	origin(obj);
}

void CheckBreakDynamiteSandOcean(ObjectMaster* obj) {

	EntityData1* data = obj->Data1.Entity;

	if (obj) {
		if (data->Action == 0 && isSonicAttacking() && GetCollidingPlayer(obj)) {
			data->Status |= 4u;
			obj->EntityData2->gap_44[0] = 0;
		}
	}

	ObjectFunc(origin, DynamiteSandOcean_t->Target());
	origin(obj);
}

void CheckAndOpenPrisonLaneDoor(ObjectMaster* obj) {

	EntityData1* data = obj->Data1.Entity;
	EntityData2* data2 = obj->Data2.Entity;

	if (obj) {

		if (!isSpeedCharacter())
			return;

		if (data->Action == 0 && data->Rotation.x == 3)
		{
			data->Rotation.x = 32;
		}
		else  if (data->Action < 1 && GetCollidingPlayer(obj)) {
			data->Rotation.x = 3;
			data->Action = 1;
		}
	}
}


void CheckPrisonLaneDoor(ObjectMaster* obj) {

	CheckAndOpenPrisonLaneDoor(obj);

	ObjectFunc(origin, PrisonLaneDoor_t->Target());
	origin(obj);
}


void CheckPrisonLaneDoor4(ObjectMaster* obj) {

	CheckAndOpenPrisonLaneDoor(obj);

	ObjectFunc(origin, PrisonLaneDoor4_t->Target());
	origin(obj);
}

void CheckAndOpenIronGateDoor(ObjectMaster* obj) {

	if (!isSpeedCharacter())
		return;

	EntityData1* data = obj->Data1.Entity;

	if (GetCollidingPlayer(obj)) {
		data->NextAction = 15;
	}
}


void doorIG_r(ObjectMaster* obj) {

	CheckAndOpenIronGateDoor(obj);

	ObjectFunc(origin, DoorIG_t->Target());
	origin(obj);
}

void doorIG2_r(ObjectMaster* obj) {

	CheckAndOpenIronGateDoor(obj);

	ObjectFunc(origin, DoorIG2_t->Target());
	origin(obj);
}


static const void* const DrawChunkModelPtr = (void*)0x42E6C0;
static inline void DrawChunkModelASM(NJS_CNK_MODEL* a1)
{
	__asm
	{
		mov eax, [a1]
		call DrawChunkModelPtr
	}
}

bool isBallForm() {
	if (MainCharObj2[0]->CharID2 == Characters_Amy || MainCharObj2[0]->CharID2 == Characters_MetalSonic)
		return false;

	if (MainCharObj2[0]->AnimInfo.Current == 30 || MainCharObj2[0]->AnimInfo.Current >= 100 && MainCharObj2[0]->AnimInfo.Current <= 103 || MainCharObj2[0]->AnimInfo.Current >= 65 && MainCharObj2[0]->AnimInfo.Current <= 67)
	{
		return true;
	}

	return false;
}

void FixUpgradeDisplay(NJS_CNK_MODEL* a1) {

	if (isBallForm())
		return;

	return DrawChunkModelASM(a1);
}


void FixUpgradeDisplay2(NJS_OBJECT* a1) {

	if (isBallForm())
		return;

	return DrawObject(a1);
}
static void __declspec(naked) DrawChunkModel()
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


void Init_BetterSonic() {

	WriteData<2>((int*)0x723E19, 0x90); //remove spin dash delay

	Init_NewAnimation();

	CheckBreakObject_t = new Trampoline((int)CheckBreakObject, (int)CheckBreakObject + 0x7, CheckBreakObject_r);
	Dynamite_t = new Trampoline((int)Dynamite_Main, (int)Dynamite_Main + 0x5, CheckBreakDynamite);
	DynamiteHiddenBase_t = new Trampoline((int)DynamiteHiddenBase_Main, (int)DynamiteHiddenBase_Main + 0x5, CheckBreakDynamiteHiddenBase);
	DynamiteSandOcean_t = new Trampoline((int)DynamiteSandOcean_Main, (int)DynamiteSandOcean_Main + 0x6, CheckBreakDynamiteSandOcean);
	PrisonLaneDoor_t = new Trampoline((int)PrisonLaneDoor, (int)PrisonLaneDoor + 0x6, CheckPrisonLaneDoor);
	PrisonLaneDoor4_t = new Trampoline((int)PrisonLaneDoor4, (int)PrisonLaneDoor4 + 0x6, CheckPrisonLaneDoor4);

	DoorIG_t = new Trampoline((int)DoorIG, (int)DoorIG + 0x6, doorIG_r);
	DoorIG2_t = new Trampoline((int)DoorIG2, (int)DoorIG2 + 0x6, doorIG2_r);


	//Remove upgrade display when ball form
	WriteCall((void*)0x72080B, DrawChunkModel);	
	WriteCall((void*)0x72086C, DrawChunkModel);	
	WriteCall((void*)0x7208F1, DrawChunkModel);	
	WriteCall((void*)0x720991, DrawChunkModel);

	//This makes the game crash when playing Costum Character... will have to dig that eventually.
	/*WriteCall((void*)0x7209E2, FixUpgradeDisplay2);
	WriteCall((void*)0x720A0C, FixUpgradeDisplay2);
	WriteCall((void*)0x720A2C, FixUpgradeDisplay2);
	WriteCall((void*)0x720A59, FixUpgradeDisplay2);
	WriteCall((void*)0x720AA6, FixUpgradeDisplay2);*/

	WriteData<5>((int*)0x71AF71, 0x90); //remove amy grunt homing attack
}