/*******************************************************************************
 * File Name:    rcc_aj_stm32f4.h
 *
 * Description:
 * Contains function declaration for RCC peripheral.
 *
 * Related Document: See README.md
 *
 ******************************************************************************/
#ifndef RCC_AJ_STM32F4
#define RCC_AJ_STM32F4

/*
 * The functions in this file may need to be called in "system_stm32f4xx.c" in
 * the system init function. Include this header file in the folder where
 * "system_stm32f4xx" is present, for keil it is "RTE/device/STM32F407VETx" and
 * for including in other ".c" files, point to this location like -
 * #include "RTE/Device/STM32F407VETx/rcc_aj_stm32f4.h"
 */
#include "stm32f4xx.h"

#ifdef __STM32F407xx_H
#include "stm32f407xx.h"
#endif

#include "core_cm4.h"
#include "bsp_aj_stm32f4.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
/* PLL clock source */
#define PLL_CLOCK_SRC_HSI                       (0)
#define PLL_CLOCK_SRC_HSE                       (1U)
#define RCC_SUCCESS_FLAG                        (0)
#define RCC_FAIL_FLAG                           (1)
#define SYSTICK_RELOAD_VAL_MAX                  (SysTick_LOAD_RELOAD_Msk)
#define AHB_PRESSCALER_ARR_SIZE                 (9)
#define APB1_PRESSCALER_ARR_SIZE                (5)
#define APB2_PRESSCALER_ARR_SIZE                (5)
#define RTC_PRESSCALER_ARR_SIZE                 (31)    /* To Do */

/*PLL dividers/multipliers */
/*
 * If PLL needed as system clock
 *
 * * PLL output frequency formula -
 *
 *    1. f_vco = f_inclock * (PLLLN/PLLM)
 *    ( finclock/PLLM = 2MHz and f_vco in range [100,432] MHz to be ensured )
 *
 *    2. f_pllout = f_vco / PLLP
 *    ( f_pllout in range [,168] MHz to be ensured )
 *
 *    3. final formula for PLL:
 *        f_pllout = f_inclock * (PLLN/(PLLM * PLLP))
 */

/*******************************************************************************
 * Structures and enums
 ******************************************************************************/
typedef enum
{
    MCO_CHANNEL_1,
    MCO_CHANNEL_2
} MCO_CHANNEL_e;

typedef enum
{
    MCO2_CLOCK_SOURCE_SYSCLK = 0,
    MCO2_CLOCK_SOURCE_PLLI2S,
    MCO2_CLOCK_SOURCE_HSE,
    MCO2_CLOCK_SOURCE_PLL,

    MCO1_CLOCK_SOURCE_HSI = 0,
    MCO1_CLOCK_SOURCE_LSE,
    MCO1_CLOCK_SOURCE_HSE,
    MCO1_CLOCK_SOURCE_PLL
} MCO_CLOCK_SOURCE_e;

typedef enum
{
    MCO_PRESCALER_NO_DIV = 0,
    MCO_PRESCALER_BY_2 = 4,
    MCO_PRESCALER_BY_3,
    MCO_PRESCALER_BY_4,
    MCO_PRESCALER_BY_5
} MCO_PRESCALER_e;

typedef enum
{
    SYS_CLOCK_SOURCE_HSI = 0,
    SYS_CLOCK_SOURCE_HSE,
    SYS_CLOCK_SOURCE_PLL
} SYS_CLOCK_SOURCE_e;

typedef enum
{
    PLL_CLOCK_SOURCE_HSI = 0,
    PLL_CLOCK_SOURCE_HSE
} PLL_CLOCK_SOURCE_e;

typedef enum
{
    PLL_P_2 = 0,
    PLL_P_4,
    PLL_P_6,
    PLL_P_8
} PLL_P_VAL_e;

typedef enum
{
    HPRE_AHB_PRESCALER_NO_DIV = 0,
    HPRE_AHB_PRESCALER_BY_2 = 8,
    HPRE_AHB_PRESCALER_BY_4,
    HPRE_AHB_PRESCALER_BY_8,
    HPRE_AHB_PRESCALER_BY_16,
    HPRE_AHB_PRESCALER_BY_64,
    HPRE_AHB_PRESCALER_BY_128,
    HPRE_AHB_PRESCALER_BY_256,
    HPRE_AHB_PRESCALER_BY_512
} HPRE_AHB_PRESCALER_e;

