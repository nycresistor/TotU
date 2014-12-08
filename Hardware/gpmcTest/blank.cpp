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

#define screenSize (width * height)
#define frameSize (screenSize * 16)

uint16_t * frame0;
uint16_t * frame1;
uint16_t * frame2;
uint16_t * frame3;
uint16_t * frame4;
uint16_t * frame5;
uint16_t * frame6;
uint16_t * frame7;
uint16_t * frame8;
uint16_t * frame9;
uint16_t * frame10;
uint16_t * frame11;
uint16_t * frame12;
uint16_t * frame13;
uint16_t * frame14;
uint16_t * frame15;
uint16_t * frame16;
uint16_t * frame17;
uint16_t * frame18;
uint16_t * frame19;
uint16_t * frame20;
uint16_t * frame21;






clock_t t1;

void generateScreen(uint16_t * screen, uint32_t size, uint16_t fillWord)
{
	for (int i = 0; i < size; i++)
	{
		screen[i] = fillWord;
	}
}

void generateCheckerboard(uint16_t * screen)
{
    for (int y = 0; y < width * 16; y++)
    { 
        for (int x = 0; x < height * 16; x += 2)
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

void generatePrimes(uint16_t * screen, uint32_t len)
{
	for (int i = 0; i < len; i++)
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

    printf("Screen size %d\n", screenSize);
    printf("Frame size %d\n", frameSize);

    frame0 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame2 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame3 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame4 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame5 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame6 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame7 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame8 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame9 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame10 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame11 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame12 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame13 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame14 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame15 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame16 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame17 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame18 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame19 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame20 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
    frame21 = (uint16_t *) calloc(frameSize, sizeof(uint16_t));


    FILE *ptr_myfile;


ptr_myfile = fopen("robin/robin1.bin", "rb");
fread(frame0, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin2.bin", "rb");
fread(frame1, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin3.bin", "rb");
fread(frame2, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin4.bin", "rb");
fread(frame3, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin5.bin", "rb");
fread(frame4, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin6.bin", "rb");
fread(frame5, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin7.bin", "rb");
fread(frame6, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin8.bin", "rb");
fread(frame7, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin9.bin", "rb");
fread(frame8, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin10.bin", "rb");
fread(frame9, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin11.bin", "rb");
fread(frame10, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin12.bin", "rb");
fread(frame11, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin13.bin", "rb");
fread(frame12, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin14.bin", "rb");
fread(frame13, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin15.bin", "rb");
fread(frame14, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin16.bin", "rb");
fread(frame15, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin17.bin", "rb");
fread(frame16, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin18.bin", "rb");
fread(frame17, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin19.bin", "rb");
fread(frame18, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin20.bin", "rb");
fread(frame19, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin21.bin", "rb");
fread(frame20, sizeof(uint16_t), frameSize, ptr_myfile);
ptr_myfile = fopen("robin/robin22.bin", "rb");
fread(frame21, sizeof(uint16_t), frameSize, ptr_myfile);


    //generateCheckerboard(frame0);
    //generateCheckerboard(frame1);

    setRotation(3);
    float lastTime = 0;
    printf("COMMENCE\n");
    setAddrWindow(0, 0, 319, 239);
    while(1) {
	float startTime = (float)clock() / CLOCKS_PER_SEC;
	writeFramePregenerated(frame0, frameSize);
//	writeFramePregenerated(frame1, frameSize);
	writeFramePregenerated(frame2, frameSize);
	writeFramePregenerated(frame3, frameSize);
	writeFramePregenerated(frame4, frameSize);
	writeFramePregenerated(frame5, frameSize);
	writeFramePregenerated(frame6, frameSize);
	writeFramePregenerated(frame7, frameSize);
	writeFramePregenerated(frame8, frameSize);
	writeFramePregenerated(frame9, frameSize);
	writeFramePregenerated(frame10, frameSize);
	writeFramePregenerated(frame11, frameSize);
	writeFramePregenerated(frame12, frameSize);
	writeFramePregenerated(frame13, frameSize);
	writeFramePregenerated(frame14, frameSize);
	writeFramePregenerated(frame15, frameSize);
	writeFramePregenerated(frame16, frameSize);
	writeFramePregenerated(frame17, frameSize);
	writeFramePregenerated(frame18, frameSize);
	writeFramePregenerated(frame19, frameSize);
	writeFramePregenerated(frame20, frameSize);
	writeFramePregenerated(frame21, frameSize);

	float fps = (1 / (((float) clock() / CLOCKS_PER_SEC) - startTime)) / 2;

	printf("Frame time: %3.5f\n", fps);
   	
 }

    close_gpmc();

    return 0;
}
