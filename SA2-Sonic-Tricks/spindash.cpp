#include "stdafx.h"

double SA1dashspeed = 0.65000001; //0.40000001;
double* SA1dashspeedptr = &SA1dashspeed;

TaskHook LightAttackParticle_Render_t(0x757B30);
TaskHook sub_757CD0_t(0x757CD0);

//fix MS Spin Dash crash
void __cdecl LightAttackParticle_Render_r(ObjectMaster* a1)
{
	auto data = a1->Data1.Entity;

	if (data)
	{
		if (MainCharObj2[data->NextAction])
		{
			if (MainCharObj2[data->NextAction]->CharID2 <= Characters_Shadow)
			{
				LightAttackParticle_Render_t.Original(a1);
			}
		}
	}
}

void __cdecl sub_757CD0_r(ObjectMaster* a1)
{
	auto data = a1->Data1.Entity;

	if (data)
	{
		if (MainCharObj2[data->NextAction])
		{
			if (MainCharObj2[data->NextAction]->CharID2 <= Characters_Shadow)
			{
				sub_757CD0_t.Original(a1);
			}
		}
	}
}

void init_SpinDash() {

	if (sa1dash) {
		WriteData<2>((int*)0x723E19, 0x90); //remove spin dash delay
	}

	if (sa1speed) {
		WriteData((double**)0x725227, SA1dashspeedptr); //increase spin dash speed
	}

	HMODULE dashMod = GetModuleHandle(L"SA2-Amy-Metal-SpinDash");

	if (allowSpinDashMS && !dashMod)
	{
		LightAttackParticle_Render_t.Hook(LightAttackParticle_Render_r);
		sub_757CD0_t.Hook(sub_757CD0_r);
	}
}