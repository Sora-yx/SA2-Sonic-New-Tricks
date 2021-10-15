#include "stdafx.h"

//Original Code by SonicFreak94, edited here to let the player change controls of the actions.

Trampoline* Sonic_CheckActionWindow_t;
Trampoline* Sonic_Somersault_t;

static Sint32 __cdecl Sonic_CheckActionWindow_orig(EntityData1* data1, EntityData2* data2, CharObj2Base* co2, SonicCharObj2* sonicCO2)
{
	auto target = Sonic_CheckActionWindow_t->Target();
	Sint32 result;
	__asm
	{
		push[sonicCO2]
		mov ecx, [co2]
		mov edx, [data2]
		mov eax, [data1]
		call target
		add esp, 4
		mov result, eax
	}
	return result;
}

static Sint32 __cdecl Sonic_CheckActionWindow_r(EntityData1* data1, EntityData2* data2, CharObj2Base* co2, SonicCharObj2* sonicCO2)
{

	// This code is based on the pseudocode of the original function
	int  pnum = co2->PlayerNum;
	char action = co2->field_D[0];
	int  count = co2->ActionWindowItemCount;
	int  i = 0;

	if (count)
	{
		if (count > 0)
		{
			do
			{
				if (co2->field_D[0] == co2->ActionWindowItems[i])
				{
					break;
				}
				++i;
			} while (i < count);
			action = co2->field_D[0];
		}

		if (count == i)
		{
			action = co2->ActionWindowItems[0];
		}

		if (action == Action_LightDash)
		{
			co2->field_D[1] = action;

			// Just nope right out of here if Light Dash button isn't pressed.
			if ((((Controllers[pnum].press & LightDashButton) == 0) && LightDashButton != buttons_XB) || LightDashButton == buttons_XB && !Action_Pressed[pnum])
			{
				return 0;
			}

			Sonic_PerformLightDash(sonicCO2, co2, data1);
			return 1; // Original function returns this value upon light dash.
		}
	}

	// If all those conditions fail, let the original code handle it.
	return Sonic_CheckActionWindow_orig(data1, data2, co2, sonicCO2);
}

static void __declspec(naked) Sonic_CheckActionWindowASM()
{
	__asm
	{
		push[esp + 04h] // a4
		push ecx // a3
		push edx // a2
		push eax // a1

		// Call your __cdecl function here:
		call Sonic_CheckActionWindow_r

		add esp, 4 // a1<eax> is also used for return value
		pop edx // a2
		pop ecx // a3
		add esp, 4 // a4
		retn
	}
}

//Ready??? Let's go, enjoy the giant mess!!!!!

static Sint32 __cdecl Sonic_Somersault_orig(SonicCharObj2* sonicCO2, EntityData1* data, CharObj2Base* co2)
{
	auto target2 = Sonic_Somersault_t->Target();

	Sint32 result;
	__asm
	{
		push[co2]
		push[data]
		mov eax, [sonicCO2]
		call target2
		add esp, 8
		mov result, eax
	}
	return result;
}

void SomersaultFinish(CharObj2Base* co2, SonicCharObj2* sonicCO2, EntityData1* data, float getSpd) {

	if (!isHedgePannel())
		co2->Speed.x = getSpd;

	int charID = co2->CharID2;
	int soundID;

	sonicCO2->SomersaultNextAction = Action_SomersaultFinish;
	sonicCO2->SomersaultTime = 0;
	PlaySoundProbably(8201, 0, 0, 0);
	if (CurrentLevel == LevelIDs_GreenHill)
	{
		VibeThing(0, 15, co2->PlayerNum, 6);
		return;
	}

	if (charID)
	{
		if (charID != Characters_Shadow)
		{
			VibeThing(0, 15, co2->PlayerNum, 6);
			return;
		}
		soundID = 12316;
	}
	else
	{
		soundID = 12298;
	}
	Play3DSound_Pos(soundID, &data->Position, 0, 0, 127);
	VibeThing(0, 15, co2->PlayerNum, 6);
	return;
}

