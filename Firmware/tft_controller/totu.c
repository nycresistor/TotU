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
#include <dirent.h>

#include "totu.h"
#include "tft.h"
#include "gpmc.h"

#define width 320
#define height 240

#define screenSize (width * height)
#define frameSize (screenSize * 16)

uint16_t * frames[100];
const char * seqfolder = "./seq";
char sequence_filenames[256][256];

int get_image_sequence(const char * folder, char filenames[256][256])
{
    DIR *d;
    struct dirent *dir;
    int numframes = 0;
    d = opendir(folder);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char * ext = strrchr(dir->d_name, '.');
            if (strcmp(ext, ".bin") == 0) {
                strcpy(filenames[numframes++], dir->d_name);
            }
        }
        closedir(d);
    } else {
        printf("Folder does not exist.\n");
        exit(-1);
    }
    return(numframes);
}
	
int main (int argc, char *argv[])
{

    setup_gpmc();
    setup_tft();
    setRotation(3);
    
    int numframes = get_image_sequence(seqfolder, sequence_filenames);

    if (!numframes > 0) {
        printf("No frames found.\n");
        exit(-1);
    }

    FILE * imagefile;
    for (int i = 0; i < numframes; i++)
    {
    	frames[i] = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
	    imagefile = fopen("seq/seq-1.bin", "rb");
    	fread(frames[i], sizeof(uint16_t), frameSize, imagefile);
    }

    float lastTime = 0;
    printf("COMMENCE\n");
    setAddrWindow(0, 0, 319, 239);
    while(1) {
	float startTime = (float)clock() / CLOCKS_PER_SEC;
	for (int i = 0; i < numframes; i++)
	{
		writeFramePregenerated(frames[i], frameSize);
	}

	float fps = (1 / (((float) clock() / CLOCKS_PER_SEC) - startTime)) / 2;

	printf("Frame time: %3.5f\n", fps);
   	
 }

    close_gpmc();

    return 0;
}
