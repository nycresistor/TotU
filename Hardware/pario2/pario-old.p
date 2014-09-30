// \file
/** PRU based parallel output driver.
 *
 * Generates up to 55 output signals from a memory mapped user buffer.
 * Optionally also drive a clock on one pin.
 *
 */
.origin 0
.entrypoint START

#include "pru.hp"

START:
    // Enable OCP master port
    // clear the STANDBY_INIT bit in the SYSCFG register,
    // otherwise the PRU will not be able to write outside the
    // PRU memory space and to the BeagleBon's pins.
    LBCO	r0, C4, 4, 4
    CLR		r0, r0, 4
    SBCO	r0, C4, 4, 4

    // Configure the programmable pointer register for PRU0 by setting
    // c28_pointer[15:0] field to 0x0120.  This will make C28 point to
    // 0x00012000 (PRU shared RAM).
    MOV		r0, 0x00000120
    MOV		r1, CTPPR_0
    ST32	r0, r1

    // Configure the programmable pointer register for PRU0 by setting
    // c31_pointer[15:0] field to 0x0010.  This will make C31 point to
    // 0x80001000 (DDR memory).
    MOV		r0, 0x00100000
    MOV		r1, CTPPR_1
    ST32	r0, r1

// register map
// The first 8 must agree with the struct pario_cmd_t in pario.h
#define data_addr	r0
#define count		r1
#define pru0r30_mask	r2
#define delay_count	r7

#define gpio0_base	r10
#define gpio1_base	r11
#define gpio2_base	r12
#define gpio3_base	r13
#define pru0r30_data	r14
#define gpio1_data	r15
#define gpio2_data	r16
#define gpio3_data	r17
#define clr_out		r18
#define set_out		r19 // must be clr_out+1
#define delay_iter	r20


MOV r30, 1<<14

RESET:
	MOV data_addr, 0
	SBCO data_addr, CONST_PRUDRAM, 0, 4

READ_LOOP:
        // Load the eight word command structure from the PRU DRAM, which is
	// mapped into the user space.
        LBCO      data_addr, CONST_PRUDRAM, 0, 8*2
        // Wait for a non-zero command
        // QBEQ READ_LOOP, data_addr, #0
        // Command of 0xFF is the signal to exit
        // QBEQ EXIT, data_addr, #0xFF

OUTPUT_LOOP:

	//QBEQ RESET, count, #0

	// read four gpio outputs worth of data
	//LBBO pru0r30_data, data_addr, 0, 4
		
	mov r30, 1<<14
	//mov r30, 0<<14
	
	// advance to the next output
	//ADD data_addr, data_addr, 4*4
	//SUB count, count, 1
	
	QBA OUTPUT_LOOP

EXIT:
#ifdef AM33XX
    // Send notification to Host for program completion
    MOV R31.b0, PRU0_ARM_INTERRUPT+16
#else
    MOV R31.b0, PRU0_ARM_INTERRUPT
#endif

    HALT
