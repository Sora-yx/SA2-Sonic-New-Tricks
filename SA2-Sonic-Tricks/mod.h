#pragma once

void ReadConfig(const char* path);

extern HelperFunctions HelperFunctionsGlobal;

enum customButton {
	buttons_XB = 0x402,
};


extern Buttons LightDashButton;
extern Buttons BounceButton;
extern Buttons SpinDashButton;
extern Buttons SomersaultButton;
extern bool SpinDashSomersault;
extern Buttons pickButton;
extern Buttons grabButton;
extern Buttons petButton;
extern Buttons gravityButton;

void Init_JumpBall();