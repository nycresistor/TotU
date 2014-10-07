#include "gpmc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int fd;

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
 
// Open up the file descriptor to the logibone_mem kernel driver
void setup_gpmc()
{

    fd = open ("/dev/logibone_mem", O_RDWR | O_SYNC);

}

// This is used for writing a byte to all GPMC channels in parallel
// currently used for TFT setup commands
void write8(uint8_t data)
{
 //  printf("Got Byte %02x\n", data);
	uint16_t out[8]; 
	for (int i = 0; i < 8; i++) {
		out[7-i] = CHECK_BIT(data, i) > 0 ? 0xFFFF : 0x0000;
	}

	pwrite(fd, &out, 8 * sizeof(uint16_t), 0);
}

void writeByte(uint8_t data)
{
    pwrite(fd, &data, 1, 0);
}

// Write out chunk of data
// Need to rewrite to go faster and support arbitrary lengths
void gpmcWrite(uint16_t * data, uint32_t len)
{

	float startTime = (float) clock() / CLOCKS_PER_SEC;

//	printf("Writing block of %d\n", len);

	// Hold the bit expansion (every bit of a uint16_t becomes it's own uint16_t word)
	uint16_t * output = (uint16_t *) malloc(len * 16 * sizeof(uint16_t));

	// Do the bit expansion 
	// Right now this only supports one screen (hence CHECK_BIT() > 0 ? 0xFFFF : 0x0000)
	for (int word = 0; word < len; word++) {
		for (int bit = 0; bit < 16; bit++) {
			output[(word * 16) + (15 - bit)] = CHECK_BIT(*(data + word), bit) > 0 ? 0xFFFF : 0x0000;
		}
	}

	float elapsed = ((float) clock() / CLOCKS_PER_SEC) - startTime;
	printf("Bitslicing time: %3.5f\n", elapsed);

//	printf("Writing block of %d\n", (16 * sizeof(uint16_t) * len));
	
	startTime = (float) clock() / CLOCKS_PER_SEC;

	// Chunk the output into blocks of BLOCK_SIZE
	uint16_t * out = (uint16_t *) malloc(BLOCK_SIZE * sizeof(uint16_t));
	for (int block = 0; block < (len * 16) / BLOCK_SIZE; block++)
	{
		for (int word = 0; word < BLOCK_SIZE; word++)
		{
			out[word] = output[(block * BLOCK_SIZE) + word];
		}
		
//		printf("Writing block %d\n", block);

		// This is where the block (out) of size (BLOCK_SIZE * sizeof(uint16_t))
		// gets written to the GPMC (filedescriptor fd) using address 0 (addressing not used)
		// This could possibly be faster by diving into how the kernel module works
		// but the overhead goes down as BLOCK_SIZE goes up
		pwrite(fd, out, BLOCK_SIZE * sizeof(uint16_t), 0);
	}

	elapsed = ((float) clock() / CLOCKS_PER_SEC) - startTime;
	printf("Outputting time: %3.5f\n", elapsed);	

}

void close_gpmc()
{
    close(fd);
}
