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
 
void setup_gpmc()
{

    fd = open ("/dev/logibone_mem", O_RDWR | O_SYNC);

}

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

void gpmcWrite(uint16_t * data, uint32_t len)
{

	float startTime = (float) clock() / CLOCKS_PER_SEC;

//	printf("Writing block of %d\n", len);
	uint16_t * output = (uint16_t *) malloc(len * 16 * sizeof(uint16_t));
//	printf("Output of %d\n", (len * 16 * sizeof(uint16_t)));

	for (int word = 0; word < len; word++) {
		for (int bit = 0; bit < 16; bit++) {
			output[(word * 16) + (15 - bit)] = CHECK_BIT(*(data + word), bit) > 0 ? 0xFFFF : 0x0000;
		}
	}

	float elapsed = ((float) clock() / CLOCKS_PER_SEC) - startTime;
	printf("Bitslicing time: %3.5f\n", elapsed);

//	printf("Writing block of %d\n", (16 * sizeof(uint16_t) * len));
	
	startTime = (float) clock() / CLOCKS_PER_SEC;

	uint16_t * out = (uint16_t *) malloc(BLOCK_SIZE * sizeof(uint16_t));
	for (int block = 0; block < (len * 16) / BLOCK_SIZE; block++)
	{
		for (int word = 0; word < BLOCK_SIZE; word++)
		{
			out[word] = output[(block * BLOCK_SIZE) + word];
		}
		
//		printf("Writing block %d\n", block);
		pwrite(fd, out, BLOCK_SIZE * sizeof(uint16_t), 0);
	}

	elapsed = ((float) clock() / CLOCKS_PER_SEC) - startTime;
	printf("Outputting time: %3.5f\n", elapsed);	

}

void close_gpmc()
{
    close(fd);
}
