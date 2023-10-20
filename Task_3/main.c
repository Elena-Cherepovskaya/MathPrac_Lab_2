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
};

enum status_codes find_substr(char* str, int count, ...)
{
    if (count <= 0)
        return fsc_invalid_parameter;
    
    va_list ptr;
    va_start(ptr, count);
    
    bool flag = false;
    
    for (int i = 0; i < count; ++i)
    {
        char* file = va_arg(ptr, char*);
        
        FILE* file_input = fopen(file, "rb");
        if (file_input == NULL)
            return fsc_memory_error_detected;
        else
        {
            fseek(file_input, 0, SEEK_END);
            int size_of_buffer = ftell(file_input);
            char* buffer = malloc(sizeof(char) * (size_of_buffer + 1));
            memset(buffer, 0, size_of_buffer + 1);
            fseek(file_input, 0, SEEK_SET);
            fread(buffer, sizeof(char), size_of_buffer, file_input);
            fclose(file_input);
            
            int num_of_str = 1;
            int res_str = 1;
            
            int ind_in_str = 0;
            int res_ind_in_str = 0;
            for (int i = 0; i <= size_of_buffer; ++i)
            {
                //bool flag = false;
                char* p_str = str;
                int ind_buffer = i;
                
                ++ind_in_str;
                if (buffer[i] == '\n')
                {
                    ++num_of_str;
                    ind_in_str = 0;
                }
                
                bool str_ready = true;
                while (*p_str != 0)
                {
                    if (*p_str == buffer[ind_buffer])
                    {
                        ++ind_buffer;
                        ++p_str;
                    }
                    else
                    {
                        str_ready = false;
                        break;
                    }
                }
                //Отладочный вывод
                if (str_ready)
                {
                    flag = true;
                    printf("%s:\n%d %d\n",file, num_of_str, ind_in_str);
                }
            }
            free(buffer);
        }
    }
    return (flag)? fsc_ok : fsc_substring_not_found;
}

int main(int argc, const char * argv[])
{
    enum status_codes function_result = fsc_unknown;
    
    function_result = find_substr("733", 3, "/Users/lena/file_1.txt", "/Users/lena/file_2.txt", "/Users/lena/file_3.txt");

    switch (function_result)
    {
        case fsc_ok:
            break;
        case fsc_substring_not_found:
            printf("Substring is not detected");
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
        default:
            printf("function_result is unknown\n");
    }
    
    return function_result == fsc_ok ? 0 : 1;
    
}
