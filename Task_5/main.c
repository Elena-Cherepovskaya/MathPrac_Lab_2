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
#include <math.h>

#define MAX_BUFFER_SIZE 512
#define START_CAPASITY 32

enum status_codes
{
    fsc_ok,
    fsc_overflow,
    fsc_invalid_parameter,
    fsc_memory_error_detected,
    fsc_file_is_not_found,
};

typedef struct
{
    int int_value;
    char* roman_value;
}T_roman_value;

bool is_custom_flag(char** custom_flags_list, char* test_flag)
{
    for (int i = 0; i < 10; ++i)
    {
        if (strcmp(test_flag, custom_flags_list[i]) == 0)
            return  true;
    }
    
    return false;
}

int num_of_custom_flag(char** custom_flags_list, char* test_flag)
{
    for (int i = 0; i < 10; ++i)
    {
        if (strcmp(test_flag, custom_flags_list[i]) == 0)
            return i;
    }
    return 11;
}

enum status_codes to_roman(int n, char** res)
{
    if (*res != NULL)
        return fsc_invalid_parameter;
    
    *res = (char*)malloc(sizeof(char) * START_CAPASITY);
    int capacity = START_CAPASITY;
    if (*res == NULL)
        return fsc_memory_error_detected;
    
    T_roman_value data[] = {{1,"I"}, {4,"IV"}, {5,"V"}, {9,"IV"}, {10, "X"}, {40,"XL"}, {50,"L"}, {90,"XC"}, {100,"C"}, {400,"CD"}, {500,"D"}, {1000,"M"}};
    
    int res_ind = 0;
    
    if (n <= 0)
        return fsc_invalid_parameter;
    
    while(n > 0)
    {
        int data_index = 11;
        while(data[data_index].int_value > n)
            --data_index;
        
        if (res_ind + 2 == capacity)
        {
            capacity *= 2;
            char* tmp_p = realloc(*res, capacity);
            if (tmp_p == NULL)
            {
                free(*res);
                *res = NULL;
                return fsc_memory_error_detected;
            }
            *res = tmp_p;
        }
        
        strcat(&((*res)[res_ind]), data[data_index].roman_value);
        res_ind += strlen(data[data_index].roman_value);
        n -= data[data_index].int_value;
    }
    
    return fsc_ok;
}

int prev_fib(int value)
{
    int a = 1;
    int b = 1;
    
    while(true)
    {
        int next = a + b;
        if (next > value)
            return b;
        a = b;
        b = next;
    }
}

enum status_codes zeckendorf(char* output, int value)
{
    if (value < 0)
        return fsc_invalid_parameter;
    
    if (value == 0)
        return fsc_ok;
    
    int prev_value = prev_fib(value);
    sprintf(&(output[strlen(output)]), "%d ", prev_value);
    
    return zeckendorf(output, value - prev_value);
}

double log_a_b(double a, double b)
{
    return (log(b) / log(a));
}

enum status_codes translation_10_to_x (int n, char** res, int base)
{
    if (n == 0)
    {
        *res = (char*)malloc(sizeof(char) * 2);
        if (*res == NULL)
            return fsc_memory_error_detected;
        (*res)[0] = '0';
        (*res)[1] = 0;
        return fsc_ok;
    }
    
    if (base < 2 || base > 36)
        base = 10;
    
    int len_of_n = floor(log_a_b(base, n)) + 1;
    
    if (*res != NULL)
        return fsc_invalid_parameter;
    
    *res = (char*)malloc(sizeof(char) * (len_of_n + 1));
    if (*res == NULL)
        return fsc_memory_error_detected;
    (*res)[len_of_n] = 0;
    
    while (n > 0)
    {
        len_of_n--;
        
        if (n % base >= 10)
            (*res)[len_of_n] = (n % base) - 10 + 'A';
        else
            (*res)[len_of_n] = (n % base) + '0';
        
        n /= base;
    }
    return fsc_ok;
}

int char_to_num(char c)
{
    if (isalpha(c))
    {
        if (isupper(c))
            return (int)(c - 'A' + 10);
        return (int)(c - 'a' + 10);
    }
    return (int)(c - '0');
}


