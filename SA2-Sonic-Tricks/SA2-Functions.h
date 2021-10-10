#pragma once

FunctionPointer(void, DrawObject, (NJS_OBJECT* a1), 0x42E730);
ObjectFunc(Dynamite_Main, 0x6da880);
ObjectFunc(DynamiteHiddenBase_Main, 0x714610);
ObjectFunc(DynamiteSandOcean_Main, 0x65AEA0);
ObjectFunc(PrisonLaneDoor, 0x606400);
ObjectFunc(PrisonLaneDoor4, 0x606A10);
ObjectFunc(DoorIG, 0x69D1D0);
ObjectFunc(DoorIG2, 0x69F460);
ObjectFunc(RocketIG, 0x6A10A0);
ObjectFunc(DoorCCThing, 0x79AFB0);
ObjectFunc(DoorHB, 0x715560);
ObjectFunc(MetalBox, 0x6D6490);
ObjectFunc(MetalBoxGravity, 0x77BB90);


static const void* const Sub4372E0Ptr = (void*)0x4372E0;
static inline char Play3DSoundPosThing(int id, NJS_VECTOR* pos, int a3, char a4, char a5)
{
	char result;
	__asm
	{
		push[a5]
		push[a4]
		push[a3]
		mov esi, [pos]
		mov edi, [id]
		call Sub4372E0Ptr
		add esp, 12
		mov result, al
	}
	return result;
}

static const void* const VibeThingPtr = (void*)0x438E70;
static inline void VibeThing(int a1, signed int a2, int a3, signed int a4)
{
	__asm
	{
		push[a4] // int a4
		mov ecx, a3 // a3
		mov edx, a2 // int a2
		mov eax, a1 // a1

		// Call your __cdecl function here:
		call VibeThingPtr
		add esp, 4 // int a4
	}
}

static const void* const somethingAboutTrickPtr = (void*)0x45ABE0;
static inline bool PlayerCheckBreakMaybe(int a1, EntityData1* a2, CharObj2Base* a3)
{
	int result;
	__asm
	{
		mov esi, [a3] // a3
		mov edi, [a2] // a2
		mov ecx, [a1] // a1

		// Call your __cdecl function here:
		call somethingAboutTrickPtr
		mov result, ecx
	}
	return result;
}

static const void* const somethingAboutTrick2Ptr = (void*)0x475100;
static inline signed int CheckPlayerStopASM(EntityData1* a1, CharObj2Base* a2, EntityData2* a4)
{
	signed int result;
	__asm
	{
		push[a4]
		mov esi, [a2] // a2
		mov eax, [a1] // a1

		// Call your __cdecl function here:
		call somethingAboutTrick2Ptr
		add esp, 4
		mov result, eax
	}
	return result;
}


