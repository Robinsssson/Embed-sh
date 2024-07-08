#include <sh/sh_string.h>
#include <stdbool.h>
#include <stdio.h>

int sh_strlen(const char* str)
{
    int i = 0;
    while (str[i]) {
        i++;
    }
    return i;
}

int sh_strcmp(const char* s1, const char* s2)
{
    int l1 = sh_strlen(s1), l2 = sh_strlen(s2);
    if (l1 != l2)
        return 0;
    while (l1--) {
        if (s1[l1] != s2[l1])
            return 0;
    }
    return 1;
}

int sh_strcpy(char* dest, const char* src)
{
    int len = sh_strlen(src);
    for (int i = 0; i <= len; i++)
        dest[i] = src[i];
    return 0;
}

int sh_strcpy_with_len(char* dest, const char* src, int len)
{
    if (len > sh_strlen(src))
        return sh_strcpy(dest, src);
    for (int i = 0; i < len; i++)
        dest[i] = src[i];
    dest[len] = '\0';
    return 0;
}

int sh_strslice(char** dest, const char* src, char slice,
                int max_segment_length, int max_segments)
{
    int start = 0, end = 0, ptr = 0;
    int len = sh_strlen(src);

    while (end <= len && ptr < max_segments) {
        if (src[end] == slice || src[end] == '\0') {
            int segment_len = end - start;
            if (segment_len < max_segment_length) {
                sh_strcpy_with_len(dest[ptr++], src + start, segment_len);
            } else {
                printf("Segment length exceeds buffer size\n");
                return -1;
            }
            start = end + 1;
        }
        end++;
    }
    return ptr;
}