//
//  main.c
//  8
//
//  Created by Лена on 19.10.2023.
//

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

enum status_codes
{
    fsc_ok,
    fsc_overflow,
    fsc_invalid_parameter,
    fsc_unknown,
    fsc_memory_error_detected,
};

int char_to_int (char c)
{
    if (isdigit(c))
        return (int)(c - '0');
    
    if (isalpha(c))
        if (isupper(c))
            return (int)(c - 'A' + 10);
            
    return (int)(c - 'a' + 10);
}

char int_to_char (int n)
{
    if (n > 9)
        return (char)(n - 10 + 'A');
    return n + '0';
}

char* reverse(const char* str, char* reversed_str)
{
    size_t len = strlen(str);
    
    if (str != reversed_str)
        reversed_str = strcpy(reversed_str, str);
    
    for (size_t i = 0; i < len / 2; i++)
    {
        char tmp = reversed_str[i];
        reversed_str[i] = reversed_str[len - i - 1];
        reversed_str[len - i - 1] = tmp;
    }
    return reversed_str;
}


enum status_codes _sum(char** res, int* len_res, char* summand, int base)
{
    unsigned long len_summand = strlen(summand);
    int i = 0;
    int sum = 0;
    int sum_one = 0;
    int sum_ten = 0;
    
    while (i < len_summand || sum_ten != 0)
    {
        if (i == (*len_res - 1))
        {
            int new_size = (*len_res - 1) * 2 + 1;
            *res = realloc(*res, new_size);
            if (*res == NULL)
                return fsc_memory_error_detected;
            memset(&(*res)[i], '0', (*len_res - 1));
            *len_res = new_size;
            (*res)[(*len_res) - 1] = 0;
        }
        else
        {
            if (i < len_summand)
                sum = char_to_int((*res)[i]) + char_to_int(summand[i]) + sum_ten;
            else
            {
                sum = char_to_int((*res)[i]) + sum_ten;
            }
            sum_one = sum % base;
            sum_ten = sum / base;
            
            (*res)[i] = int_to_char(sum_one);
            ++i;
        }
    }

    return fsc_ok;
}

enum status_codes sum(char** reversed_res, int base, int count, ...)
{
    if (base <= 1 || base > 36)
        return fsc_invalid_parameter;
    va_list ptr;
    va_start(ptr, count);
    
    char* res = va_arg(ptr, char*);
    int len_res = strlen(res) + 1;
    
    --count;
    
    *reversed_res = malloc(sizeof(char) * len_res);
    if (*reversed_res == NULL)
        return fsc_memory_error_detected;
    *reversed_res = reverse(res, *reversed_res);
    
    char* reversed_a = NULL;
    for (int i = 0; i < count; ++i)
    {
        char* a = va_arg(ptr, char*);
        reversed_a = realloc(reversed_a, sizeof(char) * (strlen(a) + 1));
        if (reversed_a == NULL)
        {
            free(*reversed_res);
            return fsc_memory_error_detected;
        }
        
        reversed_a = reverse(a, reversed_a);
        
        enum status_codes _sum_status = _sum(reversed_res, &len_res, reversed_a, base);
        if (_sum_status != fsc_ok)
        {
            free(*reversed_res);
            free(reversed_a);
            return _sum_status;
        }
    }
    free(reversed_a);
    return fsc_ok;
}

int main(int argc, const char * argv[])
{
    enum status_codes function_result = fsc_unknown;
    char* reversed_res;
    
    function_result =  sum(&reversed_res, 10, 3, "1234", "34", "11111");
    
    if (function_result == fsc_ok)
    {
        reversed_res = reverse(reversed_res, reversed_res);
        char* res_sum = reversed_res;
        while (*res_sum == '0')
            ++res_sum;
        printf("%s\n", res_sum);
        free(reversed_res);
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

