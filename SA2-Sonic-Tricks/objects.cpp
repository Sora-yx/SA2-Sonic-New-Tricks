#include "stdafx.h"

//This page hack several objects from the game to make them working with Speed characters

FunctionHook<Bool, ObjectMaster*, ObjectMaster*> CheckBreakObject_t((intptr_t)CheckBreakObject);
TaskHook Dynamite_t(Dynamite_Main);
TaskHook DynamiteHiddenBase_t(DynamiteHiddenBase_Main);
TaskHook DynamiteSandOcean_t(DynamiteSandOcean_Main);
TaskHook MetalBox_t(MetalBox);
TaskHook MetalBoxGravity_t(MetalBoxGravity);

TaskHook PrisonLaneDoor_t(PrisonLaneDoor);
TaskHook PrisonLaneDoor4_t(PrisonLaneDoor4);
TaskHook DoorIG_t(DoorIG);
TaskHook DoorIG2_t(DoorIG2);
TaskHook RocketIG_t(RocketIG);

Trampoline* BrokenDownSmoke_t = nullptr;
Trampoline* LoadCharacters_t = nullptr;
static Trampoline* Init_LandColMemory_t = nullptr;
static Trampoline* Turtle_Function_t = nullptr;
static Trampoline* PowerSupply_event_t = nullptr;

void __cdecl PowerSupply_EventTask(ObjectMaster* a1)
{
	if (CurrentCharacter != Characters_MechEggman && CurrentCharacter != Characters_MechTails)
	{
		DeleteObject_(a1);
		return;
	}

	ObjectFunc(origin, PowerSupply_event_t->Target());
	origin(a1);
}

void PatchTurtleAnimation()
{
	if (CurrentLevel != LevelIDs_DryLagoon)
		return;

	if (CurrentCharacter != Characters_Knuckles && CurrentCharacter != Characters_Rouge)
	{
		WriteData<1>((int*)0x642c98, 0x0);
	}
	else
	{
		WriteData<1>((int*)0x642c98, 0xD7);
	}
}

void Turtle_Function_r(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;

	if (isSpeedCharacter()) {
		if (data->Action == 5) {
			if (SonicCO2PtrExtern) {
				char pnum = SonicCO2PtrExtern->base.PlayerNum;

				if (MainCharObj2[pnum])
				{
					if (((MainCharObj1[pnum]->Status & 1) != 0) && (MainCharObj2[pnum]->CurrentDyncolTask != nullptr))
					{
						if (MainCharObj2[pnum]->CurrentDyncolTask->Data1.Entity->field_2 == 25) {
							if (Action_Pressed[pnum])
								MainCharObj1[pnum]->Action = 86;
						}
					}
				}
			}
		}
	}

	ObjectFunc(origin, Turtle_Function_t->Target());
	origin(obj);
}

Bool __cdecl CheckBreakObject_r(ObjectMaster* obj, ObjectMaster* other)
{
	if (obj) {
		ObjectMaster* col = GetCollidingPlayer(obj);

		if (col)
		{
			char pnum = GetPlayerNumber(col);

			if (isAttackingBoxes(pnum))
				return 1;
		}
	}

	return CheckBreakObject_t.Original(obj, other);
}

void CheckBreakDynamite(ObjectMaster* obj) {
	EntityData1* data = obj->Data1.Entity;

	if (obj) {
		if (data) {
			if (data->Action == 0 && isSonicAttacking() && GetCollidingPlayer(obj)) {
				data->Status |= 4u;
				obj->EntityData2->gap_44[0] = 0;
			}
		}
	}

	Dynamite_t.Original(obj);
}

void CheckBreakDynamiteHiddenBase(ObjectMaster* obj) {
	EntityData1* data = obj->Data1.Entity;

	if (obj) {
		if (data) {
			if (data->NextAction != 7 && isSonicAttacking() && GetCollidingPlayer(obj)) {
				data->Timer = 0;
				data->NextAction = 7;
			}
		}
	}

	DynamiteHiddenBase_t.Original(obj);
}

