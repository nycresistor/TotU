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
#include "gpmc.h"


#define width 320
#define height 240

uint16_t * screen0;

int main (int argc, char *argv[])
{

    setup_gpmc();
	
    setup_tft();

    screen0 = (uint16_t *) calloc(width * height, sizeof(uint16_t));
  
    printf("Sending black\n");

    setAddrWindow(0, 0, height - 1, width - 1);
//    writeFrame(screen0);
    
  
    for (int i = 0; i < width * height; i++) {
	writeData8(0x00);
    }


    

    close_gpmc();

    return 0;
}
