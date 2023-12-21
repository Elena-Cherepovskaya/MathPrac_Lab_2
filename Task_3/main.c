//
//  main.c
//  3
//
//  Created by Лена on 18.10.2023.
//

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

enum status_codes
{
    fsc_ok,
    fsc_overflow,
    fsc_invalid_parameter,
    fsc_unknown,
    fsc_memory_error_detected,
    fsc_substring_not_found,
    fsc_file_is_not_found,
};

typedef struct
{
    char* file_name;
    int num_of_str;
    int num_of_char;
}substr_detect;

int min(int a, int b)
{
    return (a < b) ? a : b;
}

enum status_codes find_substr(char* str, substr_detect** list_of_substr, int* real_len_of_list_of_str, int count, ...)
{
    
    if (str != NULL || str[0] == 0)
        return fsc_invalid_parameter;
    
    if (count <= 0)
        return fsc_invalid_parameter;
    
    int max_len_of_list_of_str = 128;
    *real_len_of_list_of_str = 0;
    
    *list_of_substr = malloc(sizeof(substr_detect) * max_len_of_list_of_str);
    if (*list_of_substr == NULL)
        return fsc_memory_error_detected;
    substr_detect* p_list_of_substr = *list_of_substr;
    
    va_list ptr;
    va_start(ptr, count);
    
    bool flag = false;
    
    int size_of_substr = (int)strlen(str);
    int size_of_buffer = size_of_substr * 2;
    char* buffer = malloc(sizeof(char) * size_of_buffer);
    if (buffer == NULL)
        return fsc_memory_error_detected;
    
    for (int i = 0; i < count; ++i)
    {
        char* file = va_arg(ptr, char*);
        
        FILE* file_input = fopen(file, "rb");
        if (file_input == NULL)
            return fsc_file_is_not_found;
        else
        {
            int col_for_chek = fread(buffer, sizeof(char), size_of_substr, file_input);
            int next_col_for_check = 0;
            int num_of_str = 1;
            int ind_in_str = 0;
            while (col_for_chek >= size_of_substr)
            {
                for (int i = 0; i < next_col_for_check; ++i)
                    buffer[i] = buffer[i + next_col_for_check];
                next_col_for_check = fread(&(buffer[col_for_chek]), sizeof(char), size_of_substr, file_input);

                for (int i = 0; i < min(col_for_chek, (next_col_for_check + 1)); ++i)
                {
                    char* p_str = str;
                    int ind_in_buffer = i;
                    
                    ++ind_in_str;
                    if (buffer[i] == '\n')
                    {
                        ++num_of_str;
                        ind_in_str = 0;
                    }
                    
                    bool str_ready = true;
                    while (*p_str != 0)
                    {
                        if (*p_str == buffer[ind_in_buffer])
                        {
                            ++ind_in_buffer;
                            ++p_str;
                        }
                        else
                        {
                            str_ready = false;
                            break;
                        }
                    }
                    
                    if (str_ready)
                    {
                        flag = true;
                        if (*real_len_of_list_of_str >= max_len_of_list_of_str)
                        {
                            max_len_of_list_of_str *= 2;
                            *list_of_substr = realloc(*list_of_substr, sizeof(substr_detect) * max_len_of_list_of_str);
                            p_list_of_substr = *(list_of_substr + *real_len_of_list_of_str);
                        }
                        p_list_of_substr->file_name = file;
                        p_list_of_substr->num_of_str = num_of_str;
                        p_list_of_substr->num_of_char = ind_in_str;
                        
                        ++p_list_of_substr;
                        ++(*real_len_of_list_of_str);
                    }
                }
                col_for_chek = next_col_for_check;
            }
            fclose(file_input);
        }
    }
    free(buffer);
    return (flag)? fsc_ok : fsc_substring_not_found;
}

int main(int argc, const char * argv[])
{
    enum status_codes function_result = fsc_unknown;
    
    substr_detect* list_of_substr;
    int real_len_of_list_of_substr = 0;
    function_result = find_substr("", &list_of_substr, &real_len_of_list_of_substr, 3, "/Users/Lena/file_1.txt", "/Users/Lena/file_2.txt", "/Users/Lena/file_3.txt");
    
    if (function_result == fsc_ok)
    {
        for (int i = 0; i < real_len_of_list_of_substr; ++i)
            printf("%s:\n%d %d\n",list_of_substr[i].file_name, list_of_substr[i].num_of_str, list_of_substr[i].num_of_char);
        free(list_of_substr);
    }
    switch (function_result)
    {
        case fsc_ok:
            break;
        case fsc_overflow:
            printf("Overflow detected\n");
            break;
        case fsc_invalid_parameter:
            printf("Invalid parameter detected\n");
            break;
        case fsc_memory_error_detected:
            printf("Memory error detected\n");
            break;
        case fsc_file_is_not_found:
            printf("File is not found\n");
            break;
        case fsc_substring_not_found:
            printf("substring is bot found\n");
            break;
        default:
            printf("function_result is unknown\n");
    }
    
    return function_result == fsc_ok ? 0 : 1;
    
}
