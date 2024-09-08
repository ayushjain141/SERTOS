#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "sertos_task.h"
#include "bsp_aj_stm32f4.h"


/*******************************************************************************
 * Macros
 *******************************************************************************/
#define USER_LED1_PORT                          (GPIOA)
#define USER_LED1_PIN                           (5)
#define USER_LED2_PORT                          (GPIOA)
#define USER_LED2_PIN                           (6)
#define USER_LED3_PORT                          (GPIOA)
#define USER_LED3_PIN                           (7)


/* Arrays for testing ARM instruction set stack operations between CPU registers
bank and Array in data segment of program.
int32_t test_arr[] = {0x00, 0x00, 0x00};
int32_t *test_arr_p = (&test_arr[2] + 1);

int32_t test_arr2[] = {0x11, 0x22, 0x33};
int32_t *test_arr_p2 = &test_arr2[0];
*/


/*******************************************************************************
 * Global Variables
 *******************************************************************************/
/* SERTOS TCB of task_a */
static sertos_tcb_st_t task_a_tcb_st;
/* SERTOS TCB of task_b */
static sertos_tcb_st_t task_b_tcb_st;
/* SERTOS TCB of task_c */
static sertos_tcb_st_t task_c_tcb_st;

/* Pointer to current thread's TCB */
sertos_tcb_st_t *curr_thread_tcb_p;


/*******************************************************************************
* Function Name: task_a
********************************************************************************
* Summary:
* This task increments a counter variable `counter_a` till a limit and resets
* it. This function contains a non-terminating loop.
*
* Parameters:
* Void
*
* Return:
* void
*
*******************************************************************************/
void task_a(void)
{
    int count_a = 0;

    while(1)
    {
        if(count_a > INT32_MAX)
        {
            count_a = 0;
        }
        else
        {
            count_a++;
        }

        /* Turn ON LED-1 and Turn OFF LED-2 and LED-3. */
        SET_PIN(USER_LED1_PORT, USER_LED1_PIN);
        RESET_PIN(USER_LED2_PORT, USER_LED2_PIN);
        RESET_PIN(USER_LED3_PORT, USER_LED3_PIN);
    }
}


/*******************************************************************************
* Function Name: task_b
********************************************************************************
* Summary:
* This task increments a counter variable `counter_b` till a limit and resets
* it. This function contains a non-terminating loop.
*
* Parameters:
* Void
*
* Return :
* void
*
*******************************************************************************/
void task_b(void)
{
    int count_b = 0;

    while(1)
    {
        if(count_b > INT32_MAX)
        {
            count_b = 0;
        }
        else
        {
            count_b++;
        }
 
        /* Turn ON LED-2 and Turn OFF LED-3 and LED-1. */
        SET_PIN(USER_LED2_PORT, USER_LED2_PIN);
        RESET_PIN(USER_LED1_PORT, USER_LED1_PIN);
        RESET_PIN(USER_LED3_PORT, USER_LED3_PIN);
    }
}


/*******************************************************************************
* Function Name: task_c
********************************************************************************
* Summary:
* This task increments a counter variable `counter_c` till a limit and resets
* it. This function contains a non-terminating loop.
*
* Parameters:
* Void
*
* Return :
* void
*
*******************************************************************************/
void task_c(void)
{
    int count_c = 0;

    while(1)
    {
        if(count_c > INT32_MAX)
        {
            count_c = 0;
        }
        else
        {
            count_c++;
        }

        /* Turn ON LED-3 and Turn OFF LED-1 and LED-2. */
        SET_PIN(USER_LED3_PORT, USER_LED3_PIN);
        RESET_PIN(USER_LED1_PORT, USER_LED1_PIN);
        RESET_PIN(USER_LED2_PORT, USER_LED2_PIN);
    }
}


