#include "stdafx.h"


//Original Code by SonicFreak94, edited here to let the player change controls of the actions.

Trampoline* Sonic_CheckActionWindow_t;

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
			if (( ((Controllers[pnum].press & LightDashButton) == 0) && LightDashButton != buttons_XB) || LightDashButton == buttons_XB && !Action_Pressed[pnum]) 
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


void Init_ActionRemap() {

	Sonic_CheckActionWindow_t = new Trampoline((int)0x7230E0, (int)0x7230E5, Sonic_CheckActionWindowASM);
}