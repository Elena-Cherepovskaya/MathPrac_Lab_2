//
//  main.c
//  6
//
//  Created by Лена on 09.02.2024.
//

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include<sys/mman.h>

enum status_codes
{
    fsc_ok,
    fsc_overflow,
    fsc_invalid_parameter,
    fsc_memory_error_detected,
    fsc_file_is_not_found,
};

enum state_machine
{
    synchr_flag_str,
    find_flag_begin,
    read_flag,
    apply_flag,
    apply_custom_flag,
    skip_scanf_data,
};

bool is_custom_flag(char** custom_flag_list, char* test_flag)
{
    for (int i = 0; i < 4; ++i)
    {
        if (strcmp(test_flag, custom_flag_list[i]) == 0)
            return true;
    }
    return false;
}

int num_of_flag(char** custom_flag_list, char* test_flag)
{
    for (int i = 0; i < 4; ++i)
        if (strcmp(test_flag, custom_flag_list[i]) == 0)
            return i;
    return 4;
}

int is_roman(char c)
{
    char* roman_list = "IVXLCDM";
    char* p = roman_list;
    
    switch (c)
    {
        case 'I':
            return 1;
        case 'V':
            return 5;
        case 'X':
            return 10;
        case 'L':
            return 50;
        case 'C':
            return 100;
        case 'D':
            return 500;
        case 'M':
            return 1000;
        default:
            return 0;
    }
}

int char_to_num(char c)
{
    if (isalpha(c))
        return (isupper(c)) ? c - 55 : c - 87;
    return c - '0';
}

enum status_codes transform_x_to_10(uint64_t* res, char* num, int base)
{
    if (*res != 0 || num == NULL)
        return fsc_invalid_parameter;
    
    if (base < 2 || base > 36)
        base = 10;
    
    int len = (int)strlen(num);
    char* p_num = &(num[len - 1]);
    int cur_pow = 0;
    while(len != 0)
    {
        int tmp = char_to_num(*p_num);
        
        if (tmp >= base)
            return fsc_invalid_parameter;
        
        *res += (tmp * pow(base, cur_pow));
        --len;
        --p_num;
        ++cur_pow;
    }
    
    return fsc_ok;
}

