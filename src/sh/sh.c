#include "sh/sh.h"
#include "stm32f10x.h"
#include <sh/sh_map.h>
#include <sh/sh_string.h>
#include <sh/usart1.h>

extern usart1_cache_buf global_usart1;
sh_map*                 global_sh_map = NULL;
static int              _static_key   = 0;
#define MAX_SEGMENT_LEN 15
#define MAX_SEGMENT 6

int        code_process_level;
const char constant_pools[][30] = {"the shell is normal running\r\n",
                                   "the shell is not runnning\r\n",
                                   "the shell ret erroring\r\n"};

int  sh_is_val(char* str) { return 0; }
void set_code_process_level(int val) { code_process_level = val; }

int sh_get_stdin(int opt)
{
    if (USART1_GetState(opt) == OVER) {
        char dest[80];
        USART1_GetBuffer(dest, 80);
        return sh_get_script(dest);
    }
    return SH_RET_NOT_RUNNING;
}
int sh_get_script(char* buf_ptr)
{

    char* dest[MAX_SEGMENT];
    for (int i = 0; i < MAX_SEGMENT; i++) {
        dest[i] = (char*) malloc(MAX_SEGMENT_LEN * sizeof(char));
    }
    int slice_count =
        sh_strslice(dest, buf_ptr, ' ', MAX_SEGMENT_LEN, MAX_SEGMENT);
    int ret = sh_ansys(slice_count, dest);
    for (int i = 0; i < MAX_SEGMENT; i++) {
        free(dest[i]);
    }
    return ret;
}
int sh_ansys(int number, char** ansys_string)
{
    return sh_map_get_val(global_sh_map, ansys_string[0])(number - 1,
                                                          ansys_string + 1);
}
void sh_register(char* script, sh_func function_ptr)
{
    if (global_sh_map == NULL) {
        global_sh_map = sh_map_create();
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