typedef enum
{
    PPRE1_APB1_PRESCALER_NO_DIV = 0,
    PPRE1_APB1_PRESCALER_BY_2 = 4,
    PPRE1_APB1_PRESCALER_BY_4,
    PPRE1_APB1_PRESCALER_BY_8,
    PPRE1_APB1_PRESCALER_BY_16
} PPRE1_APB1_PRESCALER_e;

typedef enum
{
    PPRE2_APB2_PRESCALER_NO_DIV = 0,
    PPRE2_APB2_PRESCALER_BY_2 = 4,
    PPRE2_APB2_PRESCALER_BY_4,
    PPRE2_APB2_PRESCALER_BY_8,
    PPRE2_APB2_PRESCALER_BY_16
} PPRE2_APB2_PRESCALER_e;

typedef enum
{
    RTCPRE_PRESCALER_BY_NO_CLOCK1 = 0,
    RTCPRE_PRESCALER_BY_NO_CLOCK2,
    RTCPRE_PRESCALER_BY_2,
    RTCPRE_PRESCALER_BY_3,
    RTCPRE_PRESCALER_BY_4,
    RTCPRE_PRESCALER_BY_5,
    RTCPRE_PRESCALER_BY_6,
    RTCPRE_PRESCALER_BY_7,
    RTCPRE_PRESCALER_BY_8,
    RTCPRE_PRESCALER_BY_9,
    RTCPRE_PRESCALER_BY_10,
    RTCPRE_PRESCALER_BY_11,
    RTCPRE_PRESCALER_BY_12,
    RTCPRE_PRESCALER_BY_13,
    RTCPRE_PRESCALER_BY_14,
    RTCPRE_PRESCALER_BY_15,
    RTCPRE_PRESCALER_BY_16,
    RTCPRE_PRESCALER_BY_17,
    RTCPRE_PRESCALER_BY_18,
    RTCPRE_PRESCALER_BY_19,
    RTCPRE_PRESCALER_BY_20,
    RTCPRE_PRESCALER_BY_21,
    RTCPRE_PRESCALER_BY_22,
    RTCPRE_PRESCALER_BY_23,
    RTCPRE_PRESCALER_BY_24,
    RTCPRE_PRESCALER_BY_25,
    RTCPRE_PRESCALER_BY_26,
    RTCPRE_PRESCALER_BY_27,
    RTCPRE_PRESCALER_BY_28,
    RTCPRE_PRESCALER_BY_29,
    RTCPRE_PRESCALER_BY_30,
    RTCPRE_PRESCALER_BY_31

} RTCPRE_PRESCALER_e;

typedef enum
{
    SYSTICK_TICKINT_DISABLE,
    SYSTICK_TICKINT_ENABLE
} SYSTICK_TICKINT_e;

typedef enum
{
    SYSTICK_CLKSOURCE_AHB_BY8,
    SYSTICK_CLKSOURCE_AHB
} SYSTICK_CLKSOURCE_e;

typedef struct
{
    uint32_t PLLM;
    uint32_t PLLN;
    uint32_t PLLP;
    uint32_t PLLQ;
} RCC_PLL_CONFIG_PARAMS_t;

typedef struct
{
    uint8_t ppre1_apb1_pre;
    uint8_t ppre2_apb2_pre;
    uint8_t rtcpre_pre;
} system_bus_clk_cfg_t;

/*******************************************************************************
 * variables
 ******************************************************************************/
extern RCC_PLL_CONFIG_PARAMS_t pll_config;


/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
void MCO_Config(MCO_CHANNEL_e mco_channel, MCO_CLOCK_SOURCE_e mco_clock_source,
                MCO_PRESCALER_e mco_prescaler);
void RCC_System_Clock_Source_Config(int system_clock_source, int pll_clock_source,
                                        RCC_PLL_CONFIG_PARAMS_t *rcc_pll_config_param_t);
void system_clock_setting(uint32_t sys_core_clock,
                             system_bus_clk_cfg_t *sys_bus_clk_cfg);
int systick_config_init(uint32_t interrupt_mode, uint32_t ticks);
void delay_us_systick(uint32_t us_delay);
void delay_ms_systick(uint32_t ms_delay);
uint32_t get_systemcore_clock(void);

#endif /* End of File */
