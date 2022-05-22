#include "stdafx.h"

Buttons	LightDashButton = Buttons_Y;
Buttons BounceButton = Buttons_X;
Buttons SpinDashButton = Buttons_X;
Buttons SomersaultButton = Buttons_B;
Buttons pickButton = Buttons_Y;
Buttons grabButton = (Buttons)buttons_XB;
Buttons petButton = (Buttons)buttons_XB;
Buttons gravityButton = Buttons_Y;

bool sa1dash = true;
bool sa1speed = true;
bool sa1SFX = true;
bool sonicBall = true;
bool shGrunt = true;
bool sonicGrunt = true;
bool amyGrunt = false;
bool superBounce = true;
bool shBounce = true;
bool SpinDashSomersault = false;

static const Buttons ButtonsList[]
{
	Buttons_B,
	Buttons_X,
	Buttons_Y,
	(Buttons)buttons_XB,
};

void ReadConfig(const char* path) {

	const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
	sa1dash = config->getBool("General", "sa1dash", true);
	sa1speed = config->getBool("General", "sa1speed", true);
	sa1SFX = config->getBool("General", "sa1SFX", true);
	superBounce = config->getBool("General", "superBounce", true);
	SpinDashSomersault = config->getBool("General", "SpinDashSomersault", false);

	sonicBall = config->getBool("Sonic", "sonicBall", true);
	sonicGrunt = config->getBool("Sonic", "sonicGrunt", true);

	shBounce = config->getBool("Shadow", "shBounce", true);
	shGrunt = config->getBool("Shadow", "shGrunt", true);

	amyGrunt = config->getBool("Amy", "amyGrunt", false);

	LightDashButton = ButtonsList[config->getInt("ActionRemap", "LightDashButton", 2)];
	BounceButton = ButtonsList[config->getInt("ActionRemap", "BounceButton", 1)];
	SpinDashButton = ButtonsList[config->getInt("ActionRemap", "SpinDashButton", 1)];
	SomersaultButton = ButtonsList[config->getInt("ActionRemap", "SomersaultButton", 0)];	
	pickButton = ButtonsList[config->getInt("ActionRemap", "pickButton", 2)];
	grabButton = ButtonsList[config->getInt("ActionRemap", "grabButton", 3)];
	petButton = ButtonsList[config->getInt("ActionRemap", "petButton", 3)];
	gravityButton = ButtonsList[config->getInt("ActionRemap", "gravityButton", 2)];

	delete config;

	if (sa1SFX)
	{
		HelperFunctionsGlobal.ReplaceFile("Resource\\gd_PC\\MLT\\se_ch_ss\\2.csb", "Resource\\gd_PC\\Sounds\\se_ch_ss\\2.csb");
		HelperFunctionsGlobal.ReplaceFile("Resource\\gd_PC\\MLT\\se_ch_sn_BATTLE\\2.csb", "Resource\\gd_PC\\Sounds\\se_ch_sn_BATTLE\\2.csb");
		HelperFunctionsGlobal.ReplaceFile("Resource\\gd_PC\\MLT\\chao_chara_ss\\2.csb", "Resource\\gd_PC\\Sounds\\chao_chara_ss\\2.csb");
	}
}