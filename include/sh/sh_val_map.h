#ifndef __SH_VAL_MAP__H__
#define __SH_VAL_MAP__H__
typedef struct {
    char* key;
    char* value;
} key_val_pair;

typedef struct {
    key_val_pair* pairs;
    int           num_pairs;
    int           capacity;
} sh_val_map;

sh_val_map*   create_sh_val_map(int capacity);
key_val_pair* find_key_value_pair(sh_val_map* map, const char* key);

void destroy_sh_val_map(sh_val_map* map);
void add_key_value_pair(sh_val_map* map, const char* key, char* value);
void remove_key_value_pair(sh_val_map* map, const char* key);

#endif //!__SH_VAL_MAP__H__