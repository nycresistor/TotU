/** \file
 * Test the pario firmware with a square wave
 */
#include <stdio.h>
#include "pario.h"

int main(void)
{
	pario_t * const p = pario_init(0);


	printf("virt: %p\n", (const void*) p->virt);
	printf("phys: %p\n", (const void*) p->phys);
	printf("cmd:  %p\n", (const void*) p->cmd);
	printf("output0:  %08x\n", p->cmd->output0);


	p->cmd->period = 256;

	while (1)
	{       
                for (int i = 1; i < 256; i++) {
                        p->cmd->output0 = i;
                        usleep(10000);
                }
		for (int i = 255; i > 0; i--) {
			p->cmd->output0 = i;
			usleep(10000);
		}
	}

	return 0;
}
