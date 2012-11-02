#include <stdio.h>
#include "playerentity.h"
#include "audiomanager.h"
#include "inputmanager.h"
#include "maps.h"
#include "math.h"

#define SFX_PLAYER_MOVE SFX_DOG_BARK

PlayerEntity* gpPlayerEntity = NULL;

extern int gMapTileIndex;
extern MapEngine* gpMapEngine;


void PlayerEntity::Init(){
	LivingEntity::Init();

	// TODO: Make this more robust
	mStats->attack = 1;
}

PlayerEntity::PlayerEntity(SpriteData* gfx) : LivingEntity(gfx)
{
	gpPlayerEntity = this;
	maxSpeed = 3;
	acceleration = 0.2;
}

PlayerEntity::~PlayerEntity()
{
	gpPlayerEntity = NULL;
}


void PlayerEntity::Update()
{
	if (keysHeld() & KEY_TOUCH) {
		touchPosition curTouchPosition;
		touchRead(&curTouchPosition);
		setTargetPosition(Vector3<s16>(curTouchPosition.px, curTouchPosition.py, 1));
		movementState = MOVING;
	}

	if (movementState == MOVING) {
		speed += acceleration;
	}

	if (movementState == STOPPED) {
		speed = 0;
	}
	if (speed < 0) {
		speed = 0;
	}

	if (speed > maxSpeed) {
		speed = maxSpeed;
	}

	LivingEntity::Update();

	// HUGE HACK
	static int mapX = 0;
	static int mapY = 0;

	const int slop = 32;
	const int width = 256;
	const int height = 190;

	if (position.x() - mapX < slop) {
		if (mapX > 0) mapX--;
	}
	if (mapX + width - position.x() < slop) {
		if (mapX < ((MAP_WIDTH-(256/8))*8)) mapX++;
	}
	if (position.y() - mapY < slop) {
		if (mapY > 0) mapY--;
	}
	if (mapY + height - position.y() < slop) {
		if (mapY < ((MAP_HEIGHT-(192/8))*8)) mapY++;
	}

	gpMapEngine->scrollMapAbsolute(gMapTileIndex, mapX, mapY);
}

void PlayerEntity::Collect(ItemEntity* item)
{
	// in the future we might add this item to our inventory (e.g. weapon, tradable item).
	// for now items are consumables that just change our stats

	LivingStats* statsDelta = item->Consume();
	this->mStats->attack += statsDelta->attack;
	this->mStats->maxHealth += statsDelta->maxHealth;
	this->mStats->health += statsDelta->health;
	this->mStats->speed += statsDelta->speed;
	if (this->mStats->health > this->mStats->maxHealth)
	{
		this->mStats->health = this->mStats->maxHealth;
	}

	mStats->Print("DARWIN");
}
