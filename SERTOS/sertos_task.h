#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include <stdint.h>

/*******************************************************************************
 * Macros
 *******************************************************************************/
#define SERTOS_TCB_STACK_SIZE               (100u)


/*******************************************************************************
 * Global Variables
 *******************************************************************************/
/* Structure for TCB of a thread */
typedef struct sertos_tcb_st
{
    /* Pointer to thread's stack pointer ( Ex: value of SP CPU register in ARM
       cortex-M ) */
    int32_t *thread_sp_p;

    /* Pointer to TCB of next thread */
    struct sertos_tcb_st *next_thread_tcb_p;

    /* SERTOS TCB's Stack(data segment) for the task */
    /* Stacks for SERTOS Tasks -
     * The following stacks should store the context of the tasks in the following
     * manner:
     *
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 1]   // xPSR reg values
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 2]   // Return address (PC)
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 3]   // Link register (R14)
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 4]   // R12
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 5]   // R3
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 6]   // R2
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 7]   // R1
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 8]   // R0
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 9]   // R11
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 10]    // R10
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 11]    // R9
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 12]    // R8
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 13]    // R7
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 14]    // R6
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 15]    // R5
     *  sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 16]    // R4
     *
     * The values of the xPSR, return address (PC), Link register (R14), R12, R0-R3,
     * are stored automatically by the CPU in the "stacking" operations during
     * exception handling, and this stack will be used for the same.
     *
     * Inside the context switch mechanism, this stack is used to save the CPU registers
     * other than those saved by the ISR mechanism. Also, the same stack is used for
     * restoring the context of the next task. */
    int32_t sertos_tcb_stack[SERTOS_TCB_STACK_SIZE];
}sertos_tcb_st_t;

extern sertos_tcb_st_t *curr_thread_tcb_p;

void sertos_create_threads(void);
void sertos_config_os(void);
void sertos_start_os(void);
