/*******************************************************************************
* File Name: timer_aj_stm32f4.c
*
* Description:
* The file contains function definition for Timer peripheral of STM32f407.
*
* Related Document: See README.md
*
*******************************************************************************/
#include "timer_aj_stm32f4.h"

/*******************************************************************************
* Function Name: general_timer_config()
********************************************************************************
* Summary:
*   Configures the TIMx peripheral according to the specified config parameters.
*
* Parameters:
*   TIMx:           Pointer to TIMx Base address.
*   tim_config:     Pointer to struct having timer configurations.
*
* Return :
*   void
*
* NOTE: This function can only perform the following:
*    - Config timer in up/down counting mode.
*    - For other functionality, refer to other functions (may be TO DO).
*
*******************************************************************************/
void general_timer_config(TIM_TypeDef *TIMx, general_timer_configs_t *tim_config)
{
    /* Enable system's RCC peripheral clock for TIMx */
    if(TIMx == TIM2 | TIMx == TIM3 | TIMx == TIM4 | TIMx == TIM5 | TIMx == TIM6
      | TIMx == TIM7 | TIMx == TIM12 | TIMx == TIM13 | TIMx == TIM14)
    {
        RCC->APB1ENR |= (uint32_t)(1 << (uint32_t)((TIMx - TIM2)/(TIM3 - TIM2)));
    }

    else if(TIMx == TIM1 | TIMx == TIM8)
    {
        RCC->APB2ENR |= (uint32_t)(1 << (uint32_t)(TIMx - TIM1)/(TIM8 - TIM1));
    }

    else if(TIMx == TIM9 | TIMx == TIM10 | TIMx == TIM11)
    {
        RCC->APB2ENR |= (uint32_t)(1 << (uint32_t)((TIMx - TIM9)/(TIM10 - TIM9)
        + RCC_APB2ENR_TIM9EN_Pos));
    }

    /* Set timer counting direction. */
    TIMx->CR1 |= (uint32_t)((TIMx->CR1 & (uint32_t)(~(TIM_CR1_DIR_Msk))) |
                (uint32_t)((uint32_t)(tim_config->count_direction) << TIM_CR1_DIR_Pos));

    /* Set the pre-scaler value for generation of timer counter clock. */
    TIMx->PSC = tim_config->prescaler;

    /* This function disables the auto-reload preload, if needed it needs
     * to be enabled explicitly outside this function.
     */
    TIMx->CR1 |= (uint32_t)((TIMx->CR1 & (uint32_t)(~(TIM_CR1_ARPE_Msk))) |
                (uint32_t)(0U << TIM_CR1_ARPE_Pos))  ;

    /* Set the Auto reload value into ARR */
    TIMx->ARR = tim_config->period;
}

/*******************************************************************************
* Function Name: timer_init()
********************************************************************************
* Summary:
*   Enables/starts the TIMx by enabling it's config register 1.
*
* Parameters:
*   TIMx:    Pointer to TIMx Base address.
*
* Return :
*   void
*
*******************************************************************************/
void timer_init(TIM_TypeDef *TIMx)
{
    /* Enable Counter */
    TIMx->CR1 |= (uint32_t)(1 << TIM_CR1_CEN_Pos);
}
