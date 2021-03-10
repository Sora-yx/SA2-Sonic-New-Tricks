#include "stdafx.h"


Trampoline* Sonic_Main_t;


void RestorePhysic(CharObj2Base* co2) {
	co2->PhysData.CollisionSize = PhysicsArray[0].CollisionSize;
	co2->PhysData.RippleSize = PhysicsArray[0].RippleSize;
	co2->PhysData.FloorGrip = PhysicsArray[0].FloorGrip;
	co2->PhysData.YOff = PhysicsArray[0].YOff;
	return;
}

//Apply Somersault physics/collision 
void SetSomersaultCol(CharObj2Base* co2, EntityData1* v1) {
	co2->PhysData.CollisionSize = PhysicsArray[0].CollisionSize * 0.4000000059604645;
	co2->PhysData.RippleSize = PhysicsArray[0].RippleSize * 0.4000000059604645;
	co2->PhysData.FloorGrip = PhysicsArray[0].FloorGrip * 0.4000000059604645;
	co2->PhysData.YOff = PhysicsArray[0].YOff * 0.4000000059604645;
	v1->Collision->CollisionArray->push &= ~0x4000u;
	return;
}



void Sonic_Main_r(ObjectMaster* obj)
{
	ObjectFunc(origin, Sonic_Main_t->Target());
	origin(obj);

	CharObj2Base* co2 = MainCharObj2[0];
	EntityData1* data1 = MainCharObj1[0];

	if (data1->Action == Action_SpinRelease)
	{

	}


}