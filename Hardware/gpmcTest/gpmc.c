#include "gpmc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int fd;

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
 
void setup_gpmc()
{

    fd = open ("/dev/logibone_mem", O_RDWR | O_SYNC);

}

void write8(uint8_t data)
{
 //  printf("Got Byte %02x\n", data);
   uint8_t out[8]; 
   for (int i = 7; i >= 0; i--) {
	out[i] = CHECK_BIT(data, i) > 0 ? 0xFF : 0x00;
 }

/*
printf("\n");
   for (int i = 7; i >= 0; i--) {
	printf("%02x ", out[i]);
   }
printf("\n");
*/

for (int i = 7; i >= 0; i--)
{
	pwrite(fd, &(out[i]), 1, 0);
}


//pwrite(fd, &out, 8, 0);

//   	printf("Writing byte %02x\n", out);
//    pwrite(fd, &out, 8, 0);
}

void write16(uint16_t data)
{
    
}

void writeByte(uint8_t data)
{
    pwrite(fd, &data, 1, 0);
}

void gpmcWrite(uint16_t * data, uint32_t len)
{
    uint8_t * output = (uint8_t *) malloc(len * 2 * sizeof(uint8_t));
    for (int i = 0; i < len; i+= 2)
    {
	output[i] = *(data + i) >> 8;
	output[i + 1] = *(data + i) & 0xFF;

    }
    
    uint8_t out = 0x00;
   
    printf("Writing screen of length %d\n", len);
    for (int i = 0; i < len * 2; i++) pwrite(fd, &out, 1, 0);	
}

void close_gpmc()
{
    close(fd);
}
