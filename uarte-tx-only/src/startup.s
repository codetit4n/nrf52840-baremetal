    .syntax unified
    .cpu cortex-m4
    .thumb

    .extern main

    /* Symbols provided by linker.ld */
    .extern _estack
    .extern _sidata
    .extern _sdata
    .extern _edata
    .extern _sbss
    .extern _ebss

    .global Reset_Handler
    .global Default_Handler

/* ============================================================
 * Vector Table
 * ============================================================ */

    .section .isr_vector, "a", %progbits

    .word _estack
    .word Reset_Handler
    .word Default_Handler      /* NMI */
    .word Default_Handler      /* HardFault */
    .word Default_Handler      /* MemManage */
    .word Default_Handler      /* BusFault */
    .word Default_Handler      /* UsageFault */
    .word 0
    .word 0
    .word 0
    .word 0
    .word Default_Handler      /* SVCall */
    .word Default_Handler      /* DebugMonitor */
    .word 0
    .word Default_Handler      /* PendSV */
    .word Default_Handler      /* SysTick */

    /* nRF52840 has 48 external interrupt vectors */
    .rept 48
        .word Default_Handler
    .endr


/* ============================================================
 * Reset Handler
 * ============================================================ */

    .section .text.Reset_Handler, "ax", %progbits
    .align 2
    .thumb_func
Reset_Handler:

    /* Copy initialized data from FLASH to RAM */

    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

copy_data:
    cmp r1, r2
    bcs zero_bss

    ldr r3, [r0], #4
    str r3, [r1], #4
    b copy_data


zero_bss:

    ldr r0, =_sbss
    ldr r1, =_ebss

clear_loop:
    cmp r0, r1
    bcs call_main

    movs r2, #0
    str r2, [r0], #4
    b clear_loop


call_main:

    bl main

hang:
    b hang


/* ============================================================
 * Default Interrupt Handler
 * ============================================================ */

    .section .text.Default_Handler, "ax", %progbits
    .align 2
    .thumb_func
Default_Handler:
default_loop:
    b default_loop
