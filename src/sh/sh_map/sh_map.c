#include "sh/sh_map.h"
#include "stm32f10x_conf.h"
#include <assert.h>
#include <sh/sh_string.h>
#define KEY_CHAHE 10
static sh_ret sh_map_node_create(sh_map_node** p_node_arr, int size, int cap);
static int    check_map_cap(sh_map* map);

static int sh_key_same(__node_key k1, __node_key k2)
{
    return sh_strcmp(k1, k2);
}
sh_map* sh_map_create()
{
    sh_map* map = malloc(sizeof(sh_map));
    assert(map != NULL);
    map->cap  = 10;
    map->size = 0;
    if (sh_map_node_create(&map->node_arr, map->size, map->cap) != RET_OK) {
        free(map); // Add this line to prevent memory leak
        return NULL;
    }
    return map;
}

static sh_ret sh_map_node_create(sh_map_node** p_node_arr, int size, int cap)
{
    sh_map_node* node_arr = NULL;
    if (size == 0) {
        node_arr = malloc(sizeof(sh_map_node) * cap);
    } else {
        if (p_node_arr == NULL || *p_node_arr == NULL) {
            return RET_FAIL;
        }
        node_arr = realloc(*p_node_arr, sizeof(sh_map_node) * cap);
        if (node_arr == NULL) {
            return RET_FAIL;
        }
    }
    *p_node_arr = node_arr;
    return RET_OK;
}

int sh_map_insert(sh_map* map, __node_key key, __node_val sh_val)
{
    int ret = check_map_cap(map);
    if (ret != RET_OK)
        return ret;
    for (int i = 0; i < map->size; i++) {
        if (sh_key_same(map->node_arr[i].key, key)) {
            map->node_arr[i].val = sh_val;
            return RET_OK;
        }
    }
    map->node_arr[map->size].key = malloc(KEY_CHAHE * sizeof(__node_key));
    sh_strcpy(map->node_arr[map->size].key, key);
    map->node_arr[map->size].val = sh_val;
    map->size++;
    return RET_OK;
}

static int check_map_cap(sh_map* map)
{
    if (map->cap > map->size)
        return RET_OK;
    map->cap *= 2;
    return sh_map_node_create(&map->node_arr, map->size, map->cap);
}

__node_val sh_map_get_val(sh_map* map, __node_key key)
{
    for (int i = 0; i < map->size; i++) {
        if (sh_key_same(map->node_arr[i].key, key)) {
            return map->node_arr[i].val;
        }
    }
    return NULL;
}

sh_map* sh_map_destroy(sh_map* map)
{
    for (int i = 0; i < map->size; i++)
        free(map->node_arr->key);
    free(map->node_arr);
    free(map);
    return NULL;
}
