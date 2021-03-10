#include "stdafx.h"

extern "C" {


	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{

		Init_StartEndPos();
		Init_BetterSonic();

		/*StoryEntry* story = (StoryEntry*)0x173A5E0;
		story->Type = 1;
		story->Level = LevelIDs_MetalHarbor;
		story->Character = Characters_Amy;*/

	}




	__declspec(dllexport) void __cdecl OnFrame() {

		if (Controllers[0].press & Buttons_Y)
		{

		}
	}





	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}