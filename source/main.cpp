/* vim: ts=4 sw=4 noexpandtab
 * Tales of Darwinia
 *
 * A Game by Yelp Engineers
 *
 * Henri B
 * Paul B
 * Patrick T
 * Kevin L
 * Jeryl C
 * Matt G
 * Tom R
 * MAYBE YOU!
 *
 */

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "darwin.h"
#include "man.h"
#include "logo.h"
#include "sprite.h"
#include "entitymanager.h"
#include "event.h"
#include "eventdispatcher.h"
#include "inputmanager.h"
#include "playerentity.h"
#include "enemyentity.h"

#include "dispatch.h"

/* Maps */
#include "maps.h"
#include "map_data.h"

#include "audiomanager.h"

#include "sprite.h"
#include "spritedata.h"

void Vblank(void) {
	
}

void init(void) {
	/* Force the main engine to top screen */
	lcdMainOnTop();

	/* Set the mode for 2 text layers and two extended background layers */
	videoSetMode(MODE_5_2D | DISPLAY_BG2_ACTIVE);
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_BG);
		
	/* Set the mode for sprite display */
	videoSetModeSub(MODE_5_2D | DISPLAY_BG0_ACTIVE | DISPLAY_SPR_ACTIVE);
	vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	/* Set the default backgorund color */
	setBackdropColor(0xF);

	oamInit(&oamMain, SpriteMapping_1D_128, false);
	oamInit(&oamSub, SpriteMapping_1D_128, false);

	/* Set the vertical blank event */
	irqSet(IRQ_VBLANK, Vblank);
}

int main(void) {
	/* Put any generic engine/game init code in the init () */
	init();

	/* Load data from map_data.h into the map engine */
	MapEngine mapEngine = MapEngine(
		&tile_palette,
		&tile_set,
		&map
	);

	/* Tile engine is going to claim BG0 */
	bgInitSub(0, BgType_Text8bpp, BgSize_T_512x512, 0, 1);
//	REG_BG0CNT = BG_64x64 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	/* Load the relevant data into the VRAM */
	mapEngine.dumpPaletteToVRAM(&BG_PALETTE_SUB[0]);
	mapEngine.dumpTilesToVRAM((u8*)BG_TILE_RAM_SUB(1));
	mapEngine.dumpMapToVRAM((u16*)BG_MAP_RAM_SUB(0));

	/* Vertical offset for scrolling the map in pixels */
	REG_BG0VOFS = 64;

	/* Decompress and show the logo */
	bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	decompress(logoBitmap, BG_GFX, LZ77Vram);

	/* Set up the console */
	PrintConsole topScreen;
	bgInit(1, BgType_Text4bpp, BgSize_T_256x256, 0, 0);
	consoleInit(&topScreen, 1, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);

	SpriteData* gfx = new SpriteData(SpriteSize_32x32, SpriteColorFormat_256Color, (u8*)darwinTiles, 3);
	SpriteData* gfx2 = new SpriteData(SpriteSize_32x32, SpriteColorFormat_256Color, (u8*)manTiles, 3);

	dmaCopy(darwinPal, SPRITE_PALETTE_SUB, 512);

	gEntityManager.Init();

	PlayerEntity* playerEntity = new PlayerEntity(gfx);
	playerEntity->Init();
	playerEntity->setPosition(Vector3<s16>(64,90,1));

	EnemyEntity* enemyEntity = new EnemyEntity(gfx2);
	enemyEntity->Init();
	enemyEntity->setPosition(Vector3<s16>(192,90,0));

	audioManager.initialize();
	audioManager.playMusic(MOD_TECHNO_MOZART);

	//consoleDemoInit();

	while(1) {
		gEntityManager.Update();
		gEntityManager.Render();

		oamUpdate(&oamSub);
		bgUpdate();

		swiWaitForVBlank();
	}

	return 0;
}
