#include "sh/sh.h"
#include <sh/sh_map.h>
#include <sh/sh_string.h>
#include <sh/usart1.h>
#include <stdio.h>

sh_map*    global_sh_map = NULL;
static int _static_key   = 0;

void sh_get_script(char* buf_ptr)
{
    int max_segment_length = 20;
    int max_segments       = 20;

    char* dest[max_segments];
    for (int i = 0; i < max_segments; i++) {
        dest[i] = (char*) malloc(max_segment_length * sizeof(char));
    }
    int slice_count =
        sh_strslice(dest, buf_ptr, ' ', max_segment_length, max_segments);
    sh_ansys(slice_count, dest);
    for (int i = 0; i < max_segments; i++) {
        free(dest[i]);
    }
}
void sh_ansys(int number, char** ansys_string)
{
    sh_map_get_val(global_sh_map, ansys_string[0])(number - 1,
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