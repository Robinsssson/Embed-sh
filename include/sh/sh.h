#ifndef __SH__H__
#define __SH__H__

#include "sh/sh_map.h"

#define SH_READ_BUF_SIZE 64
enum sh_ret { SH_RET_NORMAL, SH_RET_NOT_RUNNING = -2, SH_RET_ERROR = -1 };
int  sh_get_stdin(int opt);
int  sh_get_script(char*);
int  sh_ansys(char* function_string, char* ansys_string);
void sh_register(char*, sh_func);
int  sh_is_val(char* str);
void sh_ret_code_processing(int ret);
void set_code_process_level(int val);
#endif //!__SH__H__