void CheckBreakDynamiteSandOcean(ObjectMaster* obj) {
	EntityData1* data = obj->Data1.Entity;

	if (obj) {
		if (data) {
			if (data->Action == 0 && isSonicAttacking() && GetCollidingPlayer(obj)) {
				data->Status |= 4u;
				obj->EntityData2->gap_44[0] = 0;
			}
		}
	}

	DynamiteSandOcean_t.Original(obj);
}

void CheckAndOpenPrisonLaneDoor(ObjectMaster* obj) {
	EntityData1* data = obj->Data1.Entity;
	EntityData2* data2 = obj->Data2.Entity;

	if (obj) {
		if (!isSpeedCharacter())
			return;

		if (data) {
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
}

void CheckPrisonLaneDoor(ObjectMaster* obj) {
	CheckAndOpenPrisonLaneDoor(obj);

	PrisonLaneDoor_t.Original(obj);
}

void CheckPrisonLaneDoor4(ObjectMaster* obj) {
	CheckAndOpenPrisonLaneDoor(obj);

	PrisonLaneDoor4_t.Original(obj);
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

	DoorIG_t.Original(obj);
}

void doorIG2_r(ObjectMaster* obj) {
	CheckAndOpenIronGateDoor(obj);

	DoorIG2_t.Original(obj);
}

void rocketIG_r(ObjectMaster* obj) {
	EntityData1* data = obj->Data1.Entity;

	if (isSpeedCharacter()) {
		if (GetCollidingPlayer(obj) && data->Action == 5)
		{
			data->Action = 6;
		}
	}

	RocketIG_t.Original(obj);
}

void MetalBox_r(ObjectMaster* obj) {
	EntityData1* data = obj->Data1.Entity;

	if (data)
	{
		if (data->Collision) {
			if (data->Collision->CollidingObject) {
				char pnum = GetPlayerNumber(data->Collision->CollidingObject->Object);

				if (isAttackingMetalBoxes(pnum) && data->NextAction < 1)
				{
					data->Collision->CollisionArray->push |= 0x4000u;
					data->Timer = 1;
					AddScore(20);
					Play3DSound_Pos(4113, &data->Position, 1, 127, 80);
					data->NextAction = 1;
				}
			}
		}
	}

	MetalBox_t.Original(obj);
}

void MetalBoxGravity_r(ObjectMaster* obj) {
	EntityData1* data = obj->Data1.Entity;

	if (data)
	{
		if (data->Collision) {
			if (data->Collision->CollidingObject) {
				char pnum = GetPlayerNumber(data->Collision->CollidingObject->Object);

				if (isAttackingMetalBoxes(pnum) && data->NextAction < 1)
				{
					data->Collision->CollisionArray->push |= 0x4000u;
					data->Timer = 1;
					AddScore(20);
					Play3DSound_Pos(4113, &data->Position, 1, 127, 80);
					data->NextAction = 1;
				}
			}
		}
	}

	MetalBoxGravity_t.Original(obj);
}

void BrokenDownSmoke_r(ObjectMaster* a1) {
	if (MainCharObj2[0]->CharID != Characters_MechTails && MainCharObj2[0]->CharID != Characters_MechEggman)
		DeleteObject_(a1);
	else {
		ObjectFunc(origin, BrokenDownSmoke_t->Target());
		origin(a1);
	}
}

void CheckAndLoad_quickSandDeathAnimation() {
	if (CurrentLevel != LevelIDs_SandOcean && CurrentLevel != LevelIDs_HiddenBase)
		return;

	for (int i = 0; i < 2; i++) {
		if (MainCharObj1[i]) {
			if (MainCharObj2[i]->CharID <= Characters_Shadow)
				LoadEggGolemCharAnims(); //fix quicksand death crash
		}
	}
}

void CheckAndSetHackObject() {
	if (isSA2Miles())
		return;

	CharObj2Base* co2 = MainCharObj2[0];

	if (co2->CharID == Characters_MechTails || co2->CharID == Characters_MechEggman) {
		WriteData<1>((int*)0x715b58, 0x6);
		WriteData<1>((int*)0x715aa8, 0x6);
		WriteData<1>((int*)0x7158bf, 0x6);

		WriteData<1>((int*)0x79b427, 0x6);
		WriteData<1>((int*)0x79b959, 0x6);
		WriteData<1>((int*)0x79be57, 0x6);
		return; //if one player has a mech, we don't need to hack the door
	}

	//hack so non mech character can destroy the doors
	if (CurrentLevel == LevelIDs_HiddenBase) {
		//Hidden base door Col Stuff
		WriteData<1>((int*)0x715b58, 0x1);
		WriteData<1>((int*)0x715aa8, 0x1);
		WriteData<1>((int*)0x7158bf, 0x1);
	}

	if (CurrentLevel == LevelIDs_CannonsCoreT) {
		//CC door col Stuff
		WriteData<1>((int*)0x79b427, 0x1);
		WriteData<1>((int*)0x79b959, 0x1);
		WriteData<1>((int*)0x79be57, 0x1);
	}

	return;
}

//used to load object hack
void InitLandColMemory_r()
{
	bool isSpdChar = false;

	for (int i = 0; i < 4; i++) {
		if (MainCharObj2[i]) {
			if (MainCharObj2[i]->CharID == Characters_Sonic || MainCharObj2[i]->CharID == Characters_Shadow)
			{
				CheckAndLoad_quickSandDeathAnimation();
				CheckAndSetHackObject();
				PatchTurtleAnimation();

				if (CurrentLevel == LevelIDs_DryLagoon)
					LoadDryLagoonCharAnims();

				isSpdChar = true;
				break;
			}
		}
	}

	if (!isSpdChar)
	{
		SonicCO2PtrExtern = nullptr; //reset the pointer because the vanilla game never do lol
	}

	VoidFunc(origin, Init_LandColMemory_t->Target());
	origin();
}

static const void* const loc_6109eb = (void*)0x6109eb;
static const void* const loc_610b62 = (void*)0x610b62;
__declspec(naked) void  KBB_DamageChk() {
	if ((CurrentCharacter == Characters_Knuckles || CurrentCharacter == Characters_Rouge)
		&& (MainCharObj1[0]->Action == Action_DigFinish || MainCharObj1[0]->Action == Action_DigFinishOnWall)
		|| isAttackingKBB())
	{
		_asm jmp loc_6109eb
	}
	else {
		_asm jmp loc_610b62
	}
}

void Init_ObjectsHacks() {
	CheckBreakObject_t.Hook(CheckBreakObject_r);
	Dynamite_t.Hook(CheckBreakDynamite);
	DynamiteHiddenBase_t.Hook(CheckBreakDynamiteHiddenBase);
	DynamiteSandOcean_t.Hook(CheckBreakDynamiteSandOcean);
	MetalBox_t.Hook(MetalBox_r);
	MetalBoxGravity_t.Hook(MetalBoxGravity_r);

	PrisonLaneDoor_t.Hook(CheckPrisonLaneDoor);
	PrisonLaneDoor4_t.Hook(CheckPrisonLaneDoor4);

	DoorIG_t.Hook(doorIG_r);
	DoorIG2_t.Hook(doorIG2_r);
	RocketIG_t.Hook(rocketIG_r);

	WriteData<5>((int*)0x6cdf58, 0x90); //remove "speed nerf" when destroying boxes
	WriteData<5>((int*)0x6D6B99, 0x90);
	WriteData<5>((int*)0x77BFFB, 0x90);

	if (!isSA2Miles()) {
		BrokenDownSmoke_t = new Trampoline((int)BrokenDownSmokeExec, (int)BrokenDownSmokeExec + 0x7, BrokenDownSmoke_r);
		PowerSupply_event_t = new Trampoline((int)0x78A450, (int)0x78A455, PowerSupply_EventTask);
		WriteJump(reinterpret_cast<void*>(0x6109d5), KBB_DamageChk);
		WriteData<17>((int*)0x6109da, 0x90); //need 5 bytes to jump so we nop the rest and will add this code manually
	}

	Init_LandColMemory_t = new Trampoline((int)0x47BB50, (int)0x47BB57, InitLandColMemory_r);
	Turtle_Function_t = new Trampoline((int)0x642B10, (int)0x642B16, Turtle_Function_r);
}