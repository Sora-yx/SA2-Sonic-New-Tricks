#include "stdafx.h"

double SA1dashspeed = 0.65000001; //0.40000001;
double* SA1dashspeedptr = &SA1dashspeed;

TaskHook LightAttackParticle_Render_t(0x757B30);
TaskHook sub_757CD0_t(0x757CD0);

//fix MS Spin Dash crash
void __cdecl LightAttackParticle_Render_r(ObjectMaster* a1)
{
	if (!isSonicOrShadow())
	{
		return;
	}

	LightAttackParticle_Render_t.Original(a1);
}

void __cdecl sub_757CD0_r(ObjectMaster* a1)
{
	if (!isSonicOrShadow())
	{
		return;
	}

	sub_757CD0_t.Original(a1);
}

void init_SpinDash() {

	if (sa1dash) {
		WriteData<2>((int*)0x723E19, 0x90); //remove spin dash delay
	}

	if (sa1speed) {
		WriteData((double**)0x725227, SA1dashspeedptr); //increase spin dash speed
	}

	if (allowSpinDashMS)
	{
		LightAttackParticle_Render_t.Hook(LightAttackParticle_Render_r);
		sub_757CD0_t.Hook(sub_757CD0_r);
	}
}