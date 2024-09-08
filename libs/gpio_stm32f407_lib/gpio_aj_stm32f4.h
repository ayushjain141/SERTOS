/******************************************************************************
* File Name: gpio_aj_stm32f4.h
*
* Description:
* Contains declarations for GPIO related funcitons.
*
* Related Document: See README.md
*
*******************************************************************************/
#ifndef GPIO_AJ_STM32F4
#define GPIO_AJ_STM32F4

#include "stm32f4xx.h"
#include <stdbool.h>

#ifdef  __STM32F407xx_H
#include "stm32f407xx.h"
#endif  /* __STM32F407xx_H */

/*******************************************************************************
 * Macros
 *******************************************************************************/
#define SET_PIN(GPIOx, GPIO_pin)            (GPIOx -> BSRR |= (1 << GPIO_pin) & (~(1 << (GPIO_pin + 16))))
#define RESET_PIN(GPIOx, GPIO_pin)          (GPIOx -> BSRR |= (1 << (GPIO_pin + 16)) & (~(1 << GPIO_pin)))
#define TOGGLE_PIN(GPIOx, GPIO_pin)         (GPIOx -> ODR ^= (1U << GPIO_pin))

#define GENERAL_PURPOSE_INPUT               (0x00)
#define GENERAL_PURPOSE_OUTPUT              (0X01)

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* GPIO Mode */
typedef enum
{
    gpio_moder_in,
    gpio_moder_out,
    gpio_moder_alternate_func,
    gpio_moder_analog,
} gpio_moder_t;

/* GPIO output type */
typedef enum
{
    gpio_otyper_push_pull,
    gpio_otyper_open_drain,
} gpio_otyper_t;

/* GPIO output speed */
typedef enum
{
    gpio_ospeedr_low,
    gpio_ospeedr_med,
    gpio_ospeedr_high,
    gpio_ospeedr_very_high,
} gpio_ospeedr_t;

/* GPIO pull-up/pull-down direction */
typedef enum
{
    gpio_pupdr_float,
    gpio_pupdr_pull_up,
    gpio_pupdr_pull_down,
} gpio_pupdr_t;

/* GPIO interrupt edge trigger */
typedef struct gpio_intr_config {
    bool rising_edge;
    bool falling_edge;
} gpio_intr_config_t;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void gpio_output_config(GPIO_TypeDef *GPIOx, uint8_t gpio_pin,
                        gpio_otyper_t otyper, gpio_ospeedr_t ospeedr,
                        gpio_pupdr_t pupdr);

void gpio_input_config(GPIO_TypeDef *GPIOx, uint8_t gpio_pin,
                       gpio_pupdr_t pupdr);

void config_gpio_interrupt(GPIO_TypeDef *GPIOx, uint8_t gpio_pin_num,
                           gpio_intr_config_t *intr_conf_t);

static __inline bool pin_read(GPIO_TypeDef* GPIOx, uint32_t gpio_pin)
{
    return (0x01 & (uint32_t)(GPIOx->IDR >> gpio_pin));
}

#endif  /* GPIO_AJ_STM32F4 */
