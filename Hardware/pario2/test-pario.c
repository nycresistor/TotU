/** \file
 * Test the pario firmware with a square wave
 */
#include <stdio.h>
#include "pario.h"

int main(void)
{
	printf("1\n");
	pario_t * const p = pario_init(0);
	printf("2\n");
	uint32_t * const bits = (uint32_t*) p->cmd->data;
	printf("3\n");


	printf("virt: %p\n", (const void*) p->virt);
	printf("phys: %p\n", (const void*) p->phys);
	printf("cmd:  %p\n", (const void*) p->cmd);

	// generate a square wave on all ports
	for (int i = 0 ; i < 256 ; i += 2)
	{
		bits[i] = 0xFFFFFFFF & ~(1 << 15);
		bits[i+1] = 0x00000000;
	}

	while (1)
	{
		sleep(1);
		p->cmd->cmd = 1;

		//sleep(1);
		//p->cmd->phys_addr = 0;
	}

	return 0;
}
