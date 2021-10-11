#include "stdafx.h"


bool CheckChara() {

	if (MainCharObj2[0]->CharID == Characters_Sonic && MainCharObj2[0]->CharID2 != Characters_Amy && sonicBall)
		return true;

	return false;
}


bool isSpeedCharacter() {
	if (MainCharObj2[0]->CharID == Characters_Sonic || MainCharObj2[0]->CharID == Characters_Shadow || MainCharObj2[0]->CharID2 == Characters_MetalSonic || MainCharObj2[0]->CharID2 == Characters_Amy)
		return true;

	return false;
}


bool isSonicAttacking() {

	if (!isSpeedCharacter())
		return false;

	EntityData1* data1 = MainCharObj1[0];

	if (data1->Action == Action_SpinRelease || data1->Action == Action_Jump || data1->Action == Action_SpinCharge || data1->Action == Action_HomingAttack
		|| data1->Action >= Action_Somersault1 && data1->Action <= Action_MovingSomersault1 || data1->Action == Action_BounceDown) {

		return true;
	}


	return false;
}


bool isAttackingBoxes() {

	if (!isSpeedCharacter())
		return false;

	EntityData1* data1 = MainCharObj1[0];

	if (data1->Action == Action_SpinRelease || data1->Action == Action_SpinCharge || data1->Action == Action_HomingAttack
		|| data1->Action >= Action_Somersault1 && data1->Action <= Action_MovingSomersault1 || data1->Action == Action_BounceDown) {

		return true;
	}

	return false;
}

bool isHedgePannel() {
	return GetModuleHandle(L"HedgePanel") != NULL;
}


bool isCharaSelect() {
	HMODULE charaMod = GetModuleHandle(L"SA2CharSel");
	HMODULE charaModPlus = GetModuleHandle(L"CharacterSelectPlus");

	if (charaMod || charaModPlus)
		return true;

	return false;
}

bool isSA2Miles() {
	return GetModuleHandle(L"SA2-Better-Miles") != NULL;
}

bool isBlackShield() {
	return GetModuleHandle(L"DisableBlackShield") != NULL;
}