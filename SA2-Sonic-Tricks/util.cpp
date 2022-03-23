#include "stdafx.h"

bool isBallForm() {

	if (SonicCO2PtrExtern->base.CharID2 != Characters_Sonic)
		return false;

	if (SonicCO2PtrExtern->base.AnimInfo.Current == 30 || SonicCO2PtrExtern->base.AnimInfo.Current == 100 || SonicCO2PtrExtern->base.AnimInfo.Current >= 65 && SonicCO2PtrExtern->base.AnimInfo.Current <= 67)
	{
		return true;
	}

	return false;
}

bool CheckChara() {

	if (SonicCO2PtrExtern->base.CharID == Characters_Sonic && SonicCO2PtrExtern->base.CharID2 != Characters_Amy && sonicBall)
		return true;

	return false;
}

bool isSpeedCharacter() {

	if (SonicCO2PtrExtern->base.CharID2 <= Characters_Shadow || SonicCO2PtrExtern->base.CharID2 == Characters_MetalSonic || SonicCO2PtrExtern->base.CharID2 == Characters_Amy)
		return true;

	return false;
}

bool isSonicAttacking() {

	if (!isSpeedCharacter())
		return false;

	EntityData1* data1 = MainCharObj1[SonicCO2PtrExtern->base.PlayerNum];

	if (data1->Action == Action_SpinRelease || data1->Action == Action_Jump || data1->Action == Action_SpinCharge || data1->Action == Action_HomingAttack
		|| data1->Action >= Action_Somersault1 && data1->Action <= Action_MovingSomersault1 || data1->Action == Action_BounceDown) {

		return true;
	}


	return false;
}

bool isAttackingBoxes() {

	if (!isSpeedCharacter())
		return false;

	EntityData1* data1 = MainCharObj1[SonicCO2PtrExtern->base.PlayerNum];

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


bool isSS() {
	return GetModuleHandle(L"SA2-Super-Sonic") != NULL;
}

bool isAmySpinDash() {
	HMODULE amy = GetModuleHandle(L"amyspindash");
	HMODULE amy2 = GetModuleHandle(L"SA2-Amy-Metal-SpinDash");

	if (amy || amy2)
		return true;

	return false;
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
