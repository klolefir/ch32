# 1 "/home/klolefir/projects/ch32/v307/snippets/freertos/rtos_src/portable/GCC/RISC-V/portASM.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/home/klolefir/projects/ch32/v307/snippets/freertos/rtos_src/portable/GCC/RISC-V/portASM.S"
# 73 "/home/klolefir/projects/ch32/v307/snippets/freertos/rtos_src/portable/GCC/RISC-V/portASM.S"
# 1 "/home/klolefir/projects/ch32/v307/snippets/freertos/rtos_src/portable/GCC/RISC-V/chip_specific_extensions/RV32I_PFIC_no_extensions/freertos_risc_v_chip_specific_extensions.h" 1
# 140 "/home/klolefir/projects/ch32/v307/snippets/freertos/rtos_src/portable/GCC/RISC-V/chip_specific_extensions/RV32I_PFIC_no_extensions/freertos_risc_v_chip_specific_extensions.h"
.macro portasmSAVE_ADDITIONAL_REGISTERS

    .endm

.macro portasmRESTORE_ADDITIONAL_REGISTERS

    .endm
# 74 "/home/klolefir/projects/ch32/v307/snippets/freertos/rtos_src/portable/GCC/RISC-V/portASM.S" 2
# 93 "/home/klolefir/projects/ch32/v307/snippets/freertos/rtos_src/portable/GCC/RISC-V/portASM.S"
# #error portasmHANDLE_INTERRUPT must be defined to the function to be called to handle external/peripheral interrupts. portasmHANDLE_INTERRUPT can be defined on the assembler command line or in the appropriate freertos_risc_v_chip_specific_extensions.h header file. https:
# 107 "/home/klolefir/projects/ch32/v307/snippets/freertos/rtos_src/portable/GCC/RISC-V/portASM.S"
.global xPortStartFirstTask
.global SW_Handler
.global pxPortInitialiseStack

.extern pxCurrentTCB
.extern vTaskSwitchContext
.extern xISRStackTop

.align 8
.func
SW_Handler:
 addi sp, sp, -( 30 * 4 )
 sw x1, 1 * 4( sp )
 sw x5, 2 * 4( sp )
 sw x6, 3 * 4( sp )
 sw x7, 4 * 4( sp )
 sw x8, 5 * 4( sp )
 sw x9, 6 * 4( sp )
 sw x10, 7 * 4( sp )
 sw x11, 8 * 4( sp )
 sw x12, 9 * 4( sp )
 sw x13, 10 * 4( sp )
 sw x14, 11 * 4( sp )
 sw x15, 12 * 4( sp )
 sw x16, 13 * 4( sp )
 sw x17, 14 * 4( sp )
 sw x18, 15 * 4( sp )
 sw x19, 16 * 4( sp )
 sw x20, 17 * 4( sp )
 sw x21, 18 * 4( sp )
 sw x22, 19 * 4( sp )
 sw x23, 20 * 4( sp )
 sw x24, 21 * 4( sp )
 sw x25, 22 * 4( sp )
 sw x26, 23 * 4( sp )
 sw x27, 24 * 4( sp )
 sw x28, 25 * 4( sp )
 sw x29, 26 * 4( sp )
 sw x30, 27 * 4( sp )
 sw x31, 28 * 4( sp )

 csrr t0, mstatus
 sw t0, 29 * 4( sp )

 portasmSAVE_ADDITIONAL_REGISTERS

 lw t0, pxCurrentTCB
 sw sp, 0( t0 )

 csrr a1, mepc
 sw a1, 0( sp )

 addi a1, x0, 0x20
 csrs 0x804, a1

 lw sp, xISRStackTop
 jal vTaskSwitchContext

