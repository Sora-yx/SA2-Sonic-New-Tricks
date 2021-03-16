#include "stdafx.h"

bool sa1dash = true;
bool sa1speed = true;
bool sa1SFX = true;
bool sonicBall = true;
bool shGrunt = true;
bool sonicGrunt = true;
bool amyGrunt = false;
bool superBounce = true;
bool shBounce = true;
HelperFunctions HelperFunctionsGlobal;


extern "C" {


	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		sa1dash = config->getBool("General", "sa1dash", true);
		sa1speed = config->getBool("General", "sa1speed", true);
		sa1SFX = config->getBool("General", "sa1SFX", true);
		superBounce = config->getBool("General", "superBounce", true);

		sonicBall = config->getBool("Sonic", "sonicBall", true);
		sonicGrunt = config->getBool("Sonic", "sonicGrunt", true);

		shBounce = config->getBool("Shadow", "shBounce", true);
		shGrunt = config->getBool("Shadow", "shGrunt", false);
		
		amyGrunt = config->getBool("Amy", "amyGrunt", false);
		delete config;

		HelperFunctionsGlobal = helperFunctions;

		if (sa1SFX)
		{
			HelperFunctionsGlobal.ReplaceFile("Resource\\gd_PC\\MLT\\se_ch_ss\\2.csb", "Resource\\gd_PC\\Sounds\\se_ch_ss\\2.csb");
			HelperFunctionsGlobal.ReplaceFile("Resource\\gd_PC\\MLT\\se_ch_sn_BATTLE\\2.csb", "Resource\\gd_PC\\Sounds\\se_ch_sn_BATTLE\\2.csb");
			HelperFunctionsGlobal.ReplaceFile("Resource\\gd_PC\\MLT\\chao_chara_ss\\2.csb", "Resource\\gd_PC\\Sounds\\chao_chara_ss\\2.csb");
		}

		Init_Helper();
		Init_BetterSonic();
	}


	__declspec(dllexport) void __cdecl OnFrame() {

		if (Controllers[0].press & Buttons_Y)
		{

			//PlaySound3(0, 8206);
			//PlaySound3(v38, 8209);
		}
	}





	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}