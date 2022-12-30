#include "stdafx.h"

HelperFunctions HelperFunctionsGlobal;
const char* error = "[Sonic New Tricks]: WARNING: Your version of the Mod Loader is old, the mod won't work properly.\nPlease update your Mod Loader for the best experience.";
const char* error2 = "WARNING: Action Remap is not compatible with Sonic New Tricks.This mod already allow you to change Sonic actions.";

extern "C" {
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		Sleep(10);

		HMODULE ActionRemap = GetModuleHandle(L"sa2-action-remap");

		if (ActionRemap)
		{
			PrintDebug(error2);
			MessageBoxA(MainWindowHandle, error2, "Sonic New Tricks Mod", MB_ICONWARNING);
		}

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
		Buttons_CheckOnFrames();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}