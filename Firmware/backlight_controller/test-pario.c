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
	printf("gpio2_mask:  %08x\n", p->cmd->gpio2_mask);
	printf("gpio3_mask:  %08x\n", p->cmd->gpio3_mask);

	p->cmd->period = 256;

	while (1)
	{       
                for (int i = 1; i < 256; i++) {
                        p->cmd->output0 = i;
                        p->cmd->output1 = 256 - i;
			p->cmd->output8 = i;
			p->cmd->output9 = 256 - i;
			usleep(1000);
                }
		for (int i = 255; i > 0; i--) {
			p->cmd->output0 = i;
                        p->cmd->output1 = 256 - i;
			p->cmd->output8 = i;
			p->cmd->output9 = 256 - i;
			usleep(1000);
		}
	}

	return 0;
}
