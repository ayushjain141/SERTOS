/*******************************************************************************
 * File Name: bsp_aj_stm32f4.h
 *
 * Description:
 *   Contains declarations for hardware initialization of STM32F4 hardware.
 *
 * Related Document: See README.md
 *
 *******************************************************************************/
#ifndef BSP_AJ_STM32F4
#define BSP_AJ_STM32F4

#include <limits.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "utils_aj_stm32f4.h"
#include "rcc_aj_stm32f4.h"
#include "timer_aj_stm32f4.h"
#include "delay_aj_stm32f4.h"
#include "gpio_aj_stm32f4.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
/* This is the value of HSE crystal for clock connected to STM32 MCU */
#define HSE_CLOCK_VAL                       ((uint32_t)8000000)
/* This is the value of HSI clock of STM32 MCU */
#define HSI_CLOCK_VAL                       ((uint32_t)16000000)

/* Defines for printf to UART */
#define PRINTF_UART_COMPATMODE              (USART_COMPATIBLE_MODE_ASYNC)
#define PRINTF_UART_STOPBITS                (USART_STOPBIT_1)
#define PRINTF_UART_TXRXMODE                (USART_TXRX_MODE_RX_TX_BOTH_EN)
#define PRINTF_UART_HWFLOWCTRL              (USART_FLOWCTRL_NONE)
#define PRINTF_UART_INSTANCE                (USART1)
#define PRINTF_UART_BAUDRATE                (115200)
#define PRINTF_UART_WORDLEN                 (USART_WORD_LEN_8_BIT)
#define PRINTF_UART_OVERSAMPLE              (USART_OVERSAMPLE_BY_16)
#define PRINTF_UART_PARITY_EN               (USART_PARITY_DISABLE)
#define PRINTF_UART_PARITY                  (0)
#define PRINTF_UART_TX_GPIO_PORT            (GPIOA)
#define PRINTF_UART_TX_GPIO_PIN             (9)
#define PRINTF_UART_RX_GPIO_PORT            (GPIOA)
#define PRINTF_UART_RX_GPIO_PIN             (10)


/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
void stm32f4_bsp_init(void);

#endif /* End of file */
