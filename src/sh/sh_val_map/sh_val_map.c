#include <sh/sh_val_map.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sh_val_map* create_sh_val_map(int capacity)
{
    sh_val_map* map = malloc(sizeof(sh_val_map));
    if (map == NULL) {
        return NULL;
    }
    map->capacity = capacity;
    map->pairs    = malloc(sizeof(key_val_pair) * capacity);
    if (map->pairs == NULL) {
        free(map);
        return NULL;
    }
    map->num_pairs = 0;
    return map;
}

void destroy_sh_val_map(sh_val_map* map)
{
    for (int i = 0; i < map->num_pairs; i++) {
        free(map->pairs[i].key);
        free(map->pairs[i].value);
    }
    free(map->pairs);
    free(map);
}

void add_key_value_pair(sh_val_map* map, const char* key, char* value)
{
    if (map->num_pairs == map->capacity) {
        // Resize the array
        map->capacity *= 2;
        map->pairs = realloc(map->pairs, sizeof(key_val_pair) * map->capacity);
        if (map->pairs == NULL) {
            exit(1);
        }
    }
    key_val_pair* get = find_key_value_pair(map, key);
    if (get != NULL) {
        free(get->value);
        get->value = malloc(strlen((char*) value) + 1);
        strcpy((char*) get->value, (char*) value);
    } else {
        map->pairs[map->num_pairs].key = malloc(strlen(key) + 1);
        strcpy(map->pairs[map->num_pairs].key, key);
        map->pairs[map->num_pairs].value = malloc(strlen((char*) value) + 1);
        strcpy((char*) map->pairs[map->num_pairs].value, (char*) value);
        map->num_pairs++;
    }
}

void remove_key_value_pair(sh_val_map* map, const char* key)
{
    for (int i = 0; i < map->num_pairs; i++) {
        if (strcmp(map->pairs[i].key, key) == 0) {
            free(map->pairs[i].key);
            free(map->pairs[i].value);
            for (int j = i; j < map->num_pairs - 1; j++) {
                map->pairs[j] = map->pairs[j + 1];
            }
            map->num_pairs--;
            return;
        }
    }
}

key_val_pair* find_key_value_pair(sh_val_map* map, const char* key)
{
    for (int i = 0; i < map->num_pairs; i++) {
        if (strcmp(map->pairs[i].key, key) == 0) {
            return &map->pairs[i];
        }
    }
    return NULL;
}