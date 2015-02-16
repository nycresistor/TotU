#include <stdio.h>
#include "tbuffer.h"

int main (int argc, char *argv[])
{

	printf("Setup gpmc\n");
	setup_gpmc();

	printf("Setup tft\n");
	setup_tft();

	setRotation(3);
	setAddrWindow(0,0,319,239);

	printf("Setup buffers\n");
	setup_buffers();

	printf("Swap buffers\n");
	swap_buffers();

	printf("Write pixels\n");
	for (int x=0; x<320;  x++) {
		for (int y=0; y<240; y++) {
			write_pixel(0xFFFF, x, y, 0xF800);
		}
	}

	printf("Swap buffers again\n");
	swap_buffers();

	activateBank(0);
	printf("Display buffer\n");
	display_buffer();

	return 0;
}
