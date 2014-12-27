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
#define gpio2_mask	r2
#define gpio3_mask	r3
#define output0     r4
#define output1     r5
#define output2     r6
#define output3     r7
#define output4     r8
#define output5     r9
#define output6     r10
#define output7     r11
#define output8     r12
#define output9     r13
#define output10    r14
#define output11    r15
#define output12    r16
#define output13    r17
#define output14    r18
#define output15    r19

#define gpio0_base	r20
#define gpio1_base	r21
#define gpio2_base	r22
#define gpio3_base	r23

#define set_out2	r24
#define set_out3	r25

	MOV gpio0_base, GPIO0
	MOV gpio1_base, GPIO1
	MOV gpio2_base, GPIO2
	MOV gpio3_base, GPIO3

RESET:
	MOV data_addr, 0
	SBCO data_addr, CONST_PRUDRAM, 0, 4

READ_LOOP:
        // Load the eight word command structure from the PRU DRAM, which is
	// mapped into the user space.
        LBCO      data_addr, CONST_PRUDRAM, 0, 20*4

        // Wait for a non-zero command
 //       QBEQ READ_LOOP, data_addr, #0

        // Command of 0xFF is the signal to exit
        QBEQ EXIT, data_addr, #0xFF

        MOV set_out2, gpio2_mask
        MOV set_out3, gpio3_mask

OUTPUT_LOOP:
		QBEQ RESET, count, #0
                
                SUB output0, output0, 1
                SUB output1, output1, 1
                SUB output2, output2, 1
                SUB output3, output3, 1
                SUB output4, output4, 1
                SUB output5, output5, 1
                SUB output6, output6, 1
                SUB output7, output7, 1
                SUB output8, output8, 1
                SUB output9, output9, 1
                SUB output10, output10, 1
                SUB output11, output11, 1
                SUB output12, output12, 1
                SUB output13, output13, 1
                SUB output14, output14, 1
                SUB output15, output15, 1

                QBNE SKIP0, output0, #0
                CLR set_out2.t6
SKIP0:
                QBNE SKIP1, output1, #0
                CLR set_out2.t7
SKIP1:
                QBNE SKIP2, output2, #0
                CLR set_out2.t8
SKIP2:
                QBNE SKIP3, output3, #0
                CLR set_out2.t9
SKIP3:
                QBNE SKIP4, output4, #0
                CLR set_out2.t10
SKIP4:
                QBNE SKIP5, output5, #0
                CLR set_out2.t11
SKIP5:
                QBNE SKIP6, output6, #0
                CLR set_out2.t12
SKIP6:
                QBNE SKIP7, output7, #0
                CLR set_out2.t13
SKIP7:
                QBNE SKIP8, output8, #0
                CLR set_out3.t14 
SKIP8:
                QBNE SKIP9, output9, #0
                CLR set_out3.t15
SKIP9:
                QBNE SKIP10, output10, #0
                CLR set_out3.t16
SKIP10:
                QBNE SKIP11, output11, #0
                CLR set_out3.t17
SKIP11:
                QBNE SKIP12, output12, #0
                CLR set_out3.t18
SKIP12:
                QBNE SKIP13, output13, #0
                CLR set_out3.t19
SKIP13:
                QBNE SKIP14, output14, #0
                CLR set_out3.t20
SKIP14:
                QBNE SKIP15, output15, #0
                CLR set_out3.t21
SKIP15:
                SBBO set_out2, gpio2_base, GPIO_DATAOUT, 4
                SBBO set_out3, gpio3_base, GPIO_DATAOUT, 4
                SUB count, count, 1
		QBA OUTPUT_LOOP

EXIT:
#ifdef AM33XX
    // Send notification to Host for program completion
    MOV R31.b0, PRU0_ARM_INTERRUPT+16
#else
    MOV R31.b0, PRU0_ARM_INTERRUPT
#endif

    HALT