enum status_codes translation_x_to_10 (char* n, int* res, int base)
{
    *res = 0;
    int tmp_n = 0;
    int pow_x = 1;
    for (int i = (int)strlen(n) - 1; i >= 0; --i)
    {
        tmp_n = char_to_num(n[i]);
        
        if (tmp_n >= base)
            return fsc_invalid_parameter;
                
        *res = *res + tmp_n * pow_x;
        pow_x *= base;
    }
    return fsc_ok;
}

int overvsprintf(char* str, char const * p_format, va_list param)
{
    int count = 0;
    enum status_codes f_res = fsc_ok;
    
    char* custom_flags_list[] = {"%Ro", "%Zr", "%Cv", "CV", "%to", "%TO", "%mi", "%mu", "%md", "%mf"};
    
    str[0] = 0;
    
    char flag_buf[MAX_BUFFER_SIZE];
    char buffer[MAX_BUFFER_SIZE];
    int flag_index = 0;
    while (*p_format != 0)
    {
        
        if (*p_format == '%')
        {
            flag_buf[0] = '%';
            flag_index = 1;
            
            ++p_format;
            while(isalpha(*p_format))
            {
                flag_buf[flag_index] = *p_format;
                ++flag_index;
                ++p_format;
            }
            flag_buf[flag_index] = 0;
            
            if (!(is_custom_flag(custom_flags_list, flag_buf)))
            {
                vsprintf(buffer, flag_buf, param);
                strcat(str, buffer);
                ++count;
            }
            else
            {
                int flag_num = num_of_custom_flag(custom_flags_list, flag_buf);

                switch (flag_num)
                {
                    case 0:
                    {
                        int cur_num = va_arg(param, int);
                        char* res = NULL;
                        f_res = to_roman(cur_num, &res);
                        if (f_res == fsc_ok)
                        {
                            strcat(str, res);
                            ++count;
                        }
                        break;
                    }
                    case 1:
                    {
                        int cur_num = va_arg(param, int);
                        buffer[0] = 0;
                        f_res = zeckendorf(buffer, cur_num);
                        
                        if (f_res == fsc_ok)
                        {
                            strcat(str, buffer);
                            ++count;
                        }
                        break;
                    }
                    case 2:
                    {
                        // число потом основание
                        int x = va_arg(param, int);
                        int n = va_arg(param, int);
                        char* res = NULL;
                        f_res = translation_10_to_x(x, &res, n);
                        
                        if (f_res == fsc_ok)
                        {
                            char* p_res = res;
                            while((*p_res) != 0)
                            {
                                if (isalpha(*p_res))
                                    (*p_res) += 32;
                                ++p_res;
                            }
                            
                            sprintf(buffer, "%s", res);
                            strcat(str, buffer);
                            ++count;
                        }
                        free(res);
                        break;
                    }
                    case 3:
                    {
                        // число потом основание
                        int x = va_arg(param, int);
                        int n = va_arg(param, int);
                        
                        char* res = NULL;
                        f_res = translation_10_to_x(x, &res, n);
                        
                        if (f_res == fsc_ok)
                        {
                            sprintf(buffer, "%s", res);
                            strcat(str, buffer);
                            ++count;
                        }
                        
                        if (res != NULL)
                            free(res);
                        break;
                    }
                    case 4:
                    {
                        int res = 0;
                        char* n = va_arg(param, char*);
                        char* p_n = n;
                        while(*p_n != 0)
                        {
                            if (isupper(*p_n))
                            {
                                f_res = fsc_invalid_parameter;
                                break;
                            }
                            ++p_n;
                        }
                        
                        if (f_res == fsc_ok)
                        {
                            int x = va_arg(param, int);
                            translation_x_to_10(n, &res, x);
                            sprintf(buffer, "%d", res);
                            strcat(str, buffer);
                            ++count;
                        }
                        
                        break;
                    }
                    case 5:
                    {
                        int res = 0;
                        char* n = va_arg(param, char*);
                        char* p_n = n;
                        while(*p_n != 0)
                        {
                            if (!(isupper(*p_n)))
                            {
                                f_res = fsc_invalid_parameter;
                                break;
                            }
                            ++p_n;
                        }
                        
                        if (f_res == fsc_ok)
                        {
                            int x = va_arg(param, int);
                            translation_x_to_10(n, &res, x);
                            sprintf(buffer, "%d", res);
                            strcat(str, buffer);
                        }
                        break;
                    }
                    case 6:
                    {
                        int a = va_arg(param, int);
                        unsigned char* p_data = (unsigned char*)(&a);
                        for (int i = 0; i < 4; ++i)
                        {
                            char* res = NULL;
                            f_res = translation_10_to_x(*p_data, &res, 2);
                            
                            if (f_res == fsc_ok)
                            {
                                sprintf(buffer, "%s ", res);
                                strcat(str, buffer);
                            }
                            
                            if (res != NULL)
                                free(res);
                            p_data++;
                            
                            if (f_res != fsc_ok)
                                break;
                        }
                        
                        if (f_res == fsc_ok)
                            ++count;
                        break;
                    }
                    case 7:
                    {
                        unsigned int a = va_arg(param, int);
                        unsigned char* p_data = (unsigned char*)(&a);
                        for (int i = 0; i < 4; ++i)
                        {
                            char* res = NULL;
                            f_res = translation_10_to_x(*p_data, &res, 2);
                            
                            if (f_res == fsc_ok)
                            {
                                sprintf(buffer, "%s ", res);
                                strcat(str, buffer);
                            }
                            
                            if (res != NULL)
                                free(res);
                            p_data++;
                            
                            if (f_res != fsc_ok)
                                break;
                        }
                        
                        if (f_res == fsc_ok)
                            ++count;
                        break;
                    }
                    case 8:
                    {
                        double a = va_arg(param, double);
                        unsigned char* p_data = (unsigned char*)(&a);
                        for (int i = 0; i < sizeof(double); ++i)
                        {
                            char* res = NULL;
                            f_res = translation_10_to_x(*p_data, &res, 2);
                            
                            if (f_res == fsc_ok)
                            {
                                sprintf(buffer, "%s ", res);
                                strcat(str, buffer);
                            }

                            if (res != NULL)
                                free(res);
                            p_data++;
                            
                            if (f_res != fsc_ok)
                                break;
                        }
                        
                        if (f_res == fsc_ok)
                            ++count;
                        break;
                    }
                    case 9:
                    {
                        float a = va_arg(param, double);
                        unsigned char* p_data = (unsigned char*)(&a);
                        for (int i = 0; i < sizeof(float); ++i)
                        {
                            char* res = NULL;
                            f_res = translation_10_to_x(*p_data, &res, 2);
                            
                            if (f_res == fsc_ok)
                            {
                                sprintf(buffer, "%s ", res);
                                strcat(str, buffer);
                            }
                            
                            if (res != NULL)
                                free(res);
                            p_data++;
                            
                            if (f_res != fsc_ok)
                                break;
                        }
                        
                        if (f_res == fsc_ok)
                            ++count;
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        else
        {
            int buffer_index = 0;
            while(*p_format != '%' && *p_format != 0)
            {
                buffer[buffer_index] = *p_format;
                ++buffer_index;
                ++p_format;
            }
            buffer[buffer_index] = 0;
            strcat(str, buffer);
        }
    }
    
    return count;
}

int oversprintf(char* str, char const * p_format, ...)
{
    va_list param;
    va_start(param, p_format);
    int count = overvsprintf(str, p_format, param);
    va_end(param);
    return count;
}

int overfprintf(FILE* output, char const * p_format, ...)
{
    va_list param;
    va_start(param, p_format);
    char str[MAX_BUFFER_SIZE];
    int count = overvsprintf(str, p_format, param);
    va_end(param);
    fprintf(output, "%s", str);
    return count;
}

int main(int argc, const char * argv[])
{
    enum status_codes f_res = fsc_ok;
    
    int a = 5;
    float b = 1.5f;
    char c = 'a';

    int count = 0;
    char str[MAX_BUFFER_SIZE];
    
    oversprintf(str, "%mu\n", 1);
    printf("%s", str);
    
    count = oversprintf(str, "%d %f %c Ro:%Ro Zr:%Zr\n", a, b, c, 5, 10);
    printf("%s", str);
    printf("Count: %d\n", count);
    
    FILE* output = fopen(argv[1], "w");
    if (output == NULL)
        f_res = fsc_memory_error_detected;
    
    if (f_res == fsc_ok)
        count = overfprintf(output, "%d %f %c Ro:%Ro Zr:%Zr\n", a, b, c, 5, 10);
    
    printf("File count: %d\n", count);
    
    if (output != NULL)
        fclose(output);
    return 0;
}