processed_source:
 lw t1, pxCurrentTCB
 lw sp, 0( t1 )


 lw t0, 0( sp )
 csrw mepc, t0

 portasmRESTORE_ADDITIONAL_REGISTERS


 lw t0, 29 * 4( sp )
 csrw mstatus, t0

 lw x1, 1 * 4( sp )
 lw x5, 2 * 4( sp )
 lw x6, 3 * 4( sp )
 lw x7, 4 * 4( sp )
 lw x8, 5 * 4( sp )
 lw x9, 6 * 4( sp )
 lw x10, 7 * 4( sp )
 lw x11, 8 * 4( sp )
 lw x12, 9 * 4( sp )
 lw x13, 10 * 4( sp )
 lw x14, 11 * 4( sp )
 lw x15, 12 * 4( sp )
 lw x16, 13 * 4( sp )
 lw x17, 14 * 4( sp )
 lw x18, 15 * 4( sp )
 lw x19, 16 * 4( sp )
 lw x20, 17 * 4( sp )
 lw x21, 18 * 4( sp )
 lw x22, 19 * 4( sp )
 lw x23, 20 * 4( sp )
 lw x24, 21 * 4( sp )
 lw x25, 22 * 4( sp )
 lw x26, 23 * 4( sp )
 lw x27, 24 * 4( sp )
 lw x28, 25 * 4( sp )
 lw x29, 26 * 4( sp )
 lw x30, 27 * 4( sp )
 lw x31, 28 * 4( sp )
 addi sp, sp, ( 30 * 4 )

 mret
 .endfunc


.align 8
.func
xPortStartFirstTask:
# 224 "/home/klolefir/projects/ch32/v307/snippets/freertos/rtos_src/portable/GCC/RISC-V/portASM.S"
 lw t0, xISRStackTop
 addi t0, t0, -512
    csrw mscratch,t0

 lw sp, pxCurrentTCB
 lw sp, 0( sp )

 lw x1, 0( sp )

 portasmRESTORE_ADDITIONAL_REGISTERS

 lw x6, 3 * 4( sp )
 lw x7, 4 * 4( sp )
 lw x8, 5 * 4( sp )
 lw x9, 6 * 4( sp )
 lw x10, 7 * 4( sp )
 lw x11, 8 * 4( sp )
 lw x12, 9 * 4( sp )
 lw x13, 10 * 4( sp )
 lw x14, 11 * 4( sp )
 lw x15, 12 * 4( sp )
 lw x16, 13 * 4( sp )
 lw x17, 14 * 4( sp )
 lw x18, 15 * 4( sp )
 lw x19, 16 * 4( sp )
 lw x20, 17 * 4( sp )
 lw x21, 18 * 4( sp )
 lw x22, 19 * 4( sp )
 lw x23, 20 * 4( sp )
 lw x24, 21 * 4( sp )
 lw x25, 22 * 4( sp )
 lw x26, 23 * 4( sp )
 lw x27, 24 * 4( sp )
 lw x28, 25 * 4( sp )
 lw x29, 26 * 4( sp )
 lw x30, 27 * 4( sp )
 lw x31, 28 * 4( sp )

 lw x5, 29 * 4( sp )
 addi x5, x5, 0x08
 csrrw x0, mstatus, x5
 lw x5, 2 * 4( sp )

 addi sp, sp, ( 30 * 4 )
 ret
 .endfunc
# 334 "/home/klolefir/projects/ch32/v307/snippets/freertos/rtos_src/portable/GCC/RISC-V/portASM.S"
.align 8
.func
pxPortInitialiseStack:

 csrr t0, mstatus
 andi t0, t0, ~0x8
 addi t1, x0, 0x788
 slli t1, t1, 4
 or t0, t0, t1

 addi a0, a0, -4
 sw t0, 0(a0)
 addi a0, a0, -(22 * 4)
 sw a2, 0(a0)
 addi a0, a0, -(6 * 4)
 sw x0, 0(a0)
 addi t0, x0, 0
chip_specific_stack_frame:
 beq t0, x0, 1f
 addi a0, a0, -4
 sw x0, 0(a0)
 addi t0, t0, -1
 j chip_specific_stack_frame
1:
 addi a0, a0, -4
 sw a1, 0(a0)
 ret
 .endfunc
