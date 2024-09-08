/******************************************************************************
 * File Name: delay_aj_stm32f4.c
 *
 * Description:
 *  The file contains implementation for delay related function.
 *
 * Related Document: See README.md
 *
 *******************************************************************************/
#include "delay_aj_stm32f4.h"

/*******************************************************************************
 * Function Name: delay_init()
 ********************************************************************************
 * Summary:
 *  Initializes the timer instance for the delay routines.
 *
 * Parameters:
 *  TIMx:    Timer instance.
 *
 * Return :
 *  void
 *
 *******************************************************************************/
void delay_init(TIM_TypeDef *TIMx)
{
    /*
     * Configs for the TIMx to be enabled for delay function, this function
     * assumes that the system clock is at 100 MHz.
     */
    general_timer_configs_t timx_configs = {
        .count_direction = DELAY_TIM_INST_DIRECTION,
        .auto_reload_preload_enable = 0,
        .repetition_counter = 0,
        /*
         * We want TIMx to run at 100 MHz, system clock will be at 100 MHz(PLL),
         * hence the no prescaler.
         */
        .prescaler = 0,
        .clock_division = 0,
        /*
         * We want the timer to count till ticks corresponding to 1us, at 100 MHz
         * frequency of TIMx, it would mean:
         * 1 sec   -->   100 x 1e6 counter ticks
         * 1 usec  -->   100 ticks
         */
        .period = DELAY_TIM_INST_PERIOD - 1,
    };

    /* Config TIMx for delay operations */
    general_timer_config(TIMx, &timx_configs);

    /* Start the TIMx with previously configured settings */
    timer_init(TIMx);
}

/*******************************************************************************
 * Function Name: delay_us()
 ********************************************************************************
 * Summary:
 *   Function to produce micro-seconds delay.
 *
 * Parameters:
 *  us:      Number of micro-seconds to delay.
 *
 * Return :
 *  void
 *
 *******************************************************************************/
void delay_us(uint32_t us)
{
    uint32_t i;

    for (i = 0; i < us; i++)
    {
        /* Wait for UIF to be set. This will generate 1 ms delay */
        while ((DELAY_TIM_INST->SR & TIM_SR_UIF) == 0)
            ;

        /* Reset UIF */
        DELAY_TIM_INST->SR &= ~TIM_SR_UIF;
    }
}

/*******************************************************************************
 * Function Name: delay_ms()
 ********************************************************************************
 * Summary:
 *   Function to produce milli-seconds delay.
 * 
 *   NOTE: The delay produced by this function has some constant error of approx
 * 	 4%, more analysis is required for improving this accuracy. "TO DO".
 *
 * Parameters:
 *   ms:      Number of milli-seconds to delay.
 *
 * Return :
 *  void
 *
 *******************************************************************************/
void delay_ms(uint32_t ms)
{
    uint32_t i;

    ms *= 1000;

    for (i = 0; i < ms; i++)
    {
        DELAY_TIM_INST->CNT = DELAY_TIM_INST_MILI_CALIB_CNT_VAL;
        delay_us(1);
    }
}
