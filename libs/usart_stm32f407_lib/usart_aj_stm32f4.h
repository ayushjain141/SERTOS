/*******************************************************************************
* File Name: usart_aj_stm32f4.h
*
* Description:
* The file contains function declaration(s) for USART peripheral of STM32f407.
*
* Related Document: See README.md
*
*******************************************************************************/
#ifndef USART_AJ_STM32F4
#define USART_AJ_STM32F4

/*******************************************************************************
* Header Files
*******************************************************************************/
#include <stdint.h>
#include <math.h>

#include "stm32f4xx.h"
#include <stdbool.h>

#include "bsp_aj_stm32f4.h"
#include "gpio_aj_stm32f4.h"
#include "rcc_aj_stm32f4.h"

#ifdef  __STM32F407xx_H
#include "stm32f407xx.h"
#endif  /* __STM32F407xx_H */


/*******************************************************************************
* Macros
*******************************************************************************/
#define USART_WORD_LEN_8_BIT                (0U)
#define USART_WORD_LEN_9_BIT                (1U)
#define USART_OVERSAMPLE_BY_16              (0U)
#define USART_OVERSAMPLE_BY_8               (1U)
#define USART_PARITY_DISABLE                (0U)
#define USART_PARITY_ENABLE                 (1U)
#define USART_PARITY_EVEN                   (0U)
#define USART_PARITY_ODD                    (1U)

/*******************************************************************************
 * Global Variables
 *******************************************************************************/
typedef enum usart_compatible_mode_e
{
    USART_COMPATIBLE_MODE_ASYNC,
    USART_COMPATIBLE_MODE_SYNC,
    USART_COMPATIBLE_MODE_LIN,
    USART_COMPATIBLE_MODE_IRDA,
    USART_COMPATIBLE_MODE_SMARTCARD,
} usart_compatible_mode_e_t;

typedef enum usart_txrx_mode_e
{
    USART_TXRX_MODE_RX_EN = 1,
    USART_TXRX_MODE_TX_EN,
    USART_TXRX_MODE_RX_TX_BOTH_EN,
} usart_txrx_mode_e_t;

typedef enum usart_flow_ctrl_e
{
    USART_FLOWCTRL_NONE,
    USART_FLOWCTRL_RTS_EN,
    USART_FLOWCTRL_CTS_EN,
    USART_FLOWCTRL_RTS_CTS_BOTH_EN,
} usart_hwflowctrl_e_t;

typedef enum usart_stopbit_e
{
    USART_STOPBIT_1,
    USART_STOPBIT_0_5,
    USART_STOPBIT_2,
    USART_STOPBIT_1_5,
} usart_stopbit_e_t;

typedef struct usart_config_st
{
    usart_compatible_mode_e_t compatmode;
    usart_stopbit_e_t stopbits;
    usart_txrx_mode_e_t txrxmode;
    usart_hwflowctrl_e_t hwflowctrl;
    USART_TypeDef *instance;
    uint32_t baudrate;
    bool wordlen;
    bool oversample;
    bool parity_en;
    bool parity;
} usart_config_st_t;

typedef enum usart_status_e
{
    USART_STATUS_SUCCESS,
    USART_STATUS_FAIL,
    USART_STATUS_BAD_PARAM,
    UART_STATUS_RECEIVE_BUSY,
    UART_STATUS_TRANSMIT_BUSY,
} usart_status_e_t;

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/
usart_status_e_t usart_config(usart_config_st_t *usart_cfg, GPIO_TypeDef *tx_GPIOx,
                              uint8_t tx_gpio_pin, GPIO_TypeDef *rx_GPIOx, uint8_t rx_gpio_pin);

usart_status_e_t usart_init(usart_config_st_t *usart_cfg);
usart_status_e_t usart_deinit(usart_config_st_t *usart_cfg);

usart_status_e_t uart_receive_poll(usart_config_st_t *usart_cfg, uint8_t *rx_buff,
                                   uint16_t rx_buff_size, uint32_t timeout_milsec);

usart_status_e_t uart_transmit_blocking(usart_config_st_t *usart_cfg, uint8_t *tx_buff,
                                        uint16_t tx_buff_size, uint32_t timeout_milsec);

usart_status_e_t uart_rx_interrupt_set(USART_TypeDef *uart_inst, bool rx_int_en);

#endif /* USART_AJ_STM32F4 */
