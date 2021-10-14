#include "stdafx.h"

//unused, but maybe one day.

NJS_VECTOR GetPathPosition(NJS_VECTOR* orig, NJS_VECTOR* dest, float state) {
	NJS_VECTOR result;
	result.x = (dest->x - orig->x) * state + orig->x;
	result.y = (dest->y - orig->y) * state + orig->y;
	result.z = (dest->z - orig->z) * state + orig->z;

	return result;
}


void RailPath_SwapRail(CharObj2Base* co2, EntityData1* PlayerEntity, LoopHead* loophead) {

	if (!co2)
		return;

	if (PlayerEntity->Action != Action_Grind || !GetAnalog(PlayerEntity, co2, 0, 0))
		return;

	if (Jump_Pressed[co2->PlayerNum]) {
		NJS_VECTOR dir = { 0, 0, 21 };

		short anim = co2->AnimInfo.Current;
		bool isPlayerLookingAtLeft;

		isPlayerLookingAtLeft = (anim == 104 || anim == 106 || anim == 108 || anim == 110 || anim == 112);

		if (isPlayerLookingAtLeft)
			dir.z = -21;

		//we extend the next point pos for precision
		NJS_VECTOR CheckPos;
		njPushMatrix(_nj_unit_matrix_);
		njTranslateV(0, &PlayerEntity->Position);
		njRotateY(0, -PlayerEntity->Rotation.y);
		njCalcPoint_(&dir, &CheckPos, 0);
		njPopMatrix(1u);

		for (int16_t point = 0; point < loophead->Count - 1; point += 1) {
			LoopPoint* loopPoint = &loophead->Points[point];
			LoopPoint* NextPoint = &loophead->Points[point + 1];

			for (float state = 0; state <= 1; state += 0.01f) {

				NJS_VECTOR center = GetPathPosition(&loopPoint->Position, &NextPoint->Position, state);

				if (!IsPointInsideSphere(&center, &CheckPos, 20))
					continue;
				else
					PlayerEntity->Position = center;

				return;
			}
		}
	}
}

