/*******************************************************************************
* File Name: timer_aj_stm32f4.h
*
* Description:
* The file contains function declaration for Timer peripheral of STM32f407.
*
* Related Document: See README.md
*
*******************************************************************************/
#ifndef TIMER_AJ_STM32F4
#define TIMER_AJ_STM32F4

#include "stm32f4xx.h"
#include <stdbool.h>

#ifdef  __STM32F407xx_H
#include "stm32f407xx.h"
#endif  /* __STM32F407xx_H */

/*******************************************************************************
* Global Variables
*******************************************************************************/
typedef struct general_timer_configs_t{
    bool count_direction;
    bool auto_reload_preload_enable;
    uint8_t repetition_counter;
    uint16_t prescaler;
    uint16_t clock_division;
    uint32_t period;
}general_timer_configs_t;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void general_timer_config(TIM_TypeDef *TIMx, general_timer_configs_t *tim_config);
void timer_init(TIM_TypeDef *TIMx);

#endif  /* GPIO_AJ_STM32F4 */
