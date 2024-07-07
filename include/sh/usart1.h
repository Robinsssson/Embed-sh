#ifndef __USART1__H__
#define __USART1__H__
#include <stdint.h>
#define PACKAGE_HEAD = '@';
#define PACKAGE_END = '\n';
void sh_usart_init(uint32_t baudrate);

#endif  //!__USART1__H__