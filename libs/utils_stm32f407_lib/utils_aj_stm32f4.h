/*******************************************************************************
* File Name: utils_aj_stm32f4.h
*
* Description:
* The file contains function declaration(s) for utility frintions for the
* STM32f407 libs.
*
* Related Document: See README.md
*
*******************************************************************************/
#ifndef UTILS_AJ_STM32F4
#define UTILS_AJ_STM32F4

/*******************************************************************************
* Header Files
*******************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx.h"
#include <stdbool.h>

#ifdef  __STM32F407xx_H
#include "stm32f407xx.h"
#endif  /* __STM32F407xx_H */

/*******************************************************************************
* Macros
*******************************************************************************/
#if 0
#define ST_ASSERT(x)        do{\
                                ((x) ? (void)(0U) : __asm("BKPT #0"));\
                            }while(0)
#endif

/*******************************************************************************
 * Global Variables
 *******************************************************************************/
typedef enum result_funct_e
{
    RESULT_FUNCT_STATUS_SUCCESS,
    RESULT_FUNCT_STATUS_FAIL,
    RESULT_FUNCT_STATUS_BAD_PARAM,
} result_funct_e_t;

typedef int result_funct;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/


#endif /* UTILS_AJ_STM32F4 */
