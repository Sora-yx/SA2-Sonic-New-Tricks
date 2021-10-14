#include "stdafx.h"


bool isBallForm() {

	if (MainCharObj2[0]->CharID2 != Characters_Sonic)
		return false;

	if (MainCharObj2[0]->AnimInfo.Current == 30 || MainCharObj2[0]->AnimInfo.Current == 100 || MainCharObj2[0]->AnimInfo.Current >= 65 && MainCharObj2[0]->AnimInfo.Current <= 67)
	{
		return true;
	}

	return false;
}


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

float GetSquare(NJS_VECTOR* orig, NJS_VECTOR* dest) {
	return powf(dest->x - orig->x, 2) + powf(dest->y - orig->y, 2) + powf(dest->z - orig->z, 2);
}

float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest) {
	return sqrtf(GetSquare(orig, dest));
}

bool IsPointInsideSphere(NJS_VECTOR* center, NJS_VECTOR* pos, float radius) {
	return GetDistance(center, pos) <= radius;
}
