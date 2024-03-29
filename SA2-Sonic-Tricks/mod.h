#pragma once

#define TaskHook static FunctionHook<void, ObjectMaster*>

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
extern Buttons putButton;
extern Buttons BlackShieldButton;
void Init_JumpBall();

const uint8_t playerMAX = 2;