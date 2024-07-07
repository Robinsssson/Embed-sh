/**
 ******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @version V3.6.0
 * @date    20-September-2021
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2011 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include <sh/sh.h>
#include <sh/usart1.h>
#include <stdint.h>


#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "system_stm32f10x.h"

#ifdef USE_STM32100B_EVAL
#    include "stm32100b_eval_lcd.h"
#elif defined USE_STM3210B_EVAL
#    include "stm3210b_eval_lcd.h"
#elif defined USE_STM3210E_EVAL
#    include "stm3210e_eval_lcd.h"
#elif defined USE_STM3210C_EVAL
#    include "stm3210c_eval_lcd.h"
#elif defined USE_STM32100E_EVAL
#    include "stm32100e_eval_lcd.h"
#endif

/** @addtogroup STM32F10x_StdPeriph_Template
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef USE_STM32100B_EVAL
#    define MESSAGE1 "STM32 MD Value Line "
#    define MESSAGE2 " Device running on  "
#    define MESSAGE3 "  STM32100B-EVAL    "
#elif defined(USE_STM3210B_EVAL)
#    define MESSAGE1 "STM32 Medium Density"
#    define MESSAGE2 " Device running on  "
#    define MESSAGE3 "   STM3210B-EVAL    "
#elif defined(STM32F10X_XL) && defined(USE_STM3210E_EVAL)
#    define MESSAGE1 "  STM32 XL Density  "
#    define MESSAGE2 " Device running on  "
#    define MESSAGE3 "   STM3210E-EVAL    "
#elif defined(USE_STM3210E_EVAL)
#    define MESSAGE1 " STM32 High Density "
#    define MESSAGE2 " Device running on  "
#    define MESSAGE3 "   STM3210E-EVAL    "
#elif defined(USE_STM3210C_EVAL)
#    define MESSAGE1 " STM32 Connectivity "
#    define MESSAGE2 " Line Device running"
#    define MESSAGE3 " on STM3210C-EVAL   "
#elif defined(USE_STM32100E_EVAL)
#    define MESSAGE1 "STM32 HD Value Line "
#    define MESSAGE2 " Device running on  "
#    define MESSAGE3 "  STM32100E-EVAL    "
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#    define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#    define PUTCHAR_PROTOTYPE int fputc(int ch, FILE* f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);
void SystemConfig(void);
/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
    /*!< At this stage the microcontroller clock setting is already configured,
         this is done through SystemInit() function which is called from startup
         file (startup_stm32f10x_xx.s) before to branch to application main.
         To reconfigure the default setting of SystemInit() function, refer to
         system_stm32f10x.c file
       */
    SystemInit();
    SystemConfig();
    sh_usart_init(9600);
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    /* Infinite loop */
    while (1) {
        GPIO_ResetBits(GPIOE, GPIO_Pin_5);
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
        Delay(500);
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
        GPIO_SetBits(GPIOE, GPIO_Pin_5);
        Delay(500);
    }
}

void Delay(__IO uint32_t nTime)
{
    TimingDelay = nTime;
    while (TimingDelay != 0)
        ;
}

void SystemConfig(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1)
            ;
    }
}

/**
 * @brief  Decrements the TimingDelay variable.
 * @param  None
 * @retval None
 */
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}
/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE
{
    // /* Place your implementation of fputc here */
    // /* e.g. write a character to the USART */
    USART_SendData(USART1, (uint8_t) ch);

    // /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {
    }

    return ch;
}

#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */

    /* Infinite loop */
    while (1) {
    }
}
#endif

/**
 * @}
 */
