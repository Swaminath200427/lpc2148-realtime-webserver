.syntax unified
.arch armv4t
.cpu arm7tdmi-s
.fpu softvfp

@ Mode and Stack definitions
.equ    Mode_USR,   0x10
.equ    Mode_FIQ,   0x11
.equ    Mode_IRQ,   0x12
.equ    Mode_SVC,   0x13
.equ    Mode_ABT,   0x17
.equ    Mode_UND,   0x1B
.equ    Mode_SYS,   0x1F

.equ    I_Bit,      0x80
.equ    F_Bit,      0x40

@ Stack sizes
.equ    USR_Stack_Size,  0x00000400  @ 1KB
.equ    SVC_Stack_Size,  0x00000100  @ 256B
.equ    IRQ_Stack_Size,  0x00000100  @ 256B
.equ    FIQ_Stack_Size,  0x00000100  @ 256B
.equ    ABT_Stack_Size,  0x00000100  @ 256B
.equ    UND_Stack_Size,  0x00000100  @ 256B

.section .stack
.align 3
.global __StackTop
.global __StackLimit
__StackLimit:
    .space  USR_Stack_Size
    .space  SVC_Stack_Size
    .space  IRQ_Stack_Size
    .space  FIQ_Stack_Size
    .space  ABT_Stack_Size
    .space  UND_Stack_Size
__StackTop:

.section .vectors
.global _vectors
_vectors:
    ldr     r0, =Reset_Handler
    ldr     r1, =Undef_Handler
    ldr     r2, =SWI_Handler
    ldr     r3, =PAbt_Handler
    ldr     r4, =DAbt_Handler
    ldr     r5, =IRQ_Handler
    ldr     r6, =FIQ_Handler

    .word   0
    ldr     pc, [r0]  @ Load reset handler address
    ldr     pc, [r1]  @ Load undefined handler address
    ldr     pc, [r2]  @ Load SWI handler address
    ldr     pc, [r3]  @ Load prefetch abort handler address
    ldr     pc, [r4]  @ Load data abort handler address
    ldr     pc, [r5]  @ Load IRQ handler address
    ldr     pc, [r6]  @ Load FIQ handler address

.text
.align  2

.global Reset_Handler
.type   Reset_Handler, %function
Reset_Handler:
    @ Set up stack for each mode
    ldr     r0, =__StackTop
    
    @ Initialize stacks for each mode
    @ Undefined mode
    msr     CPSR_c, #(Mode_UND | I_Bit | F_Bit)
    mov     sp, r0
    sub     r0, r0, #UND_Stack_Size
    
    @ Abort mode
    msr     CPSR_c, #(Mode_ABT | I_Bit | F_Bit)
    mov     sp, r0
    sub     r0, r0, #ABT_Stack_Size
    
    @ FIQ mode
    msr     CPSR_c, #(Mode_FIQ | I_Bit | F_Bit)
    mov     sp, r0
    sub     r0, r0, #FIQ_Stack_Size
    
    @ IRQ mode
    msr     CPSR_c, #(Mode_IRQ | I_Bit | F_Bit)
    mov     sp, r0
    sub     r0, r0, #IRQ_Stack_Size
    
    @ Supervisor mode
    msr     CPSR_c, #(Mode_SVC | I_Bit | F_Bit)
    mov     sp, r0
    sub     r0, r0, #SVC_Stack_Size
    
    @ User/System mode
    msr     CPSR_c, #(Mode_SYS | F_Bit)
    mov     sp, r0

    @ Initialize memory
    bl      memory_init
    
    @ Branch to main
    bl      main
    
    @ Loop if main returns
    b       .

memory_init:
    @ Initialize .data section
    ldr     r0, =_data_start
    ldr     r1, =_data_end
    ldr     r2, =_data_load
1:
    cmp     r0, r1
    beq     2f
    ldr     r3, [r2], #4
    str     r3, [r0], #4
    b       1b
    
    @ Initialize .bss section
2:
    ldr     r0, =_bss_start
    ldr     r1, =_bss_end
    mov     r2, #0
3:
    cmp     r0, r1
    beq     4f
    str     r2, [r0], #4
    b       3b
4:
    bx      lr

@ Default exception handlers
.weak   Undef_Handler
.weak   SWI_Handler
.weak   PAbt_Handler
.weak   DAbt_Handler
.weak   IRQ_Handler
.weak   FIQ_Handler

Undef_Handler:
    b       .
SWI_Handler:
    b       .
PAbt_Handler:
    b       .
DAbt_Handler:
    b       .
IRQ_Handler:
    b       .
FIQ_Handler:
    b       .

.end
