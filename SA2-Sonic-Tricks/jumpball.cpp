#include "stdafx.h"

Trampoline* Sonic_CheckJump_t = nullptr;

static signed int Sonic_CheckJump_origin(EntityData1* data, CharObj2Base* co2, SonicCharObj2* a3)
{
    auto target = Sonic_CheckJump_t->Target();
    signed int result;

    __asm
    {
        push[a3]
        push[co2]
        mov eax, data
        call target
        add esp, 8
        mov result, eax
    }

    return result;
}


signed int Sonic_CheckJump_r(EntityData1* data, CharObj2Base* CO2, SonicCharObj2* a3)
{
    signed int isJump = Sonic_CheckJump_origin(data, CO2, a3);

    if (isJump != 0 && data->Action == Action_Jump && CO2->CharID2 <= Characters_Shadow)
    {
        data->Status |= Status_Ball;
    }

    return isJump;
}

static void __declspec(naked) Sonic_CheckJumpASM()
{
    __asm
    {
        push[esp + 08h]
        push[esp + 08h]
        push eax
        call Sonic_CheckJump_r
        add esp, 4 
        add esp, 4 
        add esp, 4 
        retn
    }
}


void Init_JumpBall()
{
    if (!sonicBall)
        return;

    Sonic_CheckJump_t = new Trampoline((int)0x721C00, (int)0x721C05, Sonic_CheckJumpASM);
}