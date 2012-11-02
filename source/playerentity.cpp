#include "playerentity.h"
#include "audiomanager.h"
#include "inputmanager.h"

#define SFX_PLAYER_MOVE SFX_DOG_BARK

extern PlayerEntity* gpPlayerEntity = NULL;

PlayerEntity::PlayerEntity(SpriteData* gfx) : LivingEntity(gfx)
{
	gpPlayerEntity = this;
}

PlayerEntity::~PlayerEntity()
{
	gpPlayerEntity = NULL;
}


void PlayerEntity::Update()
{
	Vector3<s16> destPosition;
	if (gInputManager.moveToPosition(getPosition(), 2.0, destPosition))
	{
		setPosition(destPosition);

		audioManager.playSound(SFX_PLAYER_MOVE);
	}
}
