#include "riscv-virt.h"
.align 4
.equ UART_REG_TXFIFO,   0
.equ UART_BASE,         0x10000000

.global uart_puts
.globl _boot
_boot:
    li t6, 0x1800               
    csrw mstatus, t6
    j _mstart

.align 6
.section .text,"ax",@progbits
.globl _mstart

/* Startup code */
_mstart:
    csrr  t0, mhartid             # read hardware thread id (`hart` stands for `hardware thread`)
    bnez  t0, halt                # run only on the first hardware thread (hartid == 0), halt all the other threads
trap_vec_init:
    la    t0, freertos_vector_table  # mtvec BASE address
    ori   t0,t0, 1                     # mtvec MODE (0: jump BASE when trap, 1: jump BASE when exception
                                     # jump BACE+4*cause when interrupt)
    csrw  mtvec, t0

plic_init:
    ### Initialize PLIC ###

    li t6, 0xc000000            # interrupt_96 priority 5
    addi t6,t6,0x180
    li t5,0x5
    sw t5,0(t6)

    li t6 , 0xc00200c            # interrupt_96 enable on context_0
    li t5 , 0x01
    sw t5 , 0(t6)

    li t6, 0xc200000            # context_0 interrupt threshold 2
    li t5,0x02
    sw t5,0(t6)

    ##  enable global external interrupt ###

reg_init:
    li	x1, 0
    li	x2, 0
    li	x3, 0
    li	x4, 0
    li	x5, 0
	li	x6, 0
    li	x7, 0
    li	x8, 0
    li	x9, 0
    li	x10, 0
    li	x11, 0
    li	x12, 0
    li	x13, 0
    li	x14, 0
    li	x15, 0
    li	x16, 0
    li	x17, 0
    li	x18, 0
    li	x19, 0
    li	x20, 0
    li	x21, 0
    li	x22, 0
    li	x23, 0
    li	x24, 0
    li	x25, 0
    li	x26, 0
    li	x27, 0
    li	x28, 0
    li	x29, 0
    li	x30, 0
    li	x31, 0

    la	gp, _gp
# Initialize UART
init_uart:
    li    t1, UART_BASE            # t1 = UART_BASE
    li    t2, 0x800                # t2 = 0x800 (Enable TX and RX)
    sw    t2, 0(t1)                # uart[0] = t2

init_bss:
    /* init bss section */
    la	a0, __bss_start
    la	a1, __bss_end-4 /* section end is actually the start of the next section */
    li	a2, 0x0
    jal	  ra, fill_block

init_sbss:
    /* init bss section */
    la	a0, __sbss_start
    la	a1, __sbss_end-4 /* section end is actually the start of the next section */
    li	a2, 0x0
    jal	  ra, fill_block

write_stack_pattern:
    /* init bss section */
    la	a0, _stack_end  /* note the stack grows from top to bottom */
    la	a1, __stack-4   /* section end is actually the start of the next section */
    li	a2, 0xABABABAB
    jal	  ra, fill_block

init_stack:
    /* set stack pointer */
    la	sp, _stack

main_entry:
    la    a0, hello_msg                 # load address of `msg` to a0 argument register
    jal   ra, uart_puts                    # jump to `puts` subroutine, return address is stored in ra regster
	# jump to main program entry point (argc = argv = 0) 
  	li  a0, 0
	li  a1, 0
	li  a2, 0
  	jal ra, main

    la    a0, msg                 # load address of `msg` to a0 argument register
    jal ra, uart_puts                    # jump to `puts` subroutine, return address is stored in ra regster

halt:   
    j     halt                     # enter the infinite loop

uart_puts:                                 
    # `puts` subroutine writes null-terminated string to UART (serial communication port)
    # input: a0 register specifies the starting address of a null-terminated string
    # clobbers: t0, t1, t2 temporary registers

    li    t0, UART_BASE           # t0 = UART_BASE
uart_print:      
    lbu   t1, (a0)       # t1 = load unsigned byte from memory address specified by a0 register
    beqz  t1, uart_end   # break the loop, if loaded byte was null

    # wait until UART is ready
positive:      
    lw    t2, UART_REG_TXFIFO(t0) # t2 = uart[UART_REG_TXFIFO]
    bltz  t2, positive            # t2 becomes positive once UART is ready for transmission
    sw    t1, UART_REG_TXFIFO(t0) # send byte, uart[UART_REG_TXFIFO] = t1

    addi  a0, a0, 1               # increment a0 address by 1 byte
    j     uart_print

uart_end:      
    ret

/* Fills memory blocks */
fill_block:
    sw		a2, 0(a0)
    bgeu	a0, a1, fb_end
    addi	a0, a0, 4
    j		fill_block
fb_end:
    ret

.section .rodata
hello_msg:
    .string "\n\n FREERTOS boot (pressing `ctrl+a x` to quit)\n\n"
msg:
    .string "Hello! The code runs to the end. (pressing `ctrl+a x` to quit)\n"

