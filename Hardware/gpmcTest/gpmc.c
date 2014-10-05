#include "gpmc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int fd;
 
void setup_gpmc()
{

    fd = open ("/dev/logibone_mem", O_RDWR | O_SYNC);

}

void write8(uint8_t data)
{
   uint8_t out; 
   for (int i = 7; i >= 0; i--) {	
    	out = data && (1 << i) > 0 ? 0xff : 0;
	    pwrite(fd, &out, 1, 0);
    }
}

void close_gpmc()
{
    close(fd);
}
