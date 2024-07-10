#include "stm32f10x.h"
#include <sh/sh_string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @brief Get length of string
 * @param str String
 * @return Length of string
 */
int sh_strlen(const char* str)
{
    int i = 0;
    while (str[i]) {
        i++;
    }
    return i;
}
/**
 * @brief Compare two strings
 * @param s1 First string
 * @param s2 Second string
 * @return 0 if strings are equal, -1 otherwise
 */
int sh_strcmp(const char* s1, const char* s2)
{
    int l1 = sh_strlen(s1), l2 = sh_strlen(s2);
    if (l1 != l2)
        return -1;
    while (l1--) {
        if (s1[l1] != s2[l1])
            return -1;
    }
    return 0;
}

/** @brief Copy string to another string
 * @param dest Destination string
 * @param src Source string
 * @return 0 if success, -1 otherwise
 */
int sh_strcpy(char* dest, const char* src)
{
    int len = sh_strlen(src);
    for (int i = 0; i <= len; i++) {
        dest[i] = src[i];
    }
    return 0;
}
/** @brief Copy string to another string with length
 * @param dest Destination string
 * @param src Source string
 * @param len Length of source string
 * @return 0 if success, -1 otherwise
 */
int sh_strcpy_with_len(char* dest, const char* src, int len)
{
    if (len > sh_strlen(src))
        return sh_strcpy(dest, src);
    for (int i = 0; i < len; i++) {
        dest[i] = src[i];
    }
    dest[len] = '\0';
    return 0;
}
/** @brief Split string into segments
 * @param dest Destination array of strings
 * @param src Source string
 * @param slice Character to split by
 * @param max_segment_length Maximum length of segment
 * @param max_segments Maximum number of segments
 * @return Number of segments
 */
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
/** @brief Find character in string
 * @param dest Destination string
 * @param ch Character to find
 * @return Position of character, -1 if not found
 */
int sh_findch(const char* dest, const char ch)
{
    for (int i = 0; dest[i] != '\0'; i++) {
        if (dest[i] == ch)
            return i;
    }
    return -1;
}
/** @brief Find string in string
 * @param dest Destination string
 * @param str String to find
 * @return Position of string, -1 if not found
 */
int sh_findstr(const char* dest, const char* str)
{
    int l1 = sh_strlen(dest), l2 = sh_strlen(str);
    if (l1 < l2)
        return -1;

    for (int i = 0; i <= l1 - l2; i++) {
        int ptr;
        for (ptr = 0; ptr < l2; ptr++) {
            if (dest[i + ptr] != str[ptr])
                break;
        }
        if (ptr == l2)
            return i;
    }

    return -1;
}

/** @brief Insert string into another string
 *  @param dest Destination string
 *  @param src Source string
 *  @param dest_max_len Maximum length of destination string
 *  @param pos Position to insert
 *  @return 0 if success, -1 otherwise
 */
int sh_str_insert(char* dest, const char* src, int dest_max_len, int pos)
{
    int dest_len = strlen(dest);
    int src_len  = strlen(src);
    int new_len  = dest_len + src_len;

    // 检查是否有足够的空间
    if (new_len >= dest_max_len) {
        return -1; // Not enough space in the destination buffer
    }

    // 检查位置是否有效
    if (pos < 0 || pos > dest_len) {
        return -1; // Invalid position
    }

    // 移动插入点后的内容
    memmove(dest + pos + src_len, dest + pos, dest_len - pos + 1);
    // 将 src 字符串复制到 dest 字符串
    memcpy(dest + pos, src, src_len);

    return 0;
}

/** @brief Replace a substring in a string
 *  @param dest Destination string
 *  @param src Source string
 *  @param dest_len Maximum length of destination string
 *  @param pos Position to replace
 *  @param len Length of substring to replace
 *  @return 0 if success, -1 otherwise
 */
int sh_str_replace(char* dest, const char* src, int dest_len, int pos, int len)
{
    int src_len     = strlen(src);
    int current_len = strlen(dest);

    // 检查位置和长度是否有效
    if (pos + len > current_len || pos < 0 || len < 0) {
        return -1; // Invalid position or length
    }

    // 计算替换后的新长度
    int new_len = current_len - len + src_len;
    if (new_len >= dest_len) {
        return -1; // Not enough space in the destination buffer
    }

    // 移动替换后的内容
    memmove(dest + pos + src_len, dest + pos + len,
            current_len - pos - len + 1);
    // 复制src字符串到dest字符串
    memcpy(dest + pos, src, src_len);

    return 0;
}
