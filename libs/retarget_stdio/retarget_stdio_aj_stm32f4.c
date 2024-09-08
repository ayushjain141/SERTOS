/*******************************************************************************
 * File Name: retarget_stdio_aj_stm32f4.c
 *
 * Description:
 *   The file contains definitions for stdio re-targetting.
 *
 * Related Document: See README.md
 *
 ******************************************************************************/
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "retarget_stdio_aj_stm32f4.h"

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
static uint8_t print_buff_size = 1;

/* Will be initialized by the printf_retarget_uart_init() */
static usart_config_st_t *retarg_usartcfg_ptr = NULL;

/* Start of ARM specific declarations for stdout/stdin, used for printf */
struct __FILE
{
  /* Just for redefining __FILE, won't be used */
  int dummyVar;
};

/* STDOUT */
FILE __stdout;

/* STDIN */
FILE __stdin;
/* End of ARM specific declarations for stdout/stdin used for printf */

/* UART config struct for printf re-targetting */
usart_config_st_t printf_usartcfg = {
    .compatmode = PRINTF_UART_COMPATMODE,
    .stopbits = PRINTF_UART_STOPBITS,
    .txrxmode = PRINTF_UART_TXRXMODE,
    .hwflowctrl = PRINTF_UART_HWFLOWCTRL,
    .instance = PRINTF_UART_INSTANCE,
    .baudrate = PRINTF_UART_BAUDRATE,
    .wordlen = PRINTF_UART_WORDLEN,
    .oversample = PRINTF_UART_OVERSAMPLE,
    .parity_en = PRINTF_UART_PARITY_EN,
    .parity = PRINTF_UART_PARITY,
};

/*******************************************************************************
 * Function Name: fputc()
 *******************************************************************************
 * Summary:
 *  Defines the "fputc" function used in "printf". Here the "fputc" is implemented
 *  to work with UART transmit function.
 *
 * Parameters:
 *  ch:             Character to be printed, is typecasted to char type for uart
 *                  transmit.
 *  stream:         File stream to send the output to. Not used in case of
 *                  printf using uart.
 *
 * Return :
 *  int             Returns the printed character on success.
 *
 ******************************************************************************/
int fputc(int ch, FILE *stream)
{
  uart_transmit_blocking(retarg_usartcfg_ptr, (uint8_t *)&ch, print_buff_size, 0);
  return ch;
}

/*******************************************************************************
 * Function Name: printf_retarget_uart_init()
 *******************************************************************************
 * Summary:
 *    This function initializes the UART for redirecting the printf.
 *
 *    NOTE: The HW/SW configs used in this function are present in the file BSP
 *    libs header file.
 *
 * Parameters:
 *  usart_cfg:    UART config structure for "printf" feature.
 *  tx_GPIOx:     GPIO TX instance for "printf" feature.
 *  tx_gpio_pin:  GPIO TX pin for for "printf" feature.
 *  rx_GPIOx:     GPIO RX instance for "printf" feature.
 *  rx_gpio_pin:  GPIO RX pin for for "printf" feature.
 *
 * Return :
 *  result_funct_e_t:   Function completion status.
 *
 ******************************************************************************/
result_funct printf_retarget_uart_init(void)
{
  result_funct res = RESULT_FUNCT_STATUS_SUCCESS;

  retarg_usartcfg_ptr = &printf_usartcfg;

  /* UART config for printf re-targetting. */
  res = usart_config(&printf_usartcfg, PRINTF_UART_TX_GPIO_PORT,
                     PRINTF_UART_TX_GPIO_PIN, PRINTF_UART_RX_GPIO_PORT, PRINTF_UART_RX_GPIO_PIN);

  if (RESULT_FUNCT_STATUS_SUCCESS != res)
  {
    return res;
  }

  /* Initialize the UART channel for printf re-targetting. */
  res = usart_init(&printf_usartcfg);
  if (RESULT_FUNCT_STATUS_SUCCESS != res)
  {
    return res;
  }

  return res;
}

/* End of File */
