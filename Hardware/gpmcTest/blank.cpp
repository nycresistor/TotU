#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <memory.h>
#include <iostream>

#include "blank.h"
#include "tft.h"
#include "gpmc.h"


#define width 320
#define height 240

uint16_t * screen0;
uint16_t * screen1;

void generateScreen(uint16_t * screen, uint16_t fillWord)
{
	for (int i = 0; i < width * height; i++)
	{
		screen[i] = fillWord;
	}
}

void generateCheckerboard(uint16_t * screen)
{
    for (int y = 0; y < width; y++)
    { 
        for (int x = 0; x < height; x += 2)
        {
                if (y % 2 == 0) {
                        screen[(y * height) + x] = 0xffff;
                        screen[(y * height) + x + 1] = 0x0000;
                } else {
                        screen[(y * height) + x] = 0x0000;
                        screen[(y * height) + x + 1] = 0xffff;
                }
        }
    }
}

int main (int argc, char *argv[])
{

    setup_gpmc();
	
    setup_tft();

    screen0 = (uint16_t *) calloc(width * height, sizeof(uint16_t));
    screen1 = (uint16_t *) calloc(width * height, sizeof(uint16_t));
 
    generateScreen(screen0, 0x0000);
    generateScreen(screen1, 0xFFFF);
 
    printf("Sending black\n");

    setAddrWindow(0, 0, height - 1, width - 1);
    
    while(1) {
	writeFrame(screen0);
    	writeFrame(screen1);
    }
/*  
    for (int i = 0; i < width * height; i++) {
	writeData8(0x00);
	writeData8(0x00); 
   }
*/


    close_gpmc();

    return 0;
}
