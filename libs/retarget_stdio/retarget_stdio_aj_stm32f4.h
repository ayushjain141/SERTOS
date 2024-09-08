/*******************************************************************************
 * File Name: retarget_stdio_aj_stm32f4.h
 *
 * Description:
 *   The file contains declarations for stdio re-direction.
 *
 * Related Document: See README.md
 *
 ******************************************************************************/
#ifndef RETARGET_STDIO_AJ_STM32F4
#define RETARGET_STDIO_AJ_STM32F4

#include <stdio.h>
#include "stm32f4xx.h"

#ifdef __STM32F407xx_H
#include "stm32f407xx.h"
#endif 

#include "usart_aj_stm32f4.h"
#include "bsp_aj_stm32f4.h"
#include "utils_aj_stm32f4.h"


/*******************************************************************************
 * Macros
 ******************************************************************************/


/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
result_funct printf_retarget_uart_init(void);

#endif   /* RETARGET_STDIO_AJ_STM32F4 */   /* End of File */
