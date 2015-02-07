#include "slicer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>     /* strcat */
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define SCREENS_PER_BANK 16

uint16_t imageData[SCREENS_PER_BANK][76800];

void readImages(char * inputfolder, int framenumber, int bankNum)
{
	uint8_t image[230400];
	char bank = bankNum == 0 ? 'A' : 'B';

	for (int screen = 0; screen < SCREENS_PER_BANK; screen++)
	{	
		// Open the file at inputfolder/framenumber-i.rgb
		char filename[100];
		sprintf(filename, "%s/%02d-%c%02d.rgb", inputfolder, framenumber, bank, screen);
		printf("Reading from %s\n", filename);

		if (access(filename, F_OK) == -1) {
			printf("ERROR: Missing file: %s\n", filename);
			exit(-1);
		}

		// Read the file into the image array
		FILE * filePointer = fopen(filename, "rb");
		fread(image, sizeof(uint8_t), 230400, filePointer);

		// Iterate through each pixel, pull out each color
		for (int q = 0; q < 76800; q++)
		{
			uint8_t r = image[(q * 3) + 0];
			uint8_t g = image[(q * 3) + 1];
			uint8_t b = image[(q * 3) + 2];

			// uint8_t hi = (r & 0xF8) + (g >> 5);
			// uint8_t lo = (g & 0x1C) + ((b & 0xF8) >> 3);

			// Convert to a 16-bit integer
			uint8_t hi = ((r >> 3) << 3) | (g >> 5);
			uint8_t lo = (g << 5) | (b >> 3);

			// Throw the 16-bit int into the imageData array
			imageData[screen][q] = (hi << 8) | lo;
		}

		fclose(filePointer);
	}
}

const char *byte_to_binary(int x)
{
    static char b[17];
    b[0] = '\0';

    int z;
    for (z = 32768; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

void outputBinary(char * filename)
{
	// Open the outputFile
	FILE * outputFile = fopen(filename, "wb");

	for (int byte = 0; byte < 76800; byte++)
	{
		uint16_t * byteArray[16];
		for (int x = 0; x < 16; x++)
		{
			byteArray[x] = &imageData[x][byte];
		}

		uint16_t outArray[16];
		for (int q = 0; q < 16; q++)
		{
			uint16_t outByte = 0;
			for (int i = 0; i < 16; i++)
			{	
				// printf("q %d i %d byteArray[i] %s\n", q, i, byte_to_binary(*byteArray[i]));
				outByte <<= 1;
				outByte |= (*byteArray[i] >> (15 - q)) & 0x01;
			}
			// printf("\tOutput Byte: %s\n\n", byte_to_binary(outByte));
			outArray[q] = outByte;
		}

		fwrite(outArray, sizeof(uint16_t), 16, outputFile);
	}

	fclose(outputFile);
}

int main (int argc, char *argv[])
{	

	// Check that an input folder was provided
	char * inputfolder = argv[1];
	if (inputfolder == NULL)
	{
		printf("Input folder not specified.\n");
		exit(-1);
	}

	// Check that an output folder was provided
	char * outputfolder = argv[2];
	if (argv[2] == NULL)
	{
		printf("Output folder not specified.\n");
	}

	// Check that the output folder exists or create it
	if (access(outputfolder, F_OK) == -1) {
		if (mkdir(outputfolder, 0777) == -1) {
			printf("Error creating output folder %s: %d\n", outputfolder, errno);
			exit(-1);
		}
	}

	// This only checks to see if the first screen of
	// each frame exists, does not check for all screens
	int numFrames = 0;
	for (int frame = 0; frame < 100; frame++)
	{
		char filename[100];
		sprintf(filename, "%s/%02d-A00.rgb", inputfolder, frame);
		if (access(filename, F_OK) != -1) {
			numFrames = frame + 1;
		} else {
			break;
		}
	}

	printf("Found %d frames\n", numFrames);

	for (int frame = 0; frame < numFrames; frame++)
	{
		for (int bankNum = 0; bankNum < 2; bankNum++){

			char bank = bankNum == 0 ? 'A' : 'B';
			readImages(inputfolder, frame, bankNum);
			char filename[100];
			sprintf(filename, "%s/%02d-%c.bin", outputfolder, frame, bank);
			outputBinary(filename);

		}	
	}
}