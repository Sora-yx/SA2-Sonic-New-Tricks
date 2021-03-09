#include "stdafx.h"

extern "C" {


	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{



	}




	__declspec(dllexport) void __cdecl OnFrame() {

		if (Controllers[0].press & Buttons_Y)
		{

		}


	}





	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}