int overvsscanf(const char * str, const char * p_format, va_list param)
{
    int count  = 0;
    enum status_codes res = fsc_ok;
    
    char const * const custom_flags_list[] = {"%Ro", "%Zr", "%Cv", "%CV"};
    
    char flag_buf[BUFSIZ];

    int flag_index = 0;
    enum state_machine state = synchr_flag_str;
    
    char buffer[BUFSIZ];
    char* p_buffer = buffer;
    
    bool read_roman_value = false;
    bool read_zr = false;
    bool read_address = false;
    bool is_address_ready = false;
    bool read_base = false;
    bool transform_address = false;
    bool is_capital_letters = false;
    
    int res_zr = 0;
    int cur_fib = 1;
    int prev_fib = 0;
    
    int base = 0;
    
    while (((*str != 0) &&
           (*p_format != 0)) ||
           (!(state == synchr_flag_str)))
    {
        switch (state)
        {
            case synchr_flag_str:
                if (*p_format == *str)
                {
                    ++p_format;
                    ++str;
                }
                else
                    state = find_flag_begin;
                break;
            
            case find_flag_begin:
                if (*p_format != '%')
                    ++p_format;
                else
                    state = read_flag;
                break;

            case read_flag:
                if (*p_format > 32)
                {
                    flag_buf[flag_index] = *p_format;
                    ++p_format;
                    ++flag_index;
                }
                else
                {
                    flag_buf[flag_index] = 0;
                    state = apply_flag;
                }
                break;
                
            case apply_flag:
                if (!(is_custom_flag(custom_flags_list, flag_buf)))
                {
                    vsscanf(str, flag_buf, param);
                    state = skip_scanf_data;
                    ++count;
                }
                else
                {
                    state = apply_custom_flag;
                    switch (num_of_flag(custom_flags_list, flag_buf))
                    {
                        case 0:
                            read_roman_value = true;
                            break;
                        case 1:
                            read_zr = true;
                            break;
                        case 2:
                            read_address = true;
                            is_capital_letters = false;
                            break;
                        case 3:
                            read_address = true;
                            is_capital_letters = true;
                        default:
                            break;
                    }
                    
                }
                break;
            
            case apply_custom_flag:
                if (read_roman_value)
                {
                    if (is_roman(*str))
                    {
                        *p_buffer = *str;
                        ++p_buffer;
                        ++str;
                    }
                    else
                    {
                        *p_buffer = 0;
                        p_buffer = buffer;
                        
                        int prev = is_roman(*buffer);
                        int cur = is_roman(buffer[1]);
                        ++p_buffer;
                        
                        int res_value = 0;
                        while (*p_buffer != 0)
                        {
                            if (prev < cur)
                                res_value -= prev;
                            else
                                res_value += prev;
                            
                            prev = cur;
                            ++p_buffer;
                            cur = is_roman(*p_buffer);
                        }
                        res_value += prev;
                        
                        state = skip_scanf_data;
                        read_roman_value = false;
                        
                        char str_buf[BUFSIZ];
                        snprintf(str_buf, BUFSIZ, "%d", res_value);
                        
                        vsscanf(str_buf, "%d", param);
                        ++count;
                    
                        p_buffer = buffer;
                    }
                    break;
                }
                
                if (read_zr)
                {
                    if (isnumber(*str))
                    {
                        if (*str - '0')
                        {
                            res_zr += cur_fib;
                        }
                        
                        int tmp = cur_fib;
                        cur_fib = prev_fib + tmp;
                        prev_fib = tmp;
                        ++str;
                    }
                    else
                    {
                        state = skip_scanf_data;
                        
                        char str_buf[BUFSIZ];
                        snprintf(str_buf, BUFSIZ, "%d", res_zr);
                        vsscanf(str_buf, "%d", param);
                        read_zr = false;
                        ++count;
                        
                        res_zr = 0;
                        cur_fib = 1;
                        prev_fib = 0;
                    }
                    break;
                }
                
                if (read_address)
                {
                    if (isalnum(*str))
                    {
                        *p_buffer = *str;
                        ++p_buffer;
                        ++str;
                    }
                    else
                    {
                        read_address = false;
                        *p_buffer = 0;
                        is_address_ready = true;
                    }
                }
                
                if (is_address_ready)
                {
                    if (is_capital_letters)
                    {
                        if (isnumber(*str) || isupper(*str))
                        {
                            is_address_ready = false;
                            read_base = true;
                            base = base * 10 + (*str - '0');
                        }
                    }
                    else
                    {
                        if (isnumber(*str) || islower(*str))
                        {
                            is_address_ready = false;
                            read_base = true;
                            base = base * 10 + (*str - '0');
                        }
                    }
                    ++str;
                }

                
                if (read_base)
                {
                    if (isnumber(*str))
                    {
                        base = base * 10 + (*str - '0');
                        ++str;
                    }
                    else
                    {
                        read_address = false;
                        transform_address = true;
                    }
                }
                
                if (transform_address)
                {
                    uint64_t addreas_10 = 0;
                    transform_x_to_10(&addreas_10, buffer, base);
                    
                    uint64_t* p_address_10 = (uint64_t*)addreas_10;
                    int stored_value = *p_address_10;
                    
                    state = skip_scanf_data;
                    
                    char str_buf[BUFSIZ];
                    snprintf(str_buf, BUFSIZ, "%d", stored_value);
                    vsscanf(str_buf, "%d", param);
                    ++count;
                    
                    p_buffer = buffer;
                    base = 0;
                    transform_address = false;
                    state = synchr_flag_str;
                }
                break;
            case skip_scanf_data:
                if (*str <= 32)
                {
                    state = synchr_flag_str;
                    flag_index = 0;
                }
                else
                    ++str;
                break;
                
            default:
                break;
        }
    }
    return count;
}

int overvfscanf(FILE * input, const char * p_format, va_list param)
{
    fseek(input, 0, SEEK_END);
    int file_size = (int)ftell(input);
    fseek(input, 0, SEEK_SET);
    char* str = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fileno(input), 0);
    
    return overvsscanf(str, p_format, param);
}

int oversscanf(char * str, const char * p_format,...)
{
    va_list param;
    va_start(param, p_format);
    int count = overvsscanf(str, p_format, param);
    va_end(param);
    return count;
}

int overfscanf(FILE* input, const char * p_format,...)
{
    va_list param;
    va_start(param, p_format);
    int count = overvfscanf(input, p_format, param);
    va_end(param);
    return count;
}

int addr = 139;
int main(int argc, const char * argv[])
{
    printf("%lx\n", (uint64_t *)&addr);
    int a, b, c, d;
    int count = oversscanf("XI 100101  123 100008090 16", "%Ro %Zr  %d %Cv", &a, &b, &c, &d);
    printf("%d\n%d %d %d %d\n",count, a, b, c, d);

    
    FILE* input = fopen(argv[1], "r");
    if (input != NULL)
        count = overfscanf(input, "%d   %Ro", &a, &b);
    
    printf("%d\n%d %d\n", count, a, b);
    
    if (input != NULL)
        fclose(input);
    
    return 0;
}
