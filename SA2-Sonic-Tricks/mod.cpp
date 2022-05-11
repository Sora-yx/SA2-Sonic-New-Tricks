#include "stdafx.h"


HelperFunctions HelperFunctionsGlobal;
const char* error = "[Sonic New Tricks]: WARNING: Your version of the Mod Loader is old, the mod won't work properly.\nPlease update your Mod Loader for the best experience.";


extern "C" {

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		Sleep(10);

		HelperFunctionsGlobal = helperFunctions;

		if (HelperFunctionsGlobal.Version < 8) {
			PrintDebug(error);
			MessageBoxA(MainWindowHandle, error, "Sonic New Tricks Mod", MB_ICONWARNING);
		}

		ReadConfig(path); //get mod settings by the user

		Init_ObjectsHacks();
		Init_SonicNewTricks();
		Init_ActionRemap();
		Init_JumpBall();
	}

	__declspec(dllexport) void __cdecl OnFrame() {

		SpinDash_ButtonCheckOnFrames();
	}



	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}