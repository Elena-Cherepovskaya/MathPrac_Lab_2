//
//  main.c
//  4_2
//
//  Created by Лена on 19.10.2023.
//

#include <stdio.h>
#include <stdarg.h>

enum status_codes
{
    fsc_ok,
    fsc_overflow,
    fsc_invalid_parameter,
    fsc_unknown,
};

enum status_codes value_polynom (double * res, double point, int p, ...)
{
    if (p < 0)
        return fsc_invalid_parameter;
    
    *res = 0.0;
    
    double list_of_coefficients[p + 1];
    va_list ptr;
    va_start(ptr, p);
    
    int count = p + 1;
    for (int i = 0; i < count; ++i)
    {
        list_of_coefficients[i] = va_arg(ptr, double);
    }
    
    double velue_pow = 1.0;
    for (int i = p; i >= 0; --i)
    {
        *res = *res + list_of_coefficients[i] * velue_pow;
        velue_pow *= point;
    }
    
    return fsc_ok;
}

int main(int argc, const char * argv[])
{
    double res;
    enum status_codes function_result = fsc_unknown;
    
    function_result = value_polynom(&res, 2.0, 0, 9.0);
    if (function_result == fsc_ok)
        printf("%lf\n", res);
    
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
        default:
            printf("function_result is unknown\n");
    }
    
    return function_result == fsc_ok ? 0 : 1;
}
