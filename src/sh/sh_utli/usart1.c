#include <sds/sds.h>
#include <sh/usart1.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "sys/_intsup.h"

usart1_cache_buf global_usart1;

static int __put_char(char ch);
static int check_usart1_cache_safe()
{
    return global_usart1.cap >= global_usart1.len;
}

void sh_usart_init(uint32_t baudrate)
{
    global_usart1.cap          = CAP;
    global_usart1.len          = 0;
    global_usart1.over_label   = WAITING;
    global_usart1.escape_label = 0;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,
                           ENABLE);
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

    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure = {USART1_IRQn, 1, 1, ENABLE};

    NVIC_Init(&NVIC_InitStructure);
}

int USART1_Printf(const char* __str, ...)
{
    va_list ap;
    va_start(ap, __str);
    for (int i = 0; __str[i] != 0; i++) {
        if (__str[i] != '%') {
            __put_char(__str[i]);
        } else {
            i++;
            switch (__str[i]) {
            case 'd': {
                int  tmp = va_arg(ap, int);
                char buffer[20];
                int  index = 0;
                if (tmp < 0) {
                    __put_char('-');
                    tmp = -tmp;
                }
                do {
                    buffer[index++] = tmp % 10 + '0';
                    tmp /= 10;
                } while (tmp != 0);
                for (int i = index - 1; i >= 0; i--) {
                    __put_char(buffer[i]);
                }
                break;
            }
            case 's': {
                char* __s = va_arg(ap, char*);
                for (int j = 0; __s[j] != 0; j++) {
                    __put_char(__s[j]);
                }
                break;
            }
            default:
                return EOF;
            }
        }
    }
    va_end(ap);
    return 0;
}
int USART1_GetBuffer(char* dest, int len)
{
    if (global_usart1.over_label != OVER || len < global_usart1.len)
        return EOF;
    int ptr = global_usart1.len;
    for (; ptr != -1; ptr--)
        dest[ptr] = global_usart1.buf[ptr];
    global_usart1.len        = 0;
    global_usart1.over_label = WAITING;
    return 0;
}

int USART1_GetState(int print_option)
{
    if (print_option)
        USART1_Printf("LOG: USART1_LEN=%d, CAP=%d, STATE=%d\n",
                      global_usart1.len, global_usart1.cap,
                      global_usart1.over_label);
    return global_usart1.over_label;
}

int USART1_GetBufferLen() { return global_usart1.len; }

void USART1_IRQHandler()
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == RESET ||
        !check_usart1_cache_safe() || global_usart1.over_label == OVER)
        goto usart1_ret;
    u8 read = USART_ReceiveData(USART1);
    if (read == PACKAGE_HEAD && global_usart1.over_label != READING) {
        global_usart1.over_label = READING;
        goto usart1_ret;
    }
    if (global_usart1.over_label != READING)
        goto usart1_ret;

    if (read == PACKAGE_ESCAPE && !global_usart1.escape_label) {
        global_usart1.escape_label = 1;
        goto usart1_ret;
    }
    if (read == PACKAGE_END && !global_usart1.escape_label) {
        global_usart1.over_label             = OVER;
        global_usart1.buf[global_usart1.len] = '\0';
        goto usart1_ret;
    }
    global_usart1.buf[global_usart1.len++] = read;
    global_usart1.escape_label             = 0;
usart1_ret:
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    return;
}

static int __put_char(char ch)
{
    USART_SendData(USART1, (uint8_t) ch);

    // 等待发送完成
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;

    // 检查是否发生传输错误
    if (USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET ||
        USART_GetFlagStatus(USART1, USART_FLAG_NE) != RESET ||
        USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET) {
        return EOF; // 如果发生传输错误，返回EOF表示错误
    }
    return 0;
}
