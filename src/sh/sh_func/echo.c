#include <sh/func/echo.h>
#include <sh/sh.h>
#include <sh/sh_map.h>
#include <sh/sh_string.h>
#include <sh/usart1.h>

// echo
// "hello
// world"
int sh_echo(int argc, char** argv)
{
    if (argc <= 0 || argv == NULL) {
        return SH_RET_ERROR; // 错误处理，参数无效
    }
    for (int i = 0; i < argc; i++) {
        USART1_Printf("%s", argv[i]);
        if (i != argc - 1)
            USART1_Printf(" ");
        else
            USART1_Printf("\n");
    }

    return SH_RET_NORMAL;
}
