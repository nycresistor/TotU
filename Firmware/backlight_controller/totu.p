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

// Register map
// Command stuff from our python script
#define command 	r0
#define count		r1
#define gpio0_mask  r2
#define gpio1_mask  r3
#define gpio2_mask	r4
#define gpio3_mask	r5

// Registers for handling 8 channels
// of PWM output at a time
#define group0      r6
#define group1      r7
#define group2      r8
#define group3      r9
#define group4      r10
#define group5      r11
#define group6      r12
#define group7      r13

// For storing gpio memory addresses
#define gpio0_base	r20
#define gpio1_base	r21
#define gpio2_base	r22
#define gpio3_base	r23

// For storing data that will eventuall
// be written out to the gpio hardware
#define set_out0    r24
#define set_out1    r25
#define set_out2	r26
#define set_out3	r27

    // Prime these registers with the memory
    // locations of our GPIO hardware
	MOV gpio0_base, GPIO0
	MOV gpio1_base, GPIO1
	MOV gpio2_base, GPIO2
	MOV gpio3_base, GPIO3

READ_LOOP:
    
    // Read command, count, and gpio[0-3]_mask
    // from data memory into the registers
    LBCO command, CONST_PRUDRAM, 0, 6*4

    // Wait for a non-zero command byte
    QBEQ READ_LOOP, command, #0

    // Command of 0xFF is the signal to exit
    QBEQ EXIT, command, 0x000000FF

    // For each group of 8 channels
    // load them from data ram into the registers
    // then store them in shared ram for later
    // Is there a better way to do this?
    LBCO group0, CONST_PRUDRAM, 6*4, 8*4
    SBCO group0, CONST_SHAREDRAM, 0, 8*4

    LBCO group0, CONST_PRUDRAM, 14*4, 8*4
    SBCO group0, CONST_SHAREDRAM, 8*4, 8*4

    LBCO group0, CONST_PRUDRAM, 22*4, 8*4
    SBCO group0, CONST_SHAREDRAM, 16*4, 8*4

    LBCO group0, CONST_PRUDRAM, 30*4, 8*4
    SBCO group0, CONST_SHAREDRAM, 24*4, 8*4

    // Chuck the used gpio pins into set_out[0-3]
    // this sets all channels to high, and we'll
    // turn them off one-by-one before writing
    // them out to hardware
    MOV set_out0, gpio0_mask
    MOV set_out1, gpio1_mask
    MOV set_out2, gpio2_mask
    MOV set_out3, gpio3_mask

// We'll loop through this <count> number of times
OUTPUT_LOOP:
    		QBEQ READ_LOOP, count, #0

            // Load in the first 8 channels from shared ram
            LBCO group0, CONST_SHAREDRAM, 0, 8*4
            
            // Subtract one from each channel
            SUB group0, group0, #1
            SUB group1, group1, #1
            SUB group2, group2, #1
            SUB group3, group3, #1
            SUB group4, group4, #1
            SUB group5, group5, #1
            SUB group6, group6, #1
            SUB group7, group7, #1
            
            // When each channel hits zero, 
            // clear its bit, otherwise skip
            // to the next channel
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

            // Store the counters for later
            SBCO group0, CONST_SHAREDRAM, 0, 8*4

// = = = = = = = = = = = = = = = = = = =
// Load in channels 8 - 15

            LBCO group0, CONST_SHAREDRAM, 8*4, 8*4

            SUB group0, group0, #1
            SUB group1, group1, #1
            SUB group2, group2, #1
            SUB group3, group3, #1
            SUB group4, group4, #1
            SUB group5, group5, #1
            SUB group6, group6, #1
            SUB group7, group7, #1
           
            QBNE SKIP8, group0, #0
            CLR set_out1.t17
    SKIP8:
            QBNE SKIP9, group1, #0
            CLR set_out0.t14
    SKIP9:
            QBNE SKIP10, group2, #0
            CLR set_out0.t3
    SKIP10:
            QBNE SKIP11, group3, #0
            CLR set_out0.t15
    SKIP11:
            QBNE SKIP12, group4, #0
            CLR set_out0.t13
    SKIP12:
            QBNE SKIP13, group5, #0
            CLR set_out0.t2
    SKIP13:
            QBNE SKIP14, group6, #0
            CLR set_out0.t4
    SKIP14:
            QBNE SKIP15, group7, #0
            CLR set_out0.t12
    SKIP15:

            SBCO group0, CONST_SHAREDRAM, 8*4, 8*4

// = = = = = = = = = = = = = = = = = = =
// Load in channels 16 - 23

            LBCO group0, CONST_SHAREDRAM, 16*4, 8*4

            SUB group0, group0, #1
            SUB group1, group1, #1
            SUB group2, group2, #1
            SUB group3, group3, #1
            SUB group4, group4, #1
            SUB group5, group5, #1
            SUB group6, group6, #1
            SUB group7, group7, #1
           
            QBNE SKIP16, group0, #0
            CLR set_out0.t11
    SKIP16:
            QBNE SKIP17, group1, #0
            CLR set_out0.t10
    SKIP17:
            QBNE SKIP18, group2, #0
            CLR set_out2.t17
    SKIP18:
            QBNE SKIP19, group3, #0
            CLR set_out0.t9
    SKIP19:
            QBNE SKIP20, group4, #0
            CLR set_out2.t16
    SKIP20:
            QBNE SKIP21, group5, #0
            CLR set_out0.t8
    SKIP21:
            QBNE SKIP22, group6, #0
            CLR set_out2.t13
    SKIP22:
            QBNE SKIP23, group7, #0
            CLR set_out2.t12
    SKIP23:

            SBCO group0, CONST_SHAREDRAM, 16*4, 8*4

// = = = = = = = = = = = = = = = = = = =
// Load in channels 24 - 32

            LBCO group0, CONST_SHAREDRAM, 24*4, 8*4

            SUB group0, group0, #1
            SUB group1, group1, #1
            SUB group2, group2, #1
            SUB group3, group3, #1
            SUB group4, group4, #1
            SUB group5, group5, #1
            SUB group6, group6, #1
            SUB group7, group7, #1
           
            QBNE SKIP24, group0, #0
            CLR set_out2.t15
    SKIP24:
            QBNE SKIP25, group1, #0
            CLR set_out2.t14
    SKIP25:
            QBNE SKIP26, group2, #0
            CLR set_out2.t11
    SKIP26:
            QBNE SKIP27, group3, #0
            CLR set_out2.t10
    SKIP27:
            QBNE SKIP28, group4, #0
            CLR set_out2.t9
    SKIP28:
            QBNE SKIP29, group5, #0
            CLR set_out2.t8
    SKIP29:
            QBNE SKIP30, group6, #0
            CLR set_out2.t7
    SKIP30:
            QBNE SKIP31, group7, #0
            CLR set_out2.t6
    SKIP31:

            SBCO group0, CONST_SHAREDRAM, 24*4, 8*4

            // Write out the set_out[0-3] registers to
            // the gpio memory locations
            SBBO set_out0, gpio0_base, GPIO_DATAOUT, 4
            SBBO set_out1, gpio1_base, GPIO_DATAOUT, 4
            SBBO set_out2, gpio2_base, GPIO_DATAOUT, 4
            SBBO set_out3, gpio3_base, GPIO_DATAOUT, 4
            
            // Subtract 1 from our count variable
            SUB count, count, 1

            // Loop back to the top of OUTPUT_LOOP
            QBA OUTPUT_LOOP

EXIT:
    // Set everything to off and write it all out
    // to the hardware
    MOV set_out0, #0
    MOV set_out1, #0
    MOV set_out2, #0
    MOV set_out3, #0
    SBBO set_out0, gpio0_base, GPIO_DATAOUT, 4
    SBBO set_out1, gpio1_base, GPIO_DATAOUT, 4
    SBBO set_out2, gpio2_base, GPIO_DATAOUT, 4
    SBBO set_out3, gpio3_base, GPIO_DATAOUT, 4
    
    // Signal that our PRU program has ended
    MOV R31.b0, PRU0_ARM_INTERRUPT+16
    HALT
