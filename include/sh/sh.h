#ifndef __SH__H__
#define __SH__H__
#include <stdlib.h>

#include "sh/sh_map.h"
extern char global_sh_read_buf[];

void sh_get_script(char *);
void sh_ansys(char *);
void sh_register(sh_func);
#endif  //!__SH__H__
