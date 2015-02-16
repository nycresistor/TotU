#ifndef _tbuffer_h_
#define _tbuffer_h_

#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "tft.h"

#define SCREENS 32

// Sets up memory for the triple buffers
void setup_buffers();

// Convenience method for adding to a number with rollover
int bounded_add(int n, int x, int m);

// Moves the display to the next buffer and preps the other two for writes
void swap_buffers();

// Writes a pixel to 1-16 screens
void write_pixel(uint16_t screens, int x, int y, uint16_t color);

// Sends the display buffer to the screens
void display_buffer();

#endif