/*******************************************************************************
* Function Name: sertos_stack_init
********************************************************************************
* Summary:
* Initializes the task stack.
*
* To Do: currently uses hard-coded values for TCB stacks for task_a, task_b,
* task_c. Make this function versatile and modular.
*
* Parameters:
* Void
*
* Return :
* void
*
*******************************************************************************/
void sertos_stack_init()
{
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 1] = 0x01000000; // xPSR reg values, thumb bit set to '1'
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 2] = (int32_t)(&task_a); // Return address (PC)
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 3] = 0x10014; // Link register (R14)
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 4] = 0x10012; // R12
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 5] = 0x10003; // R3
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 6] = 0x10002; // R2
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 7] = 0x10001; // R1
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 8] = 0x10000; // R0
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 9] = 0x10011; // R11
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 10] = 0x10010; // R10
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 11] = 0x10009; // R9
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 12] = 0x10008; // R8
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 13] = 0x10007; // R7
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 14] = 0x10006; // R6
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 15] = 0x10005; // R5
    task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 16] = 0x10004; // R4

    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 1] = 0x01000000; // xPSR reg values
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 2] = (int32_t)(&task_b); // Return address (PC)
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 3] = 0x20014; // Link register (R14)
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 4] = 0x20012; // R12
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 5] = 0x20003; // R3
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 6] = 0x20002; // R2
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 7] = 0x20001; // R1
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 8] = 0x20000; // R0
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 9] = 0x20011; // R11
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 10] = 0x20010; // R10
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 11] = 0x20009; // R9
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 12] = 0x20008; // R8
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 13] = 0x20007; // R7
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 14] = 0x20006; // R6
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 15] = 0x20005; // R5
    task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 16] = 0x20004; // R4

    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 1] = 0x01000000; // xPSR reg values
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 2] = (int32_t)(&task_c); // Return address (PC)
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 3] = 0x30014; // Link register (R14)
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 4] = 0x30012; // R12
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 5] = 0x30003; // R3
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 6] = 0x30002; // R2
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 7] = 0x30001; // R1
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 8] = 0x30000; // R0
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 9] = 0x30011; // R11
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 10] = 0x30010; // R10
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 11] = 0x30009; // R9
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 12] = 0x30008; // R8
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 13] = 0x30007; // R7
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 14] = 0x30006; // R6
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 15] = 0x30005; // R5
    task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 16] = 0x30004; // R4
}


/*******************************************************************************
* Function Name: sertos_create_threads
********************************************************************************
* Summary:
* Initializes the stack pointers of TCBs and creates the link list of TCBs for
* scheduling.
*
* To Do: currently uses hard-coded values for TCB stacks for task_a, task_b,
* task_c. Make this function versatile and modular.
*
* Parameters:
* Void
*
* Return :
* void
*
*******************************************************************************/
void sertos_create_threads()
{
    /* Initialize stacks for the threads. */
    sertos_stack_init();

    /* Initialize the stack pointer of the task_a thread's TCB. */
    task_a_tcb_st.thread_sp_p = &task_a_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 16];

    /* Initialize the pointer to the TCB of next thread of task_a, this will eventually
       form a circular linked list of TCBs. */
    task_a_tcb_st.next_thread_tcb_p = &task_b_tcb_st;

    /* Initialize the stack pointer of the task_b thread's TCB. */
    task_b_tcb_st.thread_sp_p = &task_b_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 16];

    /* Initialize the pointer to the TCB of next thread of task_b, this will eventually
       form a circular linked list of TCBs. */
    task_b_tcb_st.next_thread_tcb_p = &task_c_tcb_st;

    /* Initialize the stack pointer of the task_c thread's TCB. */
    task_c_tcb_st.thread_sp_p = &task_c_tcb_st.sertos_tcb_stack[SERTOS_TCB_STACK_SIZE - 16];

    /* Initialize the pointer to the TCB of next thread of task_c, this will eventually
       form a circular linked list of TCBs. */
    task_c_tcb_st.next_thread_tcb_p = &task_a_tcb_st;
}


int main()
{
  /* Initialize the BSP. */
  stm32f4_bsp_init();

  /* Config GPIO for LED1. */
  gpio_output_config(USER_LED1_PORT, USER_LED1_PIN, gpio_otyper_push_pull, gpio_ospeedr_high,
                     gpio_pupdr_float);

  /* Config GPIO for LED2. */
  gpio_output_config(USER_LED2_PORT, USER_LED2_PIN, gpio_otyper_push_pull, gpio_ospeedr_high,
                     gpio_pupdr_float);

  /* Config GPIO for LED3. */
  gpio_output_config(USER_LED3_PORT, USER_LED3_PIN, gpio_otyper_push_pull, gpio_ospeedr_high,
                     gpio_pupdr_float);

  /* Create threads, involves stack initialization and TCB list creation for schedulig. */
  sertos_create_threads();

  /* Initialize the current thread pointer to first thread. */
  curr_thread_tcb_p = &task_a_tcb_st;

  /* Configure the OS components. */
  sertos_config_os();

  /* Start the OS components, this will launch the OS and the tasks/threads will
   * start executing. */
  sertos_start_os();

  while(1);
}

/* End of File */
