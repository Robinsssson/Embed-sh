#include <sh/usart1.h>
#include <stdint.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#define CAP 1000
typedef struct __usart1_cache_buf {
    char global_sh_read_buf[CAP];
    int  cap;
    int  ptr;
} usart1_cache_buf;
usart1_cache_buf global_usart1_buf;

void sh_usart_init(uint32_t baudrate)
{
    global_usart1_buf.cap = CAP;
    global_usart1_buf.ptr = 0;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    USART_DeInit(USART1);

    GPIO_InitTypeDef GPIO_InitStructure = {GPIO_Pin_9, GPIO_Speed_50MHz,
                                           GPIO_Mode_AF_PP};
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure = {baudrate,
                                             USART_WordLength_8b,
                                             USART_StopBits_1,
                                             USART_Parity_No,
                                             USART_Mode_Rx | USART_Mode_Tx,
                                             USART_HardwareFlowControl_None};
    USART_Init(USART1, &USART_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure = {USART1_IRQn, 7, 1, ENABLE};
    NVIC_Init(&NVIC_InitStructure);
    USART_Cmd(USART1, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART1_IRQnHandler()
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}