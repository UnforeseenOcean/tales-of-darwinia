#include "trashcanentity.h"
#include "playerentity.h"

#include <stdio.h>
#include "math.h"

void TrashCanEntity::Update()
{
	transformContext = 0;

	u8 derp = (u8)(updateCount % 30);
	float32 periodic = 0.5*sin(3.14159*(float32)derp/30.0);
	float32 derp2 = periodic + 1.0;

	oamRotateScale(&oamSub,
		0,
		degreesToAngle(periodic*10-30),
		floatToFixed(derp2, 8),
		floatToFixed(derp2, 8)	
	);

	if (shouldBeRemoved)
		return;

	if (IsTouchedByNearbyPlayer())
	{
		LivingEntity::Damage(gpPlayerEntity->mStats->attack);
		printf("Trashcan Ouch %d!\n", mStats->health);
	}

	Sprite::Update();
}
