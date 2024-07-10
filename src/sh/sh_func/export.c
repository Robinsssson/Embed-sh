#include "sh/sh_string.h"
#include "sh/usart1.h"
#include <sh/func/export.h>
#include <sh/sh_val_map.h>

extern sh_val_map* global_sh_val_map;

// example: export PATH=$PATH:/usr/local/bin
// example: export PATH=/usr/local/bin
// example: export PATH
// the function just parse string like PATH=$PATH:/usr/local/bin PATH=/usr/local/bin
// to add val /usr/local/bin in global_sh_val_map
int sh_export(char *ansys_string) {
    int equal_ptr = sh_findch(ansys_string, '=');
    if (equal_ptr == -1)
        return -1;
    ansys_string[equal_ptr] = '\0';
    char *val = ansys_string + equal_ptr + 1;
    add_key_value_pair(global_sh_val_map, ansys_string, val);
    USART1_Printf("export %s=%s\n", ansys_string, val);
    return 0;
}