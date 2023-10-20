
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>

enum status_codes
{
    fsc_ok,
    fsc_invalid_parameter,
    fsc_unknown,
};

enum status_codes geometric_mean (double * res, int count, ...)
{
    *res = 1.0;
    
    if (count <= 0)
        return fsc_invalid_parameter;
    
    va_list ptr;
    va_start(ptr, count);
    
    for (int i = 0; i < count; ++i)
        *res *= va_arg(ptr, double);
    
    
    *res = pow(*res, 1.0 / count);
    return fsc_ok;
}

int main(int argc, const char * argv[])
{
    enum status_codes function_result = fsc_invalid_parameter;
    
    double res;
    
    function_result =  geometric_mean(&res, 0);
    if (function_result == fsc_ok)
        printf("%lf\n", res);
    
    switch (function_result)
    {
        case fsc_ok:
            break;
        case fsc_invalid_parameter:
            printf("Invalid parameter detected\n");
            break;
        default:
            printf("function_result is unknown\n");
    }
    
    return function_result == fsc_ok ? 0 : 1;
}
