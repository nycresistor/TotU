#include "gpmc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int fd;

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
 
void setup_gpmc()
{

    fd = open ("/dev/logibone_mem", O_RDWR | O_SYNC);

}

void write8(uint8_t data)
{
   printf("Got Byte %02x\n", data);
   uint8_t out; 
   for (int i = 7; i >= 0; i--) {	
	out = CHECK_BIT(data, i) > 0 ? 0xFF : 0x00;	

    	printf("Writing byte %02x\n", out);
	pwrite(fd, &out, 1, 0);
    }
}

void writeByte(uint8_t data)
{
    pwrite(fd, &data, 1, 0);
}

void close_gpmc()
{
    close(fd);
}