void SomersaultFinish2(CharObj2Base* co2, SonicCharObj2* sonicCO2) {
	co2->AnimInfo.Next = 91;
	sonicCO2->SomersaultTime = 0;

	sonicCO2->SomersaultNextAction = Action_SomersaultFinish;

	if (!isHedgePannel())
		co2->Speed.x = 1.875;

	PlayerSomerSaultSoundMaybe(co2);
	return;
}

int Somersault_ApplyChanges(SonicCharObj2* sonicCO2, EntityData1* data, CharObj2Base* co2, char oldAction, char nextAction, int flagCol) {

	char pnum = co2->PlayerNum;

	bool isSomersaultPressed = (SomersaultButton != buttons_XB && ((Controllers[pnum].press & SomersaultButton)));
	bool isSomersaultHeld = (SomersaultButton != buttons_XB && ((Controllers[pnum].on & SomersaultButton)));
	bool oldInputSomersaultHeld = (SomersaultButton == buttons_XB && Action_Held[pnum]);
	bool oldInputSomersaultPressed = (SomersaultButton == buttons_XB && Action_Pressed[pnum]);

	++sonicCO2->SomersaultTime;
	if (flagCol == 1)
	{
		if (!isHedgePannel())
			co2->Speed.x = 0.0;
	}

	if (Jump_Pressed[pnum])
	{
		sub_723690(sonicCO2, co2, data);
		return 1;
	}
	if (oldInputSomersaultPressed || isSomersaultPressed)
	{
		sonicCO2->SomersaultNextAction = oldAction;
	}
	else if (sonicCO2->SomersaultNextAction != oldAction && (!oldInputSomersaultHeld || !isSomersaultHeld))
	{
		sonicCO2->SomersaultNextAction = nextAction;
	}

	return 0;
}

void ResetSomersault(SonicCharObj2* sonicCO2, CharObj2Base* co2, EntityData1* data) {
	int curFlag;
	int charID = co2->CharID2;
	if (co2->PhysData.RunSpeed < (double)co2->Speed.x)
	{
		Sonic_DoObstacleSomersault(data, sonicCO2, co2);
	}
	else
	{
		data->Action = Action_Somersault1;
		co2->AnimInfo.Next = 91;
		data->Status |= Status_Attack;
		sonicCO2->SomersaultTime = 0;
		sonicCO2->SomersaultNextAction = 3;
		co2->Speed.x = 1.875;
	}
	if (CurrentLevel == LevelIDs_GreenHill)
	{
		VibeThing(0, 15, co2->PlayerNum, 6);
		sonicCO2->SpindashCounter = 0;
		return;
	}

	if (charID == Characters_Amy)
	{
		curFlag = 0x3021;
	}
	else if (charID)
	{
		if (charID != Characters_Shadow)
		{
			VibeThing(0, 15, co2->PlayerNum, 6);
			sonicCO2->SpindashCounter = 0;
			return;
		}
		curFlag = 0x3012;
	}
	else
	{
		curFlag = 0x3000;
	}
	Play3DSound_Pos(curFlag, &data->Position, 0, 0, 127);
	VibeThing(0, 15, co2->PlayerNum, 6);
	sonicCO2->SpindashCounter = 0;
}

