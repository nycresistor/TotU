#include <stdio.h>
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

int main(void) 
{
	prussdrv_pru_clear_event(PRU0_ARM_INTERRUPT);
	prussdrv_pru_disable(0); 
	prussdrv_exit();
}
