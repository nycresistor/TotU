#include <stdio.h>
#include "tbuffer.h"

int main (int argc, char *argv[])
{
	int i,x,y;

	printf("Setup gpmc\n");
	setup_gpmc();

	printf("Setup tft\n");
	setup_tft();

	setRotation(3);
	setAddrWindow(0,0,319,239);
	activateBank(0);

	printf("Setup buffers\n");
	setup_buffers();

//	printf("Setup thread\n");
//	setup_paint_thread();

//	printf("Swap buffers\n");
//	swap_buffers();

	for (i=0; i<10; i++) {

		printf("Write pixels\n");
		for (x=0; x<320;  x++) {
			for (y=0; y<240; y++) {
				//write_pixel(0xFFFF, x, y, 0xF800);
				write_pixel(0xFFFF, x, y, 0xFFFF);
			}
		}

		printf("Swap buffers again\n");
		swap_buffers();
		paint(NULL);
		getchar();

		printf("Write pixels\n");
		for (x=0; x<320;  x++) {
			for (y=0; y<240; y++) {
				write_pixel(0xFFFF, x, y, 0x07E0);
			}
		}

		printf("Swap buffers again\n");
		swap_buffers();
		paint(NULL);
		getchar();

	}

	//finish();
	return 0;
}
