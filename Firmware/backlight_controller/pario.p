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
#define gpio0_mask  r2
#define gpio1_mask  r3
#define gpio2_mask	r4
#define gpio3_mask	r5

#define group0      r6
#define group1      r7
#define group2      r8
#define group3      r9
#define group4      r10
#define group5      r11
#define group6      r12
#define group7      r13

#define gpio0_base	r20
#define gpio1_base	r21
#define gpio2_base	r22
#define gpio3_base	r23

#define set_out0    r24
#define set_out1    r25
#define set_out2	r26
#define set_out3	r27

	// MOV gpio0_base, GPIO0
	// MOV gpio1_base, GPIO1
	// MOV gpio2_base, GPIO2
	// MOV gpio3_base, GPIO3

RESET:
	// MOV data_addr, 0
	// SBCO data_addr, CONST_PRUDRAM, 0, 4
    


READ_LOOP:
        // Load the eight word command structure from the PRU DRAM, which is
	// mapped into the user space.
        // LBBO r2, data_addr, 0, 4
        // MOV r1, 0xbabe7175
        // SBBO r1, r2, 0, 4

        LBCO data_addr, CONST_PRUDRAM, 0, 6*4

        // Wait for a non-zero command
        QBEQ READ_LOOP, data_addr, #0

        // Command of 0xFF is the signal to exit
        QBEQ EXIT, data_addr, 0x000000FF

        LBCO group0, CONST_PRUDRAM, 6*4, 8*4


        MOV set_out0, gpio0_mask
        MOV set_out1, gpio1_mask
        MOV set_out2, gpio2_mask
        MOV set_out3, gpio3_mask

OUTPUT_LOOP:
		  QBEQ READ_LOOP, count, #0
                
                SUB group0, group0, #1
                SUB group1, group1, #1
                SUB group2, group2, #1
                SUB group3, group3, #1
                SUB group4, group4, #1
                SUB group5, group5, #1
                SUB group6, group6, #1
                SUB group7, group7, #1
               
                QBNE SKIP0, group0, #0
                CLR set_out0.t7
        SKIP0:
                QBNE SKIP1, group1, #0
                CLR set_out0.t20
        SKIP1:
                QBNE SKIP2, group2, #0
                CLR set_out3.t15
        SKIP2:
                QBNE SKIP3, group3, #0
                CLR set_out3.t14
        SKIP3:
                QBNE SKIP4, group4, #0
                CLR set_out3.t19
        SKIP4:
                QBNE SKIP5, group5, #0
                CLR set_out3.t16
        SKIP5:
                QBNE SKIP6, group6, #0
                CLR set_out3.t21
        SKIP6:
                QBNE SKIP7, group7, #0
                CLR set_out3.t17
        SKIP7:

        //         AND check, group0, 0x0000FF00
        //         QBNE SKIP2, check, #0
        //         CLR set_out0.t1 //??

        // SKIP2:

        //         AND check, group0, 0x000000FF
        //         QBNE SKIP3, check, #0
//         //         CLR set_out0.t2 //??

                SBBO set_out0, gpio0_base, GPIO_DATAOUT, 4
                SBBO set_out3, gpio3_base, GPIO_DATAOUT, 4
                SUB count, count, 1

                QBA OUTPUT_LOOP
        // QBA EXIT

EXIT:
    
    mov set_out0, #0
    mov set_out3, #0
    SBBO set_out0, gpio0_base, GPIO_DATAOUT, 4
    SBBO set_out3, gpio3_base, GPIO_DATAOUT, 4
    // SBCO group0, CONST_PRUDRAM, 0, 2*4
// #ifdef AM33XX
    // Send notification to Host for program completion
    MOV R31.b0, PRU0_ARM_INTERRUPT+16
// #else
//     MOV R31.b0, PRU0_ARM_INTERRUPT
// #endif

    HALT