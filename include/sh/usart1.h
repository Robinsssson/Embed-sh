#ifndef __USART1__H__
#define __USART1__H__
#include <stdint.h>
#define PACKAGE_HEAD '@'
#define PACKAGE_END '#'
#define PACKAGE_ESCAPE '/'
#define CAP 200
typedef enum __label_state {
    READING,
    OVER,
    WAITING,
} label_state;
typedef struct __usart1_cache_buf {
    char buf[200];
    int  cap;
    int  len;
    int  over_label;
    int  escape_label;
} usart1_cache_buf;
typedef struct __usart1_cache_buf usart1_cache_buf;

void sh_usart_init(uint32_t baudrate);
int  USART1_Printf(const char* __str, ...);
int USART1_GetState(int);
int USART1_GetBuffer(char* dest, int len);
int USART1_GetBufferLen();
#endif //!__USART1__H__