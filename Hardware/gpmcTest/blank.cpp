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

#define OE_ADDR 0x134
#define GPIO_DATAOUT 0x13C
#define GPIO_DATAIN 0x138
#define GPIO0_ADDR 0x44E07000
#define GPIO1_ADDR 0x4804C000
#define GPIO2_ADDR 0x481AC000
#define GPIO3_ADDR 0x481AF000

#define DATA 0
#define CMD 1

// void writeALot(uint16_t addr, uint16_t * data, uint16_t len);
void write8(uint8_t data);
void writeData8(uint8_t data);
void writeCommand8(uint8_t data);

int fd = 0;


int main (int argc, char *argv[])
{
    // open fpga memory device
    fd = open ("/dev/logibone_mem", O_RDWR | O_SYNC);

    

    setup_tft();
	
    close (fd);

    return 0;
}

void writeData8(uint8_t data)
{
    setDC(DATA);
    write8(data);	
}

void writeCommand8(uint8_t cmd)
{
    setDC(CMD);
    write8(cmd);
}

void write8(uint8_t data)
{
   uint8_t out; 
   for (int i = 7; i >= 0; i--) {	
    	out = data && (1 << i) > 0 ? 0xff : 0;
	    pwrite(fd, &out, 1, 0);
    }
}

// void writeALot(uint16_t addr, uint16_t * data, uint16_t len)
// {
//     pwrite (fd, data, len, addr);
// }
