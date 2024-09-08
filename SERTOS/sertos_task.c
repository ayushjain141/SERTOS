#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "sertos_task.h"
#include "rcc_aj_stm32f4.h"


/*******************************************************************************
 * Macros
 *******************************************************************************/
#define SYSTICK_INTERRUPT_CTRL              (1U)
/* For RTOS scheduling time slice of 50ms, and system clock at 100 MHz using PLL
 * following is the systick reload value. */
#define SYSTICK_LOAD_VALUE                  (5000000U)
#define SYSTICK_IRQ_PRIORITY                (3U)


/*******************************************************************************
* Function Name: sertos_config_os
********************************************************************************
* Summary:
* Configures various HW/SW components which will be used by the OS kernel.
*
* Parameters:
* Void
*
* Return :
* void
*
*******************************************************************************/
void sertos_config_os()
{
    /* Configure and Initialize Systick timer for SERTOS context switch. */
    systick_config_init(SYSTICK_INTERRUPT_CTRL, SYSTICK_LOAD_VALUE);

    /* Set priority for Systick IRQ, using CMSIS function */
    NVIC_SetPriority(SysTick_IRQn, SYSTICK_IRQ_PRIORITY);

    /* Enable Systick IRQ, using CMSIS function */
    NVIC_EnableIRQ(SysTick_IRQn);

    /* Global Interrupts will be eabled by the sertos_start_os() fuction.
     * by using the CMSIS function __enable_irq(). */
}
