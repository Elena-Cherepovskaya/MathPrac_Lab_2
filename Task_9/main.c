//
//  main.c
//  9
//
//  Created by Лена on 26.10.2023.
//

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>

#define INT_MAX 1e8

enum status_codes
{
    fsc_ok,
    fsc_invalid_parameter,
    fsc_unknown,
    fsc_memory_error_detected,
};

double f_eps(void)
{
    double eps = 1.0;
    while (1.0 < 1.0 + (eps / 2.0))
        eps /= 2.0;
    
    return eps;
}

bool _convert(double x, int base)
{
    double eps = f_eps();
    int b = 1;
    while (x > eps)
    {
        x *= base;
        double v = floor(x);
        x = x - v;
        b = b * base;
        if (b > INT_MAX)
            return false;
    }
    return true;
}

enum status_codes convert(bool** res, int base, int count, ...)
{
    if (count <= 0)
        return fsc_invalid_parameter;
    
    *res = malloc(sizeof(bool) * count);
    if (res == NULL)
        return  fsc_memory_error_detected;
    
    va_list ptr;
    va_start(ptr, count);
    
    for (int i = 0; i < count; ++i)
    {
        double x = va_arg(ptr, double);
        (*res)[i] = _convert(x, base);
    }
    
    return fsc_ok;
}

int main(int argc, const char * argv[])
{
    enum status_codes function_result = fsc_unknown;
    
    bool* res = NULL;
    int base = 16;
    int count = 3;
    
    function_result = convert(&res, base, count, 0.00390625, 0.1875, 0.67);
    
    if (function_result == fsc_ok)
    {
        for (int i = 0; i < count; ++i)
            printf("%d-е число  %s имеет %d-e конечное дробное представление\n",i, res[i] ? "" : "не", base);
        free(res);
    }
    
    switch (function_result)
    {
        case fsc_ok:
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
