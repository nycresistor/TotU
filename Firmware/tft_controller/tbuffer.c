// TODO Doesn't account for the 2 banks
// TODO Write buffer from a separate thread
// TODO Add dirty flags

#include "tbuffer.h"
#include "stdio.h"

uint16_t * buffers[3];
int bufindex;
uint16_t * display;
uint16_t * draw1;
uint16_t * draw2;

// Sets up memory for the triple buffers
void setup_buffers()
{
	for (int i=0; i<3; i++) {
		buffers[i] = (uint16_t *) calloc(WIDTH * HEIGHT * SCREENS, sizeof(uint16_t));
	}
}

// Convenience method for adding to a number with rollover
int bounded_add(int n, int x, int m)
{
	n += x;

	while (n>m)
		n -= m;

	return n;
}

// Moves the display to the next buffer and preps the other two for writes
void swap_buffers() 
{
	bufindex = bounded_add(bufindex, 1, 2);
	
	display = buffers[bufindex];
	draw1 = buffers[bounded_add(bufindex, 1, 2)];
	draw2 = buffers[bounded_add(bufindex, 2, 2)];

	memcpy(draw1, display, WIDTH * HEIGHT * SCREENS * sizeof(uint16_t));	
}

// Writes a pixel to up to 16 screens.  Screens is a bitmask.
// Color is a 16-bit color: rrrrrggggggbbbbb (5,6,5)
// TODO Add bank
void write_pixel(uint16_t screens, int x, int y, uint16_t color)
{
	int idx = (y * WIDTH + x) * 16;
	uint16_t *d1 = draw1 + idx;
	uint16_t *d2 = draw2 + idx;
	uint16_t bit;

	for (int i=0; i<16; i++) {
		bit = color & (1<<i);
		*d1 = *d1 ^ bit;
		*d2 = *d2 ^ bit;
	}
}

// Sends the display buffer to the screens
// TODO Add bank
void display_buffer()
{
	printf("Calling writeFramePregenerated\n");
	writeFramePregenerated(display, WIDTH * HEIGHT * 16);
}
