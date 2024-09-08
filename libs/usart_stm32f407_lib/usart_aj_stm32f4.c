/*******************************************************************************
 * File Name: usart_aj_stm32f4.c
 *
 * Description:
 * The file contains function definition(s) for USART peripheral of STM32F407.
 *
 * Related Document: See README.md
 *
 *******************************************************************************/
#include "usart_aj_stm32f4.h"

/*******************************************************************************
 * Function Name: usart_config()
 ********************************************************************************
 * Summary:
 *      Configures the USART peripheral.
 *
 * Parameters:
 *      usart_cfg:      Pointer to USART configs
 *      tx_GPIOx:       Pointer to USART TX pin port, used in TX mode
 *      tx_gpio_pin:    USART Tx GPIO pin, used in TX mode
 *      rx_GPIOx:       Pointer to USART Rx GPIO pin, used in RX mode
 *      rx_gpio_pin:    USART Rx GPIO pin, used in RX mode
 *
 * Return :
 *      usart_status_e_t:   USART config operation result
 *
 *******************************************************************************/
usart_status_e_t usart_config(usart_config_st_t *usart_cfg, GPIO_TypeDef *tx_GPIOx,
                              uint8_t tx_gpio_pin, GPIO_TypeDef *rx_GPIOx, uint8_t rx_gpio_pin)
{
    uint32_t tmp = 0;
    float baud_usart_div = 0;
    uint32_t baud_fractn = 0, baud_mantissa = 0;
    uint32_t sys_clock = 0;

    /* Check whether the usart instance is valid or not - TO DO */

    /* Enable the USARTx/UARTx peripheral clock */
    if ((USART1 == usart_cfg->instance) || (USART6 == usart_cfg->instance))
    {
        /* Calculate pos of USARTx/UART enable bit in RCC_APB2ENR reg,
         * using pointer arithmatic on memory mapped addr of the USART/UART instance
         */
        tmp = ((usart_cfg->instance - USART1) / (USART6 - USART1));

        /* Enable clock */
        RCC->APB2ENR |= (uint32_t)((RCC->APB2ENR & (~(3U << RCC_APB2ENR_USART1EN_Pos))) |
                                   (1U << (RCC_APB2ENR_USART1EN_Pos + tmp)));
    }
    else
    {
        /* Calculate pos of USARTx/UART enable bit in RCC_APB2ENR reg,
         * using pointer arithmatic on memory mapped addr of the USART/UART instance
         */
        tmp = ((usart_cfg->instance - USART2) / (USART3 - USART2));

        /* Enable clock */
        RCC->APB1ENR |= (uint32_t)((RCC->APB1ENR & (~(0xFU << RCC_APB1ENR_USART2EN_Pos))) |
                                   (1U << (RCC_APB1ENR_USART2EN_Pos + tmp)));
    }

    /* Enable USARTx/UARTx GPIO clock, simple pointer arithematic operations used on the
     * GPIOx address and the hard-coded GPIO base addresses in the STM32 BSP,
     * for ports A,B,C,D etc, see file "stm32f407xx.h" for these base addresses.
     */
    /* For Tx */
    RCC->AHB1ENR |= (uint32_t)((1U << (uint32_t)((tx_GPIOx - GPIOA) / (GPIOB - GPIOA))));
    /* For Rx */
    RCC->AHB1ENR |= (uint32_t)((1U << (uint32_t)((rx_GPIOx - GPIOA) / (GPIOB - GPIOA))));

    /* Check whether the GPIO port and pin corresponds to the specified USART
     * instance - TO DO.
     */

    /* Set GPIO pin to Alternate Function(AF) mode */
    /* For Tx */
    tx_GPIOx->MODER |= (uint32_t)((tx_GPIOx->MODER & (~(3U << (tx_gpio_pin * 2)))) |
                                  (2U << (tx_gpio_pin * 2)));
    /* For Rx */
    rx_GPIOx->MODER |= (uint32_t)((rx_GPIOx->MODER & (~(3U << (rx_gpio_pin * 2)))) |
                                  (2U << (rx_gpio_pin * 2)));

    /* If Tx pin specified, set output type, speed, pull-up/pull-down */
    if (USART_TXRX_MODE_TX_EN == usart_cfg->txrxmode ||
        USART_TXRX_MODE_RX_TX_BOTH_EN == usart_cfg->txrxmode)
    {
        /* Output type set to push-pull */
        tx_GPIOx->OTYPER |= (uint32_t)((tx_GPIOx->OTYPER) & (~(1U << tx_gpio_pin)));

        /* Output speed set to max speed */
        tx_GPIOx->OSPEEDR |= (uint32_t)((tx_GPIOx->OSPEEDR & (~(3U << (tx_gpio_pin * 2)))) |
                                        (3U << (tx_gpio_pin * 2)));

        /* No pull-up, pull-down enabled */
        tx_GPIOx->PUPDR |= (uint32_t)((tx_GPIOx->PUPDR & (~(3U << (tx_gpio_pin * 2)))));
    }

    /* Set alternate function to pin, this AF number is from product datasheet's
     * alternate function mapping for pins */
    if ((USART1 == usart_cfg->instance) || (USART2 == usart_cfg->instance) ||
        (USART3 == usart_cfg->instance))
    {

        if (USART_TXRX_MODE_TX_EN == usart_cfg->txrxmode)
        {
            tmp = tx_gpio_pin / 8;
            tx_GPIOx->AFR[tmp] |= (uint32_t)((tx_GPIOx->AFR[tmp] & (~(0xFU << ((tx_gpio_pin % 8) * 4)))) |
                                             (7U << ((tx_gpio_pin % 8) * 4)));
        }

        if (USART_TXRX_MODE_RX_EN == usart_cfg->txrxmode)
        {
            tmp = rx_gpio_pin / 8;
            rx_GPIOx->AFR[tmp] |= (uint32_t)((rx_GPIOx->AFR[tmp] & (~(0xFU << ((rx_gpio_pin % 8) * 4)))) |
                                             (7U << ((rx_gpio_pin % 8) * 4)));
        }

        if (USART_TXRX_MODE_RX_TX_BOTH_EN == usart_cfg->txrxmode)
        {
            tmp = tx_gpio_pin / 8;
            tx_GPIOx->AFR[tmp] |= (uint32_t)((tx_GPIOx->AFR[tmp] & (~(0xFU << ((tx_gpio_pin % 8) * 4)))) |
                                             (7U << ((tx_gpio_pin % 8) * 4)));

            tmp = rx_gpio_pin / 8;
            rx_GPIOx->AFR[tmp] |= (uint32_t)((rx_GPIOx->AFR[tmp] & (~(0xFU << ((rx_gpio_pin % 8) * 4)))) |
                                             (7U << ((rx_gpio_pin % 8) * 4)));
        }
    }

    else if ((UART4 == usart_cfg->instance) || (UART5 == usart_cfg->instance) ||
             (USART6 == usart_cfg->instance))
    {
        if (USART_TXRX_MODE_TX_EN == usart_cfg->txrxmode)
        {
            tmp = tx_gpio_pin / 8;
            tx_GPIOx->AFR[tmp] |= (uint32_t)((tx_GPIOx->AFR[tmp] & (~(0xFU << ((tx_gpio_pin % 8) * 4)))) |
                                             (8U << ((tx_gpio_pin % 8) * 4)));
        }

        if (USART_TXRX_MODE_RX_EN == usart_cfg->txrxmode)
        {
            tmp = rx_gpio_pin / 8;
            rx_GPIOx->AFR[tmp] |= (uint32_t)((rx_GPIOx->AFR[tmp] & (~(0xFU << ((rx_gpio_pin % 8) * 4)))) |
                                             (8U << ((rx_gpio_pin % 8) * 4)));
        }

        if (USART_TXRX_MODE_RX_TX_BOTH_EN == usart_cfg->txrxmode)
        {
            tmp = tx_gpio_pin / 8;
            tx_GPIOx->AFR[tmp] |= (uint32_t)((tx_GPIOx->AFR[tmp] & (~(0xFU << ((tx_gpio_pin % 8) * 4)))) |
                                             (8U << ((tx_gpio_pin % 8) * 4)));

            tmp = rx_gpio_pin / 8;
            rx_GPIOx->AFR[tmp] |= (uint32_t)((rx_GPIOx->AFR[tmp] & (~(0xFU << ((rx_gpio_pin % 8) * 4)))) |
                                             (8U << ((rx_gpio_pin % 8) * 4)));
        }
    }

    /* Create the config data for USART_CR1 register */
    tmp = (uint32_t)(((uint32_t)usart_cfg->txrxmode << USART_CR1_RE_Pos) |
                     ((uint32_t)usart_cfg->parity_en << USART_CR1_PCE_Pos) |
                     ((uint32_t)usart_cfg->parity << USART_CR1_PS_Pos) |
                     ((uint32_t)usart_cfg->wordlen << USART_CR1_M_Pos) |
                     ((uint32_t)usart_cfg->oversample << USART_CR1_OVER8_Pos));

    /* Clear the bits of USART_CR1 reg to program it's config values */
    usart_cfg->instance->CR1 &= (uint32_t)(~((3U << USART_CR1_RE_Pos) | (1U << USART_CR1_PCE_Pos) |
                                             (1U << USART_CR1_PS_Pos) | (1U << USART_CR1_M_Pos) | (1U << USART_CR1_OVER8_Pos)));

    /* Program the config data for USART_CR1 register  */
    usart_cfg->instance->CR1 |= tmp;

    /* Create the config data for USART_CR2 register */
    tmp = (uint32_t)((uint32_t)usart_cfg->stopbits << USART_CR2_STOP_Pos);

    /* Clear the bits of USART_CR2 reg to program it's config values */
    usart_cfg->instance->CR2 &= (uint32_t)(~(3U << USART_CR2_STOP_Pos));

    /* Create the config data for USART_CR2 register */
    usart_cfg->instance->CR2 |= tmp;

    sys_clock = get_systemcore_clock();

    /* Calculate the fractional part and mantissa of the baud rate */
    if (USART_COMPATIBLE_MODE_ASYNC == usart_cfg->compatmode ||
        USART_COMPATIBLE_MODE_SYNC == usart_cfg->compatmode)
    {
        baud_usart_div = ((sys_clock) / (usart_cfg->baudrate * 8 * (2 - usart_cfg->oversample)));

        if (USART_OVERSAMPLE_BY_16 == usart_cfg->oversample)
        {
            baud_fractn = (uint32_t)round((double)((baud_usart_div - (int)baud_usart_div) * 16));
            baud_mantissa = (baud_fractn > 15U) ? (1U) : (0U);
            baud_mantissa += (uint32_t)(baud_usart_div);

            usart_cfg->instance->BRR = 0U;
            usart_cfg->instance->BRR |= (uint32_t)(baud_fractn | (baud_mantissa << 4U));
        }
        else if (USART_OVERSAMPLE_BY_8 == usart_cfg->oversample)
        {
            baud_fractn = (uint32_t)round((double)((baud_usart_div - (int)baud_usart_div) * 8));
            baud_mantissa = (baud_fractn > 7U) ? (1U) : (0U);
            baud_mantissa += (uint32_t)(baud_usart_div);

            /* Program the calculated USART_DIV mantissa and fraction in BRR reg */
            usart_cfg->instance->BRR = 0U;
            usart_cfg->instance->BRR |= (uint32_t)((baud_fractn & 0x7U) |
                                                   (baud_mantissa << 4U));
        }
    }

    return USART_STATUS_SUCCESS;
}

