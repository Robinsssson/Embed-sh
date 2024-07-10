#include "sh/sh.h"
#include "stm32f10x.h"
#include <sh/sh_map.h>
#include <sds/sds.h>
#include <sh/sh_string.h>
#include <sh/sh_val_map.h>
#include <sh/usart1.h>
#include <stdlib.h>

extern usart1_cache_buf global_usart1;

sh_map*     global_sh_map     = NULL;
sh_val_map* global_sh_val_map = NULL;
static int  _static_key       = 0;


int        code_process_level;
const char constant_pools[][30] = {"the shell is normal running\r\n",
                                   "the shell is not runnning\r\n",
                                   "the shell ret erroring\r\n"};

int  sh_is_val(char* str) { return 0; }
void set_code_process_level(int val) { code_process_level = val; }

int sh_get_stdin(int opt)
{
    if (USART1_GetState(opt) == OVER) {
        char *buf = malloc(SH_READ_BUF_SIZE);
        USART1_GetBuffer(buf, SH_READ_BUF_SIZE);
        int ret = sh_get_script(buf);
        free(buf);
        return ret;
    }
    return SH_RET_NOT_RUNNING;
}
int sh_get_script(char* buf_ptr)
{
    int   ptr_of_space         = sh_findch(buf_ptr, ' ');
    char* function_str         = buf_ptr;
    function_str[ptr_of_space] = '\0';
    char* ansys_str            = &buf_ptr[ptr_of_space + 1];
    int   ret                  = sh_ansys(function_str, ansys_str);
    return ret;
}
int sh_ansys(char* function_string, char* ansys_string)
{
    return sh_map_get_val(global_sh_map, function_string)(ansys_string);
}
void sh_register(char* script, sh_func function_ptr)
{
    if (global_sh_map == NULL) {
        global_sh_map = sh_map_create();
    }
    if (global_sh_val_map == NULL) {
        global_sh_val_map = create_sh_val_map(4);
    }
    sh_map_insert(global_sh_map, script, function_ptr);
    _static_key = global_sh_map->size;
}

void sh_ret_code_processing(int ret)
{
    switch (ret) {
    case SH_RET_NORMAL:
        if (code_process_level == SH_RET_NORMAL)
            USART1_Printf(constant_pools[0]);
        break;

    case SH_RET_NOT_RUNNING:
        if (code_process_level == SH_RET_NOT_RUNNING ||
            code_process_level == SH_RET_NORMAL)
            USART1_Printf(constant_pools[1]);
        break;

    case SH_RET_ERROR:
        USART1_Printf(constant_pools[2]);
        break;
    }
}