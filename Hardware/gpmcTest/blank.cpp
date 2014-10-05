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



int main (int argc, char *argv[])
{

    setup_gpmc();
	
    setup_tft();

    close_gpmc();

    return 0;
}
