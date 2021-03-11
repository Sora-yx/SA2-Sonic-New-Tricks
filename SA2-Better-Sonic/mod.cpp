#include "stdafx.h"

bool sa1dash = true;
bool sa1jump = true;
bool sonicBall = true;
bool shBall = true;
bool amyBall = true;
bool sonicGrunt = true;
bool shGrunt = true;
bool amyGrunt = false;
bool superBounce = true;
bool shBounce = true;

extern "C" {


	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		sa1dash = config->getBool("General", "sa1dash", true);
		sa1jump = config->getBool("General", "sa1jump", true);
		superBounce = config->getBool("General", "superBounce", true);

		sonicBall = config->getBool("Sonic", "sonicBall", true);
		sonicGrunt = config->getBool("Sonic", "sonicGrunt", true);

		shBall = config->getBool("Shadow", "shBall", true);
		shBounce = config->getBool("Shadow", "shBounce", true);
		shGrunt = config->getBool("Shadow", "shGrunt", false);
		
		amyBall = config->getBool("Amy", "amyBall", true);
		amyGrunt = config->getBool("Amy", "amyGrunt", false);
		delete config;

		Init_StartEndPos();
		Init_Helper();
		Init_BetterSonic();

		StoryEntry* story = (StoryEntry*)0x173A5E0;
		story->Type = 1;
		story->Level = LevelIDs_RadicalHighway;
		story->Character = Characters_Shadow;

	}




	__declspec(dllexport) void __cdecl OnFrame() {

		if (Controllers[0].press & Buttons_Y)
		{

		}
	}





	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}