/*******************************************************************************
 * Function Name: usart_init()
 ********************************************************************************
 * Summary:
 *   Initializes the UART channel.
 *
 * Parameters:
 *   usart_cfg:     Pointer to USART configs
 *
 * Return :
 *  usart_status_e_t:
 *
 *******************************************************************************/
usart_status_e_t usart_init(usart_config_st_t *usart_cfg)
{
    usart_cfg->instance->CR1 |= (1U << USART_CR1_UE_Pos);
    return USART_STATUS_SUCCESS;
}

/*******************************************************************************
 * Function Name: usart_deinit()
 ********************************************************************************
 * Summary:
 *   De-initializes the UART channel.
 *
 * Parameters:
 *   usart_cfg:     Pointer to USART configs
 *
 * Return :
 *  usart_status_e_t:
 *
 *******************************************************************************/
usart_status_e_t usart_deinit(usart_config_st_t *usart_cfg)
{
    usart_cfg->instance->CR1 &= (~(1U << USART_CR1_UE_Pos));
    return USART_STATUS_SUCCESS;
}

/*******************************************************************************
 * Function Name: uart_receive_poll()
 ********************************************************************************
 * Summary:
 *   Receive UART data and store it into the specified buffer.
 *
 * Parameters:
 *  usart_cfg:          Pointer to USART configs
 *  rx_buff:            Pointer to buffer to store Rx data
 *  rx_buff_size:       Size of the RX data buffer
 *  timeout_milsec:     (TO DO) The API waits for this much time in ms for receiving the
 *                      data and then terminates.
 *
 * Return :
 *  usart_status_e_t:   Status of this receive operation
 *
 *******************************************************************************/
