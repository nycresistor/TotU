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

uint16_t * frames[2][100];
// char sequence_filenames[256][256];

// int get_image_sequence(const char * folder, char filenames[256][256])
// {
//     DIR *d;
//     struct dirent *dir;
//     int numframes = 0;
//     d = opendir(folder);
//     if (d)
//     {
//         while ((dir = readdir(d)) != NULL)
//         {
//             char * ext = strrchr(dir->d_name, '.');
//             if (strcmp(ext, ".bin") == 0) {
//                 strcpy(filenames[numframes++], dir->d_name);
//             }
//         }
//         closedir(d);
//     } else {
//         printf("Folder does not exist.\n");
//         exit(-1);
//     }
//     return(numframes);
// }
	
int main (int argc, char *argv[])
{

    // Check that the input folder is specified
    char * inputfolder = argv[1];
    if (inputfolder == NULL)
    {
        printf("Input folder not specified.\n");
        exit(-1);
    }

    // Iterate through .bin files and count
    // how many frames there are to load in
    int numFrames = 0;
    for (int frame = 0; frame < 100; frame++)
    {
        char filename[100];
        sprintf(filename, "%s/%02d-A.bin", inputfolder, frame);
        if (access(filename, F_OK) != -1) {
            numFrames = frame + 1;
        } else {
            if (numFrames == 0) {
                printf("No frames found.\n");
                exit(-1);
            }
            break;
        }
    }

    // For each .bin frame found, calloc a frame in the frames array, 
    // assemble the filename, and read the file into the frame
    FILE * imageFile;
    for (int frame = 0; frame < numFrames; frame++)
    {
        for (int bankNum = 0; bankNum < 2; bankNum++)
        {
            char bank = bankNum == 0 ? 'A' : 'B';
            
            frames[bankNum][frame] = (uint16_t *) calloc(frameSize, sizeof(uint16_t));
            
            char filename[100];
            sprintf(filename, "%s/%02d-%c.bin", inputfolder, frame, bank);
            
            imageFile = fopen(filename, "rb");
            fread(frames[bankNum][frame], sizeof(uint16_t), frameSize, imageFile);
        }
    }

    // Set up the GPMC and send TFT setup commands
    setup_gpmc();
	tft_go();
    setRotation(3);

    // Set up timer and TFT window
    float lastTime = 0;
    printf("COMMENCE\n");
    setAddrWindow(0, 0, 319, 239);

    // Iterate through frames and write them to the GPMC
    while(1) {
    	float startTime = (float)clock() / CLOCKS_PER_SEC;
    	
        for (int frame = 0; frame < numFrames; frame++)
    	{
            for (int bank = 0; bank < 2; bank++) {
                activateBank(bank);
		writeFramePregenerated(frames[bank][frame], frameSize);
            }
    	}

    	float fps = (1 / (((float) clock() / CLOCKS_PER_SEC) - startTime)) / 2;
    	printf("Frame time: %3.5f\n", fps);
    }

    close_gpmc();
    return 0;
}
