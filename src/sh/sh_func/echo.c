#include <sh/func/echo.h>
#include <sh/sh.h>
#include <sh/sh_map.h>
#include <sh/sh_string.h>
#include <sh/sh_val_map.h>
#include <sh/usart1.h>
// echo
// "hello
// world"

extern sh_val_map* global_sh_val_map;

/** @brief string echo
 *  @param ansys_string
 *  @return SH_RET_NORMAL
 */
int sh_echo(char* ansys_string)
{
    int ptr_of_val = sh_findch(ansys_string, '$');
    if (ptr_of_val != -1) {
        for (int i = 0; i < global_sh_val_map->num_pairs; i++) {
            if (sh_findstr(ansys_string + 1, global_sh_val_map->pairs[i].key) ==
                0) {
                int leng_of_key = sh_strlen(global_sh_val_map->pairs[i].key);
                sh_str_replace(ansys_string, global_sh_val_map->pairs[i].value,
                               SH_READ_BUF_SIZE, ptr_of_val, leng_of_key + 1);
            }
        }
    }
    USART1_Printf("%s\n", ansys_string);
    return SH_RET_NORMAL;
}