usart_status_e_t uart_receive_poll(usart_config_st_t *usart_cfg, uint8_t *rx_buff,
                                   uint16_t rx_buff_size, uint32_t timeout_milsec)
{
    for (uint16_t i = 0; i < rx_buff_size; i++)
    {
        /* Wait while the RX data can be safely read from the HW Rx buffer */
        while (!(usart_cfg->instance->SR & (1U << USART_SR_RXNE_Pos)))
            ;

        /* Now the data can be safely read from the HW Rx buffer,
         * Read operation clears the RXNE flag .*/
        rx_buff[i] = usart_cfg->instance->DR;
    }

    return USART_STATUS_SUCCESS;
}

/*******************************************************************************
 * Function Name: uart_transmit_blocking()
 ********************************************************************************
 * Summary:
 *   Transmit UART data and from the specified buffer.
 *
 * Parameters:
 *   usart_cfg:          Pointer to USART configs
 *   tx_buff:            Pointer to buffer of Tx data
 *   tx_buff_size:       Size of the TX data buffer
 *   timeout_milsec:     (TO DO) The API waits for this much time in ms for
 *                       transmitting the data and then terminates.
 *
 * Return :
 *  usart_status_e_t:   Status of this transmit operation
 *
 *******************************************************************************/
usart_status_e_t uart_transmit_blocking(usart_config_st_t *usart_cfg, uint8_t *tx_buff,
                                        uint16_t tx_buff_size, uint32_t timeout_milsec)
{
    for (uint16_t i = 0; i < tx_buff_size; i++)
    {
        /* Wait for the Transmit buffer to be ready to accept data safely */
        while (!(usart_cfg->instance->SR & (1U << USART_SR_TXE_Pos)))
            ;

        /* The transmission data can be safely put to Tx data register
         * (HW Tx buffer). This operation also clears the TXE bit. */
        usart_cfg->instance->DR = tx_buff[i];
    }

    return USART_STATUS_SUCCESS;
}

/*******************************************************************************
 * Function Name: uart_rx_interrupt_set()
 ********************************************************************************
 * Summary:
 *   Configure the interrupt on the UART RX.
 *
 * Parameters:
 *   uart_inst:          UART instance
 *   rx_int_en:          Enable/disable value for RX interupt.
 *
 * Return :
 *  usart_status_e_t:    Status of RX interrupt config operation.
 *
 *******************************************************************************/
usart_status_e_t uart_rx_interrupt_set(USART_TypeDef *uart_inst, bool rx_int_en)
{
    uart_inst->CR1 |= (uint32_t)((uart_inst->CR1 & (~(USART_CR1_RXNEIE_Msk))) |
                                 ((uint32_t)rx_int_en << USART_CR1_RXNEIE_Pos));

    return USART_STATUS_SUCCESS;
}

/* End of File */
