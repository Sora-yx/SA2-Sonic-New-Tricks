#pragma once

extern bool sa1dash;
extern bool sa1SFX;
extern bool sonicBall;
extern bool shBall;
extern bool amyBall;
extern bool sonicGrunt;
extern bool shGrunt;
extern bool amyGrunt;
extern bool superBounce;
extern bool shBounce;


FunctionPointer(void, DrawObject, (NJS_OBJECT* a1), 0x42E730);
ObjectFunc(Dynamite_Main, 0x6da880);
ObjectFunc(DynamiteHiddenBase_Main, 0x714610);
ObjectFunc(DynamiteSandOcean_Main, 0x65AEA0);
ObjectFunc(PrisonLaneDoor, 0x606400);
ObjectFunc(PrisonLaneDoor4, 0x606A10);
ObjectFunc(DoorIG, 0x69D1D0);
ObjectFunc(DoorIG2, 0x69F460);
ObjectFunc(DoorCCThing, 0x79AFB0);
ObjectFunc(DoorHB, 0x715560);
ObjectFunc(MetalBox, 0x6D6490);
ObjectFunc(MetalBoxGravity, 0x77BB90);

FunctionPointer(void, PlaySound3, (int a1, int a2), 0x437DD0);

void Init_NewAnimation();
void Init_StartEndPos();
void Init_BetterSonic();
void Init_Helper();
void Init_Bounce();
void init_SpinDash();
