#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "util.h"


int main(void) 
{
	printf("Initializing PRU\n");
	prussdrv_init();		

	printf("Initializing PRU memory mapping\n");
	int ret = prussdrv_open(PRU_EVTOUT_0);
	printf("RET:%08x\n", ret);
	if (ret)
		die("prussdrv_open open failed\n");

	printf("Initializing PRU Interupts\n");
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
	prussdrv_pruintc_init(&pruss_intc_initdata);

	printf("Initializing L3 memory\n");
	int mmap_fd;
	mmap_fd = open("/dev/mem", O_RDWR|O_SYNC);
	
	uint32_t * const l3_mem = mmap(0, 64*1024, PROT_READ | PROT_WRITE, MAP_SHARED, mmap_fd, 0x40300000);
	if (l3_mem == MAP_FAILED) printf("Map Failed, asshole\n");
	//printf("Errno:%08x\n", errno);

	*l3_mem = 0x12345678;
	printf("MEM:%08x\n", *l3_mem);
	
	printf("Starting PRU\n");
	prussdrv_exec_program(0, "./test.bin");

	printf("Shutting PRU back down\n");
	
	prussdrv_pru_wait_event(PRU_EVTOUT_0);
	prussdrv_pru_clear_event(PRU0_ARM_INTERRUPT);
	
	printf("MEM:%08x\n", *l3_mem);
	printf("MEM:%08x\n", *(l3_mem+1));
	
	prussdrv_pru_disable(0);
	prussdrv_exit();
}
