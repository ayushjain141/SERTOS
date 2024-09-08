/******************************************************************************
 * File Name: gpio_aj_stm32f4.c
 *
 * Description:
 * The file contains functions related to GPIO peripheral of STM32f407.
 *
 * Related Document: See README.md
 *
 *******************************************************************************/
#include "gpio_aj_stm32f4.h"

/*******************************************************************************
 * Function Name: gpio_output_config()
 ********************************************************************************
 * Summary:
 *   Configure output on GPIO pin.
 *
 * Parameters:
 *  GPIOx:      GPIO base.
 *  gpio_pin:   GPIO pin number.
 *  otyper:     GPIO output type.
 *  ospeedr:    GPIO output speed.
 *  pupdr:      Pull-up/pull-down or float config.
 *
 * Return :
 *  void
 *
 *******************************************************************************/
void gpio_output_config(GPIO_TypeDef *GPIOx, uint8_t gpio_pin,
                        gpio_otyper_t otyper, gpio_ospeedr_t ospeedr,
                        gpio_pupdr_t pupdr)
{
    /* Enable clock for the GPIO, simple arithematic operations used on the
     * GPIOx address and the hard-coded GPIO base addresses in the STM32 BSP,
     * for ports A,B,C,D etc, see file "stm32f407xx.h" for these base addresses.
     */
    RCC->AHB1ENR |= (uint32_t)((1U << ((GPIOx - GPIOA) / (GPIOB - GPIOA))));

    /* config mode as output in GPIO MODER config */
    GPIOx->MODER |= (uint32_t)((GPIOx->MODER & (~(3U << gpio_pin * 2))) |
                    (1U << (gpio_pin * 2)));

    /* config output type in GPIO OTYPER config */
    GPIOx->OTYPER |= (uint32_t)((((GPIOx->OTYPER) & (~(1U << gpio_pin))) |
                      (otyper << gpio_pin)));

    /* config outputspeed in GPIO OSPEEDR config */
    GPIOx->OSPEEDR |= (uint32_t)(((GPIOx->OSPEEDR & (~(3U << (gpio_pin * 2)))) |
                       (ospeedr << (gpio_pin * 2))));

    /* config no pull-up/pull-down in GPIO PUPDR config */
    GPIOx->PUPDR |= (uint32_t)(((GPIOx->PUPDR & (~(3U << (gpio_pin * 2)))) |
                     (pupdr << (gpio_pin * 2))));
}

/*******************************************************************************
* Function Name: gpio_input_config()
********************************************************************************
* Summary:
*   Configure input on GPIO input.
*
* Parameters:
*   GPIOx:      GPIO base.
*   gpio_pin:   GPIO pin number.
*   pupdr:      Pull-up/pull-down or float config.
*
* Return :
*  void
*
*******************************************************************************/
void gpio_input_config(GPIO_TypeDef *GPIOx, uint8_t gpio_pin,
                       gpio_pupdr_t pupdr)
{
    /* Enable clock for the GPIO, simple arithematic operations used on the
     * GPIOx address and the hard-coded GPIO base addresses in the STM32 BSP,
     * for ports A,B,C,D etc, see file "stm32f407xx.h" for these base addresses.
     */
    RCC->AHB1ENR |= (uint32_t)((1U << (uint8_t)((GPIOx - GPIOA) / (GPIOB - GPIOA))));

    /* config mode as input in GPIO MODER config */
    GPIOx->MODER |= (uint32_t)(((GPIOx->MODER & (~(3U << gpio_pin * 2))) |
                     (0U << (gpio_pin * 2))));

    /* config pull-up/pull-down in GPIO PUPDR config */
    GPIOx->PUPDR |= (uint32_t)(((GPIOx->PUPDR & (~(3U << (gpio_pin * 2)))) |
                     (pupdr << (gpio_pin * 2))));
}

/*******************************************************************************
* Function Name: config_gpio_interrupt()
********************************************************************************
* Summary:
*   Configure GPIO interrupt.
*
* Parameters:
*   GPIOx:          GPIO base.
*   gpio_pin:       GPIO pin number.
*   intr_conf_t:    Rising/falling edge interrupt directtion.
*
* Return :
*  void
*
*******************************************************************************/
void config_gpio_interrupt(GPIO_TypeDef *GPIOx, uint8_t gpio_pin_num,
                           gpio_intr_config_t *intr_conf_t)
{
    /* Enable RCC clock for SYSCFG peripheral. */
    RCC->APB2ENR |= (uint32_t)((uint32_t)(1U << RCC_APB2ENR_SYSCFGEN_Pos));

    /* Config External interrupt in SYSCFG register, based on pin number */
    SYSCFG->EXTICR[gpio_pin_num/4] |= (uint32_t)((SYSCFG->EXTICR[gpio_pin_num/4] &
    (~(0xF << (gpio_pin_num % 4) * 4))) |
        ((uint8_t)((GPIOx - GPIOA) / (GPIOB - GPIOA)) << (gpio_pin_num % 4) * 4));

    /* Disable masking of interrupt on line corresponding to gpio_pin_num. */
    EXTI->IMR |= (uint32_t)(1U << gpio_pin_num);

    /* Event generation masking/un-masking is unused in this function. */

    /* It can be rising edge only, falling edge only or rising + falling edge */

    /* Config Rising edge trigger selection */
    EXTI->RTSR |=  (uint32_t)((EXTI->RTSR & (~((uint32_t)(1U << gpio_pin_num))) |
            ((intr_conf_t->rising_edge) << gpio_pin_num)));

    /* Config Falling edge trigger selection */
    EXTI->FTSR |= (uint32_t)((EXTI->FTSR & (~((uint32_t)(1U << gpio_pin_num))) |
            ((intr_conf_t->falling_edge) << gpio_pin_num)));
}
