#include "slicer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>     /* strcat */
#include <dirent.h>
#include <unistd.h>

uint16_t imageData[16][76800];
const char * outputfolder = "seq";

void readImages(char * inputfolder, int framenumber)
{
	uint8_t image[230400];

	for (int i = 0; i < 16; i++)
	{		
		char filename[100];
		sprintf(filename, "%s/%02d-%02d.rgb", inputfolder, framenumber, i);
		printf("Reading from %s\n", filename);

		if (access(filename, F_OK) == -1) {
			printf("ERROR: Missing file: %s\n", filename);
			exit(-1);
		}

		FILE * filePointer = fopen(filename, "rb");

		fread(image, sizeof(uint8_t), 230400, filePointer);

		for (int q = 0; q < 76800; q++)
		{
			uint8_t r = image[(q * 3) + 0];
			uint8_t g = image[(q * 3) + 1];
			uint8_t b = image[(q * 3) + 2];

			// uint8_t hi = (r & 0xF8) + (g >> 5);
			// uint8_t lo = (g & 0x1C) + ((b & 0xF8) >> 3);

			uint8_t hi = ((r >> 3) << 3) | (g >> 5);
			uint8_t lo = (g << 5) | (b >> 3);

			imageData[i][q] = (hi << 8) | lo;
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
	char * inputfolder = argv[1];
	if (inputfolder == NULL)
	{
		printf("Input folder not specified.\n");
		exit(-1);
	}
	if (argv[2] != NULL)
	{
		outputfolder = argv[2];
	}
	if (access(outputfolder, F_OK) == -1) {
		printf("Output folder %s doesn't exist.\n", outputfolder);
		exit(-1);
	}

	int numframes = 0;
	for (int i = 0; i < 100; i++)
	{
		char filename[100];
		sprintf(filename, "%s/%02d-00.rgb", inputfolder, i);
		if (access(filename, F_OK) != -1) {
			numframes = i + 1;
		} else {
			break;
		}
	}

	printf("Found %d frames\n", numframes);

	for (int frameno = 0; frameno < numframes; frameno++)
	{
		readImages(inputfolder, frameno);
		char filename[100];
		sprintf(filename, "%s/%02d.bin", outputfolder, frameno);
		outputBinary(filename);
	}
	
	
}