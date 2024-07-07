#ifndef __SH_MAP__H__
#define __SH_MAP__H__
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum __RET_CODE {
    RET_OK = 0,
    RET_FAIL = 255,
} sh_ret;
typedef int8_t (*sh_func)(int8_t argc, int8_t **argv);

typedef int16_t __node_key;
typedef sh_func __node_val;
typedef struct __sh_map_node {
    __node_key key;
    __node_val val;
} sh_map_node;

typedef struct __sh_map {
    int cap;
    int size;
    sh_map_node *node_arr;
} sh_map;

sh_map *sh_map_create();
int sh_map_insert(sh_map *map, __node_key key, __node_val sh_fun);
__node_val sh_map_get_val(sh_map *map, __node_key key);
sh_map *sh_map_destroy(sh_map *map);
#endif  //!__SH_MAP__H__