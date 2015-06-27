// TODO Doesn't account for the 2 banks
// TODO Write buffer from a separate thread
// TODO Add dirty flags

#include "tbuffer.h"
#include "stdio.h"

pthread_t thread;
pthread_mutex_t mutex;
pthread_mutex_t mutex2;
uint16_t * buffers[3];
int bufindex;
uint16_t * display;
uint16_t * draw1;
uint16_t * draw2;
int done;

// Sets up memory for the triple buffers
void setup_buffers()
{
	for (int i=0; i<3; i++) {
		buffers[i] = (uint16_t *) calloc(WIDTH * HEIGHT * SCREENS, sizeof(uint16_t));
	}

	display = buffers[0];
	draw1 = buffers[1];
	draw2 = buffers[2];
}

void setup_paint_thread()
{
	done = 0;
	int result = pthread_create(&thread, NULL, paint, NULL);

	if (result) {
		fprintf(stderr, "phtread create %d", result);
		exit(1);
	}
	
	//pthread_mutex_lock(&mutex);
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
	//pthread_mutex_lock(&mutex2);
	bufindex = bounded_add(bufindex, 1, 2);
	
	display = buffers[bufindex];
	draw1 = buffers[bounded_add(bufindex, 1, 2)];
	draw2 = buffers[bounded_add(bufindex, 2, 2)];

	memcpy(draw1, display, WIDTH * HEIGHT * SCREENS * sizeof(uint16_t));	

	printf("unlocking mutex\n");
	//pthread_mutex_unlock(&mutex2);
	//pthread_mutex_unlock(&mutex);
}

// Writes a pixel to up to 16 screens.  Screens is a bitmask.
// Color is a 16-bit color: rrrrrggggggbbbbb (5,6,5)
// TODO Add bank
void write_pixel(uint16_t screens, int x, int y, uint16_t color)
{

	int idx = (y * WIDTH + x) * 16;
	uint16_t *d1 = draw1 + idx;
	uint16_t *d2 = draw2 + idx;
	uint16_t mask;

	for (int i=0; i<16; i++) {
		mask = color & (1<<i) ? 
			*d1 ^ screens & screens : 
			*d1 & screens;

		*d2 = *d1 ^= mask;
		d1++;
		d2++;
	}
}

// Sends the display buffer to the screens
// TODO Add bank
void *paint(void *param)
{
//	while (1) {
		printf("Getting lock\n");
		//pthread_mutex_lock(&mutex);

		if (done) 
			return NULL;

		//pthread_mutex_lock(&mutex2);
		printf("Calling writeFramePregenerated\n");
		writeFramePregenerated(display, WIDTH * HEIGHT * 16);
		printf("Paint done\n");
		//pthread_mutex_unlock(&mutex2);
//	}
}

void finish() 
{
	printf("Finish\n");
	done = 1;
	pthread_mutex_unlock(&mutex);
	printf("Joining\n");
	pthread_join(thread, NULL);
}

