/******************************************************************************
 * File Name: delay_aj_stm32f4.h
 *
 * Description:
 *   The file contains declarations for delay related function.
 *
 * Related Document: See README.md
 *
 *******************************************************************************/
#ifndef DELAY_AJ_STM32F4
#define DELAY_AJ_STM32F4

#include "stm32f4xx.h"

#ifdef __STM32F407xx_H
#include "stm32f407xx.h"
#endif

#include "core_cm4.h"
#include "timer_aj_stm32f4.h"

/*******************************************************************************
 * Macros
 *******************************************************************************/
/* TIMx instance reserved for the delay related functions */
#define DELAY_TIM_INST                          (TIM2)
/* Delay timer's count Direction */
#define DELAY_TIM_INST_DIRECTION                (1)
/* Delay Timer period */
#define DELAY_TIM_INST_PERIOD                   (100)
/* Timer period calib value to correct the generated millisecond delay, this
 * value was experimentally determined and, requires some more analysis, for
 * higher accuracy.
 */
#define DELAY_TIM_INST_MILI_CALIB_CNT_VAL       (60)

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/
void delay_init(TIM_TypeDef *TIMx);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif /* End of File */
