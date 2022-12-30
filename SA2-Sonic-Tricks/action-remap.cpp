#include "stdafx.h"

uint8_t actionRemapList[] = { Action_LightDash, Action_PickUp, Action_PutDown, Action_GrabObject2, Action_Pet, Action_GravitySwitch };

static UsercallFunc(signed int, Sonic_CheckActionWindow_t, (EntityData1* data1, EntityData2* data2, CharObj2Base* co2, SonicCharObj2* sonicCO2), (data1, data2, co2, sonicCO2), 0x7230E0, rEAX, rEAX, rEDX, rECX, stack4);
Trampoline* Sonic_Somersault_t = nullptr; //Can't funchook this for some reason

bool isCustomAction(char action)
{
	for (uint8_t i = 0; i < LengthOfArray(actionRemapList); i++)
	{
		if (actionRemapList[i] == action)
		{
			return true;
		}
	}

	return false;
}

bool isInputPressed(Buttons btn, char pnum)
{
	if ((((Controllers[pnum].press & btn) == 0) && btn != buttons_XB) || btn == buttons_XB && !Action_Pressed[pnum])
	{
		return false;
	}

	return true;
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

		if (isCustomAction(action))
		{
			co2->field_D[1] = action;

			switch (action)
			{
			case Action_LightDash:
				// Just nope right out of here if Light Dash button isn't pressed.
				if (!isInputPressed(LightDashButton, pnum))
				{
					return 0;
				}

				Sonic_PerformLightDash(sonicCO2, co2, data1);
				return 1; // Original function returns this value upon light dash.
			case Action_PickUp:
				if (!isInputPressed(pickButton, pnum))
				{
					return 0;
				}
				Sonic_DoPickObject(data1, co2);
				sonicCO2->SpindashCounter = 0;
				return 1;
			case Action_PutDown:
				if (!isInputPressed(putButton, pnum))
				{
					return 0;
				}
				PutDownObj(co2, data2, data1);
				sonicCO2->SpindashCounter = 0;
				return 1;
			case Action_GrabObject2:
				if (!isInputPressed(grabButton, pnum))
				{
					return 0;
				}
				data1->Action = Action_GrabObject2;
				co2->AnimInfo.Next = 43;
				data1->Status &= ~0x2500u;
				sonicCO2->SpindashCounter = 0;
				return 1;
			case Action_Pet:
				if (!isInputPressed(petButton, pnum))
				{
					return 0;
				}
				DoPetChao(co2, data1);
				return 1;
			case Action_GravitySwitch:
				if (!isInputPressed(gravityButton, pnum))
				{
					return 0;
				}
				data1->Action = Action_GravitySwitch;
				co2->AnimInfo.Next = 201;
				sonicCO2->SomersaultTime = 0;
				sonicCO2->SpindashCounter = 0;
				return 1;
			}
		}
	}

	// If all those conditions fail, let the original code handle it.
	return Sonic_CheckActionWindow_t.Original(data1, data2, co2, sonicCO2);
}

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
			co2->Speed.x = 0.0f;
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
		co2->Speed.x = 1.875f;
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
	signed int result;
	CollisionInfo* col;
	int flagCol;
	char somerSaultNextAction;
	char getSomersaultNextAction;
	char SomersaultNextAct;

	double getSpd;
	char getSomersaultNextAction2;
	int pnum;
	__int16 spinDashCount;
	__int16 timer;
	__int16 status = data->Status;

	if ((status & Status_HoldObject) != 0)
	{
		sonicCO2->SpindashCounter = 0;
		return 0;
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

	switch (data->Action)
	{
	case Action_Somersault1:

		if (Somersault_ApplyChanges(sonicCO2, data, co2, Action_MechPunch, Action_SomersaultFinish, flagCol))
			return 1;

		if ((co2->AnimInfo.field_C & 1) == 0)
		{
			return 0;
		}
		if (data->Position.y + 5.0f >= co2->SurfaceInfo.TopSurfaceDist
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
			co2->Speed.x = 2.5f;
			sonicCO2->SomersaultNextAction = 64;
			sonicCO2->SomersaultTime = 0;
			PlayerSomerSaultSoundMaybe(co2);
		}
		return 1;
	case Action_MechPunch: //idk

		if (Somersault_ApplyChanges(sonicCO2, data, co2, Action_Somersault2, Action_SomersaultFinish, flagCol))
			return 1;

		if ((co2->AnimInfo.field_C & 2) == 0)
		{
			return 0;
		}
		if (data->Position.y + 5.0f >= co2->SurfaceInfo.TopSurfaceDist
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
		getSpd = 2.5f;
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
			|| data->Position.y + 5.0f >= co2->SurfaceInfo.TopSurfaceDist
			&& (co2->SurfaceInfo.TopSurface & (SurfaceFlag_WaterNoAlpha | SurfaceFlag_Water)) == 0)
		{
			co2->AnimInfo.Next = 96;
			sonicCO2->SomersaultTime = 40;
			sonicCO2->SomersaultNextAction = Action_SomersaultFinish;

			if (!isHedgePannel())
				co2->Speed.x = 1.875f;

			PlayerSomerSaultSoundMaybe(co2);
			return 0;
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
		return 1;
	case Action_MovingSomersault2:

		if (Somersault_ApplyChanges(sonicCO2, data, co2, Action_MovingSomersaultFinish, Action_SomersaultFinish, flagCol))
			return 1;

		if ((co2->AnimInfo.field_C & 2) == 0)
		{
			return 0;
		}
		if (data->Position.y + 5.0f >= co2->SurfaceInfo.TopSurfaceDist
			&& (co2->SurfaceInfo.TopSurface & 0x2002) == 0)
		{
			SomersaultFinish2(co2, sonicCO2);
			return 0;
		}
		SomersaultNextAct = sonicCO2->SomersaultNextAction;
		data->Action = SomersaultNextAct;
		if (SomersaultNextAct == Action_MovingSomersaultFinish)
		{
			getSpd = 3.0f;
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
		if (co2->SurfaceInfo.TopSurfaceDist <= data->Position.y + 5.0f
			&& co2->SurfaceInfo.BottomSurfaceDist >= co2->SurfaceInfo.TopSurfaceDist - 5.0f
			&& co2->DynColInfo->tnorm.y > 0.9900000095367432f
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
	}

	// If all those conditions fail, let the original code handle it.
	return Sonic_Somersault_orig(sonicCO2, data, co2);
}

char SpinDash_Held[2] = { 0 };
char SpinDash_Released[2] = { 0 };

void SpinDash_ButtonCheckOnFrames()
{

	for (uint8_t i = 0; i < 2; i++)
	{
		if (Controllers[i].on & SpinDashButton)
		{
			SpinDash_Held[i] = 1;
		}
		else
		{
			SpinDash_Held[i] = 0;
		}

		if (Controllers[i].release & SpinDashButton)
		{
			SpinDash_Released[i] = 1;
		}
		else
		{
			SpinDash_Released[i] = 0;
		}
	}
}

char Pet_Held[2] = { 0 };
void Pet_ButtonCheckOnFrames()
{
	if (petButton == buttons_XB)
		return;

	if (CurrentCharacter > Characters_Shadow)
	{
		WriteData((char**)0x4758D4, &Action_Held);
	}
	else
	{
		WriteData((char**)0x4758D4, Pet_Held);
	}

	for (uint8_t i = 0; i < 2; i++)
	{
		if (Controllers[i].on & petButton)
		{
			Pet_Held[i] = 1;
		}
		else
		{
			Pet_Held[i] = 0;
		}
	}
}

void Buttons_CheckOnFrames()
{
	if (GameState != GameStates_Ingame)
		return;

	SpinDash_ButtonCheckOnFrames();
	Pet_ButtonCheckOnFrames();
}

static void __declspec(naked) Sonic_SomersaultASM()
{
	__asm
	{
		push[esp + 08h] 
		push[esp + 08h] 
		push eax 
		call Sonic_Somersault_r
		add esp, 4 
		add esp, 4 
		add esp, 4 
		retn
	}
}

void Init_ActionRemap() {

	if (LightDashButton != buttons_XB || pickButton != buttons_XB || petButton != buttons_XB || grabButton != buttons_XB
		|| gravityButton != buttons_XB || putButton != buttons_XB)
	{
		Sonic_CheckActionWindow_t.Hook(Sonic_CheckActionWindow_r);
	}

	if (SpinDashButton != buttons_XB || SomersaultButton != buttons_XB)
		Sonic_Somersault_t = new Trampoline((int)0x723880, (int)0x723885, Sonic_SomersaultASM);

	if (SpinDashButton != buttons_XB) {
		WriteData((char**)0x725e68, SpinDash_Released);
		WriteData((char**)0x7251b8, SpinDash_Held);
	}

	if (petButton != buttons_XB)
	{
		WriteData((char**)0x4758D4, Pet_Held);
	}

	if (SpinDashButton == Buttons_Y)
	{
		//release spin dash from XB (0x402 = 1026) to Y (0x200 = 502)
		WriteData<1>((int*)0x71a0fb, 0x2);
		WriteData<1>((int*)0x71a0fa, 0x0);
	}
}