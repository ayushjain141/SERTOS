;File Name: asm_test.s
;
;Description:
;The file contains a test assembly code which has several instructions and directives
;used in it. The code has few instructions for purpose of testing only.
;Instructions related to function calls and stack operations are also
;tested in the example.
;
;The semicolon is used to lead an inline documentation.
;Function calls also implemented, see description at end of file. References
;also present at the end of file.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Directives
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        ; The current file preserves eight-byte alignment of the stack.
        ; The linker checks that any code that requires eight-byte alignment of the
        ; stack is only called, directly or indirectly, by code that preserves
        ; eight-byte alignment of the stack.
        PRESERVE8
        ; The THUMB directive instructs the assembler to interpret subsequent
        ; instructions as T32 instructions.
        THUMB

        ; Defines the code (text) area, this contains instructions to be executed
        ; by the CPU.
        AREA    |.text|, CODE, READONLY, ALIGN=2
        ; Specify variable/token/functions as externally declared/defined for the linker
        EXTERN  curr_thread_tcb_p
        ; Specify variable/token/functions as externally declared/defined for the linker
        EXPORT  SysTick_Handler
        EXPORT  sertos_start_os


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function Name: SysTick_Handler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Summary:
; ISR handler for the Systick timer of ARM cortex M4 CPU.
; Context switching will take place in this ISR.
;
; To Do:
; Implement the context switching using the PendSV(SW interrupt) so that the
; systick can be reserved for timer related functions.
;
; Parameters:
;
; Return :
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SysTick_Handler

        ; Prevent interrupt during SystickISR Execution.
        CPSID   I

        ; push CPU reg {r4 - r11} to current TCB stack.
        PUSH {r4 - r11}

        ; r0 = &curr_thread_tcb_p
        ;
        ; This instruction uses Literal Pool to fetch "address" of curr_thread_tcb_p.
        ; The "value" of curr_thread_tcb_p is the address of the current TCB struct
        ; and thus holds address of member "thread_sp_p" which is a pointer to the
        ; TCB's stack.
        LDR r0, =curr_thread_tcb_p

        ; r1 = curr_thread_tcb_p , it also implies r1 = &thread_sp_p
        ; curr_thread_tcb_p is pointer to current TCB struct and thus pointer to
        ; it's first member "thread_sp_p".
        LDR r1, [r0]

        ; CPU reg SP "value" is copied to Current TCBs "thread_sp_p".
        ;
        ; Currently the CPU reg SP "value" will be some "address" in the range
        ; of current task TCB's stack array.
        STR sp, [r1]

        ; r1 = "value" of next_thread_tcb_p
        LDR r1, [r1, #4]

        ; (value at mem address pointed by r0) = "value" of next_thread_tcb_p,
        ;
        ; this instruction copies value in r1 ("value" of next_thread_tcb_p) into
        ; (value at address stored in r0). This will imply -
        ; curr_thread_tcb_p = curr_thread_tcb_p -> next_thread_tcb_p;
        STR r1, [r0]

        ; Context has been switched to the next task about to resume, this task
        ; will be referred to as "switched task" till the context switch completes.
        ; After the ISR completes, the CPU will start executing this switched task.

        ; CPU reg sp = address of the stack of TCB of switched task.
        LDR sp, [r1]

        ; POP registers of current TCB stack (uses TCB's stack pointer "thread_sp_p")
        ; into CPU reg {r4 - r11}.
        POP {r4 - r11}

        ; Enable the global interrupts to allow Systick IRQ to be triggered at
        ; occurance of Systick event.
        CPSIE   I

        ; CPU returns to the program execution using internal exception return
        ; mechanism involving the `Link register (LR)`.
        BX LR


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function Name: sertos_start_os
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Summary:
; Launches the OS by starting the first task using a branch instruction, enables
; the global interrupts so the systick IRQ can be handled. This function makes
; it look like the context of the first task has already been pushed into it's
; TCB's stack and now we are just restoring it to CPU registers, just like the
; context switch function. Initially before the OS launch, all the TCB's stacks
; will have dummy data.
;
; Parameters:
;
; Return :
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
sertos_start_os
        ; Pointer to First thread's TCB. Also this will point to the first task
        ; TCB struct's first member ie: the stack pointer to TCB's stack.
        LDR r0 , =curr_thread_tcb_p

        ; Load the value at address (r0 value) into r1. Done to achieve (r1 = sp).
        LDR r1, [r0]

        ; Load the value at address (r1 value) into sp.
        ;
        ; Done to achieve (sp = &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 16]).
        LDR sp, [r1]

        ; POP the "r4-r11" registers.
        ;
        ; This is done to move the stack pointer to
        ; &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 8], after this instruction
        ; sp = &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 8]
        POP {r4 - r11}

        ; POP the "r0-r3" registers.
        ;
        ; This is done to move the stack pointer to
        ; &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 4], after this instruction
        ; sp = &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 4]
        POP {r0 - r3}

        ; POP the r12 register.
        ;
        ; This is done to move the stack pointer to
        ; &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 3], after this instruction
        ; sp = &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 3]
        POP {r12}

        ; POP the LR (R14) register.
        ;
        ; This is done to move the stack pointer to
        ; &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 2], after this instruction
        ; sp = &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 2]
        POP {LR}

        ; POP the Return address (PC) from TCB stack into the LR. This is done to
        ; load the function pointer to first task into the LR so that we can
        ; switch to first task at the end of this function.
        ;
        ; This moves the stack pointer to
        ; &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 1], after this instruction
        ; sp = &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 1]
        POP {LR}

        ; POP the xPSR value from TCB stack to r0, this value is not to be
        ; used/modified by this function, and contains thumb bit information.
        ; This instruction is only used to do a dummy read of the xPSR from the
        ; TCB stack so that the stack pointer moves to
        ; &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 0], after this instruction
        ; sp = &sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 0]
        POP {r0}

        ; Enable the global interrupts to allow Systick IRQ to be triggered at
        ; occurance of Systick event.
        CPSIE I

        ; Branch to the address pointed by LR. This will be "function pointer to
        ; First task", and hence the first task starts.
        BX LR

        ALIGN
        END


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Below code is for testing purpose only, to be removed later.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Directives
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;       ; the current file preserves eight-byte alignment of the stack.
;       ; The linker checks that any code that requires eight-byte alignment of the
;       ; stack is only called, directly or indirectly, by code that preserves
;       ; eight-byte alignment of the stack.
;       PRESERVE8
;       ; The THUMB directive instructs the assembler to interpret subsequent
;       ; instructions as T32 instructions.
;       THUMB
;
;       ; Defines the code (text) area, this contains instructions to be executed
;       ; the CPU.
;       AREA    |.text|, CODE, READONLY, ALIGN=2
;       ; Specify variable/token as externally declared/defined for the linker
;       EXTERN  test_arr_p
;       ; Specify variable/token as externally declared/defined for the linker
;       EXTERN  test_arr_p2
;       ; Specify variable/token as externally declared/defined for the linker
;       EXPORT  SysTick_Handler

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;* Systick ISR for testing ARM instruction set stack operations between CPU registers
;bank and Array in data segment of program.*;
;SysTick_Handler
;       ; Prevent interrupt during SystickISR Execution.
;       CPSID   I
;       MOV r5, sp
;       MOV r0, #0x77
;       MOV r1, #0x88
;       MOV r2, #0x99
;       LDR r4, =test_arr_p
;       LDR SP, [r4]
;       PUSH {r0-r2}
;       LDR r4, =test_arr_p2
;       LDR SP, [r4]
;       POP {r0-r2}
;       MOV r4, #0x44
;       MOV sp, r5
;       ;Tasks run with interrupts enabled.
;       CPSIE   I
;       BX LR


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Refererences
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;(1). The code reference is taken from 
;   [University of Regina, CS-301 course ](https://www.labs.cs.uregina.ca/301/ARM-subroutine/lecture.php).
;   The main site for above is - https://www.labs.cs.uregina.ca/301/index.php
;
;(2). [Assembly language PPT from book jonathon Volvano Book web, lectures tab, lec:4,5](https://users.ece.utexas.edu/~valvano/Volume1/)

; End of File ;
