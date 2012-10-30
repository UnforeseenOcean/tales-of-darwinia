/* vim: ts=4 sw=4 noexpandtab
 * Tales of Darwinia
 *
 * A Game by Yelp Engineers
 *
 * Henri Bai
 * Paul Berens
 * Patrick Tiet
 * Kevin Lange
 *
 */

#include <nds.h>
#include <stdio.h>

volatile int frame = 0;

void Vblank() {
	frame++;
}

int main(void) {

	touchPosition touchXY;

	irqSet(IRQ_VBLANK, Vblank);

	consoleDemoInit();

	iprintf("Tales of Darwinia\n");
 
	while(1) {
	
		swiWaitForVBlank();
		touchRead(&touchXY);

		iprintf("\x1b[10;0HFrame = %d", frame);
		iprintf("\x1b[16;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
		iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);
	
	}

	return 0;
}
