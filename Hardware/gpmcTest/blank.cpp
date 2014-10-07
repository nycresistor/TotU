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

int isPrime(uint16_t num)
{
	for (int i = 2; i * i <= num; i++)
	{
		if (num % i == 0) return 0;
	}
	return 1;
}

void generatePrimes(uint16_t * screen)
{
	for (int i = 0; i < width * height; i++)
	{
		if (isPrime(i)) screen[i] = 0;
		else screen[i] = 0xFFFF;
	}
}

void generateDiagonals(uint16_t * screen)
{	

	int counter = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (x == counter) screen[(y * width) + x] = 0xFFFF;
			else screen[(y * width) + x] = 0;
		}
		counter++;
		if (counter > width) counter = 0;

	}
}

int main (int argc, char *argv[])
{

    setup_gpmc();
	
    setup_tft();

    screen0 = (uint16_t *) calloc(width * height, sizeof(uint16_t));
    screen1 = (uint16_t *) calloc(width * height, sizeof(uint16_t));
 

/*
    uint16_t * output0 = (uint16_t *) malloc(320 * 240 * 16 * sizeof(uint16_t));
    uint16_t * output1 = (uint16_t *) malloc(320 * 240 * 16 * sizeof(uint16_t));
   
    for (int i = 0; i < 320 * 240 * 16; i++)
	{
		output1[i] = 0xFFFF;
	}
*/ 
    generateScreen(screen0, 0x0000);
    generateScreen(screen1, 0xFFFF);


    setRotation(3);
    setAddrWindow(0, 0, width - 1, height - 1);

    printf("COMMENCE\n");
  //  setDC(DATA);
    while(1) {
//	gpmcWritePregenerated(output0, 76800);
 //   	gpmcWritePregenerated(output1, 76800);
   
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