//This actually also manage the spin dash
signed int Sonic_Somersault_r(SonicCharObj2* sonicCO2, EntityData1* data, CharObj2Base* co2)
{
	signed int result; // eax
	CollisionInfo* col; // eax
	int flagCol; // eax
	int pNum; // eax
	char somerSaultNextAction; // al
	char getSomersaultNextAction; // al
	char SomersaultNextAct;

	double getSpd; // st7
	char getSomersaultNextAction2;
	int pnum; // esi
	__int16 spinDashCount; // ax
	__int16 timer; // ax
	__int16 status = data->Status;

	if ((status & Status_HoldObject) != 0)
	{
		result = 0;
		sonicCO2->SpindashCounter = 0;
		return result;
	}

	if ((status & (Status_OnObjectColli | Status_Ground)) == 0
		|| ((col = data->Collision->CollidingObject) == 0 || col->Object->Data1.Entity->field_2 != 21 ? (flagCol = 0) : (flagCol = 1),
			CurrentLevel == LevelIDs_ChaoWorld))
	{
		sonicCO2->SpindashCounter = 0;
		return 0;
	}

	pnum = co2->PlayerNum;

	bool isSomersaultPressed = (SomersaultButton != buttons_XB && ((Controllers[pnum].press & SomersaultButton)));
	bool isSpinDashPressed = (SpinDashButton != buttons_XB && ((Controllers[pnum].press & SpinDashButton)));
	bool isSpinDashHeld = (SpinDashButton != buttons_XB && ((Controllers[pnum].on & SpinDashButton)));
	bool isSomersaultHeld = (SomersaultButton != buttons_XB && ((Controllers[pnum].on & SomersaultButton)));
	bool oldInputSomersaultHeld = (SomersaultButton == buttons_XB && Action_Held[pnum]);
	bool oldInputSomersaultPressed = (SomersaultButton == buttons_XB && Action_Pressed[pnum]);

	switch (data->Action)
	{
	case Action_Somersault1:

		if (Somersault_ApplyChanges(sonicCO2, data, co2, Action_MechPunch, Action_SomersaultFinish, flagCol))
			return 1;

		if ((co2->AnimInfo.field_C & 1) == 0)
		{
			return 0;
		}
		if (data->Position.y + 5.0 >= co2->SurfaceInfo.TopSurfaceDist
			&& (co2->SurfaceInfo.TopSurface & (SurfaceFlag_WaterNoAlpha | SurfaceFlag_Water)) == 0)
		{
			SomersaultFinish2(co2, sonicCO2);
			return 0;
		}
		somerSaultNextAction = sonicCO2->SomersaultNextAction;
		data->Action = somerSaultNextAction;
		if (somerSaultNextAction == 64)
		{
			sub_7235C0(co2, data, sonicCO2);
		}
		else
		{
			co2->Speed.x = 2.5;
			sonicCO2->SomersaultNextAction = 64;
			sonicCO2->SomersaultTime = 0;
			PlayerSomerSaultSoundMaybe(co2);
		}
		return 1;
		break;
	case Action_MechPunch: //idk

		if (Somersault_ApplyChanges(sonicCO2, data, co2, Action_Somersault2, Action_SomersaultFinish, flagCol))
			return 1;

		if ((co2->AnimInfo.field_C & 2) == 0)
		{
			return 0;
		}
		if (data->Position.y + 5.0 >= co2->SurfaceInfo.TopSurfaceDist
			&& (co2->SurfaceInfo.TopSurface & (SurfaceFlag_WaterNoAlpha | SurfaceFlag_Water)) == 0)
		{
			SomersaultFinish2(co2, sonicCO2);
			return 0;
		}
		getSomersaultNextAction2 = sonicCO2->SomersaultNextAction;
		data->Action = getSomersaultNextAction2;
		if (getSomersaultNextAction2 != Action_Somersault2)
		{
			sub_7235C0(co2, data, sonicCO2);
			return 1;
		}
		getSpd = 2.5;
		SomersaultFinish(co2, sonicCO2, data, getSpd);
		return 1;
	case Action_Somersault2:
		break;
	case Action_MovingSomersault1:

		if (Somersault_ApplyChanges(sonicCO2, data, co2, Action_MovingSomersault2, Action_SomersaultFinish, flagCol))
			return 1;

		if ((co2->AnimInfo.field_C & 1) == 0)
		{
			return 0;
		}
		if (sonicCO2->SomersaultTime <= 41
			|| data->Position.y + 5.0 >= co2->SurfaceInfo.TopSurfaceDist
			&& (co2->SurfaceInfo.TopSurface & (SurfaceFlag_WaterNoAlpha | SurfaceFlag_Water)) == 0)
		{
			co2->AnimInfo.Next = 96;
			sonicCO2->SomersaultTime = 40;
			sonicCO2->SomersaultNextAction = Action_SomersaultFinish;
			if (!isHedgePannel())
				co2->Speed.x = 1.875;
			PlayerSomerSaultSoundMaybe(co2);
			result = 0;
			return result;
		}
		getSomersaultNextAction = sonicCO2->SomersaultNextAction;
		data->Action = getSomersaultNextAction;


		if (getSomersaultNextAction == Action_SomersaultFinish)
		{
			sub_7235C0(co2, data, sonicCO2);
		}
		else
		{
			if (!isHedgePannel())
				co2->Speed.x = 3.125;

			sonicCO2->SomersaultNextAction = Action_SomersaultFinish;
			sonicCO2->SomersaultTime = 0;
			PlayerSomerSaultSoundMaybe(co2);
		}
		result = 1;

		return result;
	case Action_MovingSomersault2:

		if (Somersault_ApplyChanges(sonicCO2, data, co2, Action_MovingSomersaultFinish, Action_SomersaultFinish, flagCol))
			return 1;

		if ((co2->AnimInfo.field_C & 2) == 0)
		{
			return 0;
		}
		if (data->Position.y + 5.0 >= co2->SurfaceInfo.TopSurfaceDist
			&& (co2->SurfaceInfo.TopSurface & 0x2002) == 0)
		{
			SomersaultFinish2(co2, sonicCO2);
			return 0;
		}
		SomersaultNextAct = sonicCO2->SomersaultNextAction;
		data->Action = SomersaultNextAct;
		if (SomersaultNextAct == Action_MovingSomersaultFinish)
		{
			getSpd = 3.0;
			SomersaultFinish(co2, sonicCO2, data, getSpd);
			return 1;
		}

		sub_7235C0(co2, data, sonicCO2);
		return 1;
	case Action_MovingSomersaultFinish:
		break;
	default:

		if (((SpinDashButton == buttons_XB || SomersaultButton == buttons_XB) && Action_Pressed[pnum])
			|| isSpinDashPressed || isSomersaultPressed)
		{
			sonicCO2->SpindashCounter = 1;
			return 0;
		}

		spinDashCount = sonicCO2->SpindashCounter;
		timer = spinDashCount + 1;

		if ((SpinDashButton == buttons_XB && Action_Held[pnum]) || isSpinDashHeld)
		{
			if (spinDashCount >= 1)
			{
				sonicCO2->SpindashCounter = timer;
				if (!sa1dash && timer <= 24)
				{
					data->Status |= 0x400u;
					result = 0;
				}
				else
				{
					if (co2->CharID2 <= Characters_Shadow || isAmySpinDash())
					{
						Sonic_StartSpindash(data, co2, sonicCO2);
					}
					sonicCO2->SpindashCounter = 0;
					result = 1;
				}
				return result;
			}
		}  //else if (sonicCO2->SpindashCounter > 2)
		else if ((SomersaultButton == buttons_XB && Action_Held[pnum]) || isSomersaultHeld)
		{
			ResetSomersault(sonicCO2, co2, data);
			return 1;
		}
		if (co2->SurfaceInfo.TopSurfaceDist <= data->Position.y + 5.0
			&& co2->SurfaceInfo.BottomSurfaceDist >= co2->SurfaceInfo.TopSurfaceDist - 5.0
			&& co2->DynColInfo->tnorm.y > 0.9900000095367432
			&& (co2->SurfaceInfo.TopSurface & (SurfaceFlag_WaterNoAlpha | SurfaceFlag_Water)) == 0)
		{
			ResetSomersault(sonicCO2, co2, data);
			return 1;
		}
		sonicCO2->SpindashCounter = 0;
		if (data->Action == Action_SomersaultFinish)
		{
			data->Status &= ~Status_Attack;
		}
		return 0;
	}

	// If all those conditions fail, let the original code handle it.
	return Sonic_Somersault_orig(sonicCO2, data, co2);
}

static void __declspec(naked) Sonic_SomersaultASM()
{
	__asm
	{
		push[esp + 08h] // co2
		push[esp + 08h] // data
		push eax // sonicCO2

		// Call your __cdecl function here:
		call Sonic_Somersault_r

		add esp, 4 // sonicCO2<eax> is also used for return value
		add esp, 4 // data
		add esp, 4 // co2
		retn
	}
}


void Init_ActionRemap() {

	if (LightDashButton != buttons_XB)
		Sonic_CheckActionWindow_t = new Trampoline((int)0x7230E0, (int)0x7230E5, Sonic_CheckActionWindowASM);

	if (SpinDashButton != buttons_XB || SomersaultButton != buttons_XB)
		Sonic_Somersault_t = new Trampoline((int)0x723880, (int)0x723885, Sonic_SomersaultASM);

	return;
}