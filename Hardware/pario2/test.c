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

#define DISABLE_L3RAM_SUPPORT

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
	void * pru_l3_mem;
	prussdrv_map_l3mem(&pru_l3_mem);


	printf("%08x\n", pru_l3_mem);

	printf("Shutting PRU back down\n");
	//prussdrv_pru_wait_event(PRU_EVTOUT_0);
	prussdrv_pru_clear_event(PRU0_ARM_INTERRUPT);
	prussdrv_pru_disable(0);
	prussdrv_exit();
}
