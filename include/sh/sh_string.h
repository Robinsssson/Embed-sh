#ifndef __SH_STRING__H__
#define __SH_STRING__H__
int sh_strlen(const char*);
int sh_strcmp(const char*, const char*);
int sh_strcpy(char* dest, const char* src);
int sh_findstr(const char*, const char*);
int sh_findch(const char* dest, const char ch);
int sh_strcpy_with_len(char* dest, const char* src, int len);
int sh_strslice(char** dest, const char* src, char slice,
                int max_segment_length, int max_segments);
#endif //!__SH_STRING__H__