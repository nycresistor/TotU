#ifndef _pario_h_
#define _pario_h_

/** \file
 * Parallel Output driver.
 *
 * pario: Latin for "to bring forth", which this does in abundance.
 * Up to 150 Mbps of bandwidth using the PRU.
 */
#include <stdint.h>


/** Command structure for the pario PRU firmware.
 *
 * Each iteration these eight words are read from the PRU shared memory.
 * The phys_addr must point to the buffer in the PRU's map of the
 * DDR.  
 *
 * If any of the masks are zero the store instruction will not be
 * executed, which will speed up the output by a few hundred KHz.
 */
typedef struct
{	
	uint32_t cmd;
	uint32_t data[256];
} pario_cmd_t;


typedef struct
{
	pario_cmd_t * const cmd;
	void * const virt;
	const uintptr_t phys;
} pario_t;


#endif
