











.origin 0
.entrypoint START



.macro LD32
.mparam dst,src
    LBBO dst,src,#0x00,4
.endm

.macro LD16
.mparam dst,src
    LBBO dst,src,#0x00,2
.endm

.macro LD8
.mparam dst,src
    LBBO dst,src,#0x00,1
.endm

.macro ST32
.mparam src,dst
    SBBO src,dst,#0x00,4
.endm

.macro ST16
.mparam src,dst
    SBBO src,dst,#0x00,2
.endm

.macro ST8
.mparam src,dst
    SBBO src,dst,#0x00,1
.endm







.macro stack_init
    mov r0, (0x2000 - 0x200)
.endm

.macro push
.mparam reg, cnt
    sbbo reg, r0, 0, 4*cnt
    add r0, r0, 4*cnt
.endm

.macro pop
.mparam reg, cnt
    sub r0, r0, 4*cnt
    lbbo reg, r0, 0, 4*cnt
.endm


START:




    LBCO r0, C4, 4, 4
    CLR r0, r0, 4
    SBCO r0, C4, 4, 4




    MOV r0, 0x00000120
    MOV r1, 0x22028
    ST32 r0, r1




    MOV r0, 0x00100000
    MOV r1, 0x2202C
    ST32 r0, r1

 MOV r20, (0x44E07000 + 0x100)
 MOV r21, (0x4804c000 + 0x100)
 MOV r22, (0x481AC000 + 0x100)
 MOV r23, (0x481AE000 + 0x100)

READ_LOOP:



    LBCO r0, C24, 0, 6*4


    QBEQ READ_LOOP, r0, #0


    QBEQ EXIT, r0, 0x000000FF





    LBCO r6, C24, 6*4, 8*4
    SBCO r6, C28, 0, 8*4

    LBCO r6, C24, 14*4, 8*4
    SBCO r6, C28, 8*4, 8*4

    LBCO r6, C24, 22*4, 8*4
    SBCO r6, C28, 16*4, 8*4

    LBCO r6, C24, 30*4, 8*4
    SBCO r6, C28, 24*4, 8*4






    MOV r24, r2
    MOV r25, r3
    MOV r26, r4
    MOV r27, r5


OUTPUT_LOOP:
      QBEQ READ_LOOP, r1, #0


            LBCO r6, C28, 0, 8*4


            SUB r6, r6, #1
            SUB r7, r7, #1
            SUB r8, r8, #1
            SUB r9, r9, #1
            SUB r10, r10, #1
            SUB r11, r11, #1
            SUB r12, r12, #1
            SUB r13, r13, #1




            QBNE SKIP0, r6, #0
            CLR r24.t7
    SKIP0:
            QBNE SKIP1, r7, #0
            CLR r24.t20
    SKIP1:
            QBNE SKIP2, r8, #0
            CLR r27.t15
    SKIP2:
            QBNE SKIP3, r9, #0
            CLR r27.t14
    SKIP3:
            QBNE SKIP4, r10, #0
            CLR r27.t19
    SKIP4:
            QBNE SKIP5, r11, #0
            CLR r27.t16
    SKIP5:
            QBNE SKIP6, r12, #0
            CLR r27.t21
    SKIP6:
            QBNE SKIP7, r13, #0
            CLR r27.t17
    SKIP7:


            SBCO r6, C28, 0, 8*4




            SBBO r2, r20, (0x194 - 0x100), 4
            SBBO r24, r20, (0x190 - 0x100), 4




            LBCO r6, C28, 8*4, 8*4

            SUB r6, r6, #1
            SUB r7, r7, #1
            SUB r8, r8, #1
            SUB r9, r9, #1
            SUB r10, r10, #1
            SUB r11, r11, #1
            SUB r12, r12, #1
            SUB r13, r13, #1

            QBNE SKIP8, r6, #0
            CLR r25.t17
    SKIP8:
            QBNE SKIP9, r7, #0
            CLR r24.t14
    SKIP9:
            QBNE SKIP10, r8, #0
            CLR r24.t3
    SKIP10:
            QBNE SKIP11, r9, #0
            CLR r24.t15
    SKIP11:
            QBNE SKIP12, r10, #0
            CLR r24.t13
    SKIP12:
            QBNE SKIP13, r11, #0
            CLR r24.t2
    SKIP13:
            QBNE SKIP14, r12, #0
            CLR r24.t4
    SKIP14:
            QBNE SKIP15, r13, #0
            CLR r24.t12
    SKIP15:

            SBCO r6, C28, 8*4, 8*4

            SBBO r3, r21, (0x194 - 0x100), 4
            SBBO r25, r21, (0x190 - 0x100), 4




            LBCO r6, C28, 16*4, 8*4

            SUB r6, r6, #1
            SUB r7, r7, #1
            SUB r8, r8, #1
            SUB r9, r9, #1
            SUB r10, r10, #1
            SUB r11, r11, #1
            SUB r12, r12, #1
            SUB r13, r13, #1

            QBNE SKIP16, r6, #0
            CLR r24.t11
    SKIP16:
            QBNE SKIP17, r7, #0
            CLR r24.t10
    SKIP17:
            QBNE SKIP18, r8, #0
            CLR r26.t17
    SKIP18:
            QBNE SKIP19, r9, #0
            CLR r24.t9
    SKIP19:
            QBNE SKIP20, r10, #0
            CLR r26.t16
    SKIP20:
            QBNE SKIP21, r11, #0
            CLR r24.t8
    SKIP21:
            QBNE SKIP22, r12, #0
            CLR r26.t13
    SKIP22:
            QBNE SKIP23, r13, #0
            CLR r26.t12
    SKIP23:

            SBCO r6, C28, 16*4, 8*4

            SBBO r4, r22, (0x194 - 0x100), 4
            SBBO r26, r22, (0x190 - 0x100), 4




            LBCO r6, C28, 24*4, 8*4

            SUB r6, r6, #1
            SUB r7, r7, #1
            SUB r8, r8, #1
            SUB r9, r9, #1
            SUB r10, r10, #1
            SUB r11, r11, #1
            SUB r12, r12, #1
            SUB r13, r13, #1

            QBNE SKIP24, r6, #0
            CLR r26.t15
    SKIP24:
            QBNE SKIP25, r7, #0
            CLR r26.t14
    SKIP25:
            QBNE SKIP26, r8, #0
            CLR r26.t11
    SKIP26:
            QBNE SKIP27, r9, #0
            CLR r26.t10
    SKIP27:
            QBNE SKIP28, r10, #0
            CLR r26.t9
    SKIP28:
            QBNE SKIP29, r11, #0
            CLR r26.t8
    SKIP29:
            QBNE SKIP30, r12, #0
            CLR r26.t7
    SKIP30:
            QBNE SKIP31, r13, #0
            CLR r26.t6
    SKIP31:

            SBCO r6, C28, 24*4, 8*4

            SBBO r5, r23, (0x194 - 0x100), 4
            SBBO r27, r23, (0x190 - 0x100), 4


            SUB r1, r1, 1


            QBA OUTPUT_LOOP

EXIT:


    SBBO r2, r20, (0x190 - 0x100), 4
    SBBO r3, r21, (0x190 - 0x100), 4
    SBBO r4, r22, (0x190 - 0x100), 4
    SBBO r5, r23, (0x190 - 0x100), 4


    MOV R31.b0, 19 +16
    HALT
