#ifndef __USART1__H__
#define __USART1__H__
#include <stdint.h>
#define PACKAGE_HEAD = '@';
#define PACKAGE_END = '\n';

typedef struct __usart1_cache_buf usart1_cache_buf;
extern usart1_cache_buf global_usart1;

void sh_usart_init(uint32_t baudrate);
int usart1_printf(const char* __str, ...);

#endif  //!__USART1__H__