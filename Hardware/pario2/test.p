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
#define delay_count	r3
#define pru0r30_data0	r10
#define pru0r30_data1	r11
#define pru0r30_data2	r12
#define pru0r30_data3	r13
#define pru0r30_data4	r14
#define pru0r30_data5	r15
#define pru0r30_data6	r16
#define pru0r30_data7	r17

#define delay_iter	r20
#define OCMC_RAM #0x40300000

mov r30, 0<<14

mov r10, OCMC_RAM
mov r1, #0x66666666
mov r2, #0x55555555
SBBO r1, r10, 0, 8

EXIT:
#ifdef AM33XX
    // Send notification to Host for program completion
    MOV R31.b0, PRU0_ARM_INTERRUPT+16
#else
    MOV R31.b0, PRU0_ARM_INTERRUPT
#endif

    HALT
