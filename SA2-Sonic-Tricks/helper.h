#pragma once

extern bool sa1dash;
extern bool sa1speed;
extern bool sa1SFX;
extern bool sonicBall;
extern bool sonicGrunt;
extern bool shGrunt;
extern bool amyGrunt;
extern bool superBounce;
extern bool shBounce;



extern AnimationInfo SonicAnimationListR[];


void Init_NewAnimation();
void Init_SonicNewTricks();
void Init_Helper();
void Init_ActionRemap();


void init_SpinDash();
bool isBlackShield();
void Init_Bounce();
void SpinDash_ButtonCheckOnFrames();