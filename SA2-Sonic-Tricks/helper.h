#pragma once

extern bool sa1dash;
extern bool sa1speed;
extern bool sa1SFX;
extern bool jumpBall;
extern bool sonicGrunt;
extern bool shGrunt;
extern bool amyGrunt;
extern bool superBounce;
extern bool shBounce;

void Init_SonicNewTricks();
void Init_ObjectsHacks();
void Init_ActionRemap();

void init_SpinDash();
bool isBlackShield();
void Init_Bounce();
void Buttons_CheckOnFrames();