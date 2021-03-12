#include "stdafx.h"

double SA1dashspeed = 0.95; //0.40000001;
double* SA1dashspeedptr = &SA1dashspeed;


static const void* const PlaySound3DPtr = (void*)0x4374D0;
static inline void PlaySound3DASM(int a1, EntityData1* a2, char a3, char a4, __int16 a5)
{
    __asm
    {
        push[a5]
        push[a4]
        push[a3]
        push[a2]
        mov edi, [a1]
        call PlaySound3DPtr
        add esp, 16
    }
}


void PlaySpinDashSoundR(int a1, EntityData1* a2, char a3, char a4, __int16 a5) {
    if (!is2PCharacter())
        PlaySound3DASM(8209, a2, a3, a4, a5);
}

static void __declspec(naked) PlaySpinDashSound()
{
    __asm
    {
        push[esp + 10h] // a5
        push[esp + 10h] // a4
        push[esp + 10h] // a3
        push[esp + 10h] // a2
        push edi // a1

        // Call your __cdecl function here:
        call PlaySpinDashSoundR

        pop edi // a1
        add esp, 4 // a2
        add esp, 4 // a3
        add esp, 4 // a4
        add esp, 4 // a5
        retn
    }
}



void init_SpinDash() {
	if (sa1dash) {
		WriteData<2>((int*)0x723E19, 0x90); //remove spin dash delay
		WriteData((double**)0x725227, SA1dashspeedptr); //increase spin dash speed
	}

    if (sa1SFX)
        WriteCall((void*)0x725248, PlaySpinDashSound);
}