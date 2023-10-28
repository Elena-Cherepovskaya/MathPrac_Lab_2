//
//  main.c
//  1
//
//  Created by Лена on 14.10.2023.
//

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

enum status_codes
{
    fsc_ok,
    fsc_overflow,
    fsc_invalid_parameter,
    fsc_unknown,
    fsc_memory_error_detected,
};

size_t str_len(const char* str)
{
    int len = 0;
    while(*str != 0)
    {
        ++len;
        ++str;
    }
    return len;
}

char* str_copy(const char* str, char* new_str)
{
    char const* p_str = str;
    char* p_new_str = new_str;
    while(*p_str != 0)
    {
        *p_new_str = *p_str;
        ++p_new_str;
        ++p_str;
    }
    *p_new_str = 0;
    return new_str;
}

char* reverse(const char* str, char* new_str)
{
    size_t len = str_len(str);
    char* reversed_str = str_copy(str, new_str);
    for (size_t i = 0, j = len - 1; i < j; i++, j--)
    {
        char tmp = reversed_str[i];
        reversed_str[i] = reversed_str[j];
        reversed_str[j] = tmp;
    }
    return reversed_str;
}

enum status_codes flag_u(const char* str, char** new_str)
{
    size_t len = str_len(str);
    *new_str = malloc(sizeof(char) * (len + 1));
    
    if (*new_str == NULL)
        return fsc_memory_error_detected;
    
    for (int i = 0; i < len; ++i)
    {
        if (isalpha(str[i]) && i % 2 == 0)
            (*new_str)[i] = toupper(str[i]);
        else
            (*new_str)[i] = str[i];
    }
    (*new_str)[len] = 0;
    return fsc_ok;
}

enum status_codes flag_n(const char* str, char** new_str)
{
    size_t len = str_len(str);
    *new_str = malloc(sizeof(char) * (len + 1));
    if (*new_str == NULL)
        return fsc_memory_error_detected;
    
    
    char* p_str = *new_str;
    for (int i = 0; i < 3; ++i)
    {
        char* ptr = str;
        while (*ptr != 0)
        {
            switch (i)
            {
                case 0:
                    if (isalpha(*ptr))
                    {
                        *p_str = *ptr;
                        ++p_str;
                    }
                    break;
                case 1:
                    if (isnumber(*ptr))
                    {
                        *p_str = *ptr;
                        ++p_str;
                    }
                case 2:
                    if (!isalpha(*ptr) && !isnumber(*ptr))
                    {
                        *p_str = *ptr;
                        ++p_str;
                    }
                    break;
                default:
                    break;
            }
            ++ptr;
        }
    }
    *p_str = 0;
    return fsc_ok;
}

enum status_codes flag_c(unsigned seed, int len_list_num_str, const char *argv[], char **str)
{
    unsigned int list_num_str[len_list_num_str];
    for (int i = 0; i < len_list_num_str; ++i)
        list_num_str[i] = i;
    
    srand(seed);
    int tmp = (len_list_num_str) / 2;
    for (int i = 0; i < tmp; ++i)
    {
        int ind_1 = rand() % len_list_num_str;
        int ind_2 = rand() % len_list_num_str;
        
        int c = list_num_str[ind_1];
        list_num_str[ind_1] = list_num_str[ind_2];
        list_num_str[ind_2] = c;
    }
    
    int size_new_str = 0;
    for (int i = 0; i < len_list_num_str; ++i)
        size_new_str += str_len(argv[i]);
    
    char* p_str = malloc(sizeof(char) * (size_new_str + 1));
    if (p_str == NULL)
        return fsc_memory_error_detected;
    
    *str = p_str;
    for (int i = 0; i < len_list_num_str; ++i)
    {
        char const * p_tmp = argv[list_num_str[i]];
        while(*p_tmp != 0)
        {
            *p_str = *p_tmp;
            ++p_str;
            ++p_tmp;
        }
    }
    *p_str = 0;
    return fsc_ok;
}

int main(int argc, const char * argv[])
{
    enum status_codes function_result = fsc_unknown;
    function_result = (argc >= 3) ? fsc_ok : fsc_invalid_parameter;
    
    if (function_result == fsc_ok)
    {
        char flag = argv[1][1];
        
        switch (flag)
        {
            case 'l':
            {
                printf("lenth of str: %zu\n", str_len(argv[2]));
                break;
            }
            case 'r':
            {
                char* new_str = malloc(sizeof(char) * (str_len(argv[2]) + 1));
                if (new_str == NULL)
                {
                    function_result = fsc_memory_error_detected;
                    break;
                }
                new_str = str_copy(argv[2], new_str);
                printf("Revesed str: %s\n",reverse(argv[2],new_str));
                free(new_str);
                break;
            }
            case 'u':
            {
                char* new_str;
                function_result = flag_u(argv[2], &new_str);
                if (function_result == fsc_ok)
                {
                    printf("%s\n", new_str);
                    free(new_str);
                }
                break;
            }
            case 'n':
            {
                char* new_str;
                function_result = flag_n(argv[2], &new_str);
                if (function_result == fsc_ok)
                {
                    printf("%s\n", new_str);
                    free(new_str);
                }
            }
            case 'c':
            {
                if (argc >= 4)
                {
                    unsigned int seed = atoi(argv[2]);
                    int len_list_num_str = argc - 3;
                    char *res_str;
                    function_result = flag_c(seed, len_list_num_str, argv+3, &res_str);
                    if (function_result == fsc_ok)
                    {
                        printf("New str: %s\n", res_str);
                        free(res_str);
                    }
                }
                else
                    function_result = fsc_invalid_parameter;
                break;
            }
            default:
                break;
        }
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
        default:
            printf("function_result is unknown\n");
    }
    
    return function_result == fsc_ok ? 0 : 1;
}
