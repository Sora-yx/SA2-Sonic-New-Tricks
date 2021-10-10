#include "stdafx.h"


HelperFunctions HelperFunctionsGlobal;


extern "C" {

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		HelperFunctionsGlobal = helperFunctions;

		ReadConfig(path); //get mod settings by the user


		Init_Helper();
		Init_SonicNewTricks();
		Init_ActionRemap();
	}


	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}