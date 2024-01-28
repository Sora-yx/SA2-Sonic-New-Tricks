#include "stdafx.h"

HelperFunctions HelperFunctionsGlobal;
const char* error = "[Sonic New Tricks]: WARNING: Your version of the Mod Loader is old, the mod won't work properly.\nPlease update your Mod Loader for the best experience.";
const char* error2 = "WARNING: Action Remap is not compatible with Sonic New Tricks.\nThis mod already allow you to change Sonic actions.";
const char* deprecated = "WARNING Sonic New Tricks is deprecated! Please download and install 'SA2 New Tricks Mod' for the best experience.\n\nIf you already have it installed, please uninstall Sonic New Tricks.";
extern "C" 
{

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		PrintDebug(deprecated);
		MessageBoxA(MainWindowHandle, deprecated, "Sonic New Tricks", MB_ICONWARNING);
		return;
	}

	__declspec(dllexport) void __cdecl OnFrame() 
	{

	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}