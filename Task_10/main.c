//
//  main.c
//  10
//
//  Created by Лена on 26.10.2023.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

enum status_codes
{
    fsc_ok,
    fsc_overflow,
    fsc_invalid_parameter,
    fsc_unknown,
    fsc_memory_error_detected,
};

enum status_codes fact(int n, int* res)
{
    if (n < 0)
        return fsc_invalid_parameter;
    *res = 1;
    for (int i = 2; i <= n; ++i)
    {
        *res *= i;
        if (*res < 0)
            return fsc_overflow;
    }
    return fsc_ok;
}

enum status_codes Taylor_function (double a, double* coefficients, int degree, int num_of_coefficient, double* res)
{
    double x = 1.0;
    int tmp_fact = 0;
    *res = 0.0;
    
    for (int i = 0; i <= degree; ++i)
    {
        *res = *res + (coefficients[i] * x);
        x *= a;
    }
    
    if (fact(num_of_coefficient, &tmp_fact) != fsc_ok)
        return fsc_overflow;
    
    *res /= tmp_fact;
    return fsc_ok;
}

void derivative(double* coefficients, int degree, double** res_coefficient)
{
    for (int i = 0; i < degree; ++i)
        (*res_coefficient)[i] = (i + 1) * coefficients[i + 1];
    
    return;
}

enum status_codes create_new_polynom(double eps, double a, double** list_of_new_coefficients, int degree, ...)
{
    if (degree < 0)
        return fsc_invalid_parameter;
    
    int col_of_coefficients = degree + 1;
    
    double* coefficients = malloc(sizeof(double) * col_of_coefficients);
    if (coefficients == NULL)
        return fsc_overflow;
    
    va_list ptr;
    va_start(ptr, degree);
    for (int i = 0; i < col_of_coefficients; ++i)
    {
        double x = va_arg(ptr, double);
        coefficients[i] = x;
    }
    
    double res = 0.0;
    
    double* res_coefficients = malloc(sizeof(double) * degree);
    if (res_coefficients == NULL)
    {
        free(coefficients);
        return fsc_overflow;
    }
    
    *list_of_new_coefficients = malloc(sizeof(double) * col_of_coefficients);
    if (*list_of_new_coefficients == NULL)
    {
        free(res_coefficients);
        return fsc_overflow;
    }
    
    for (int i = 0; i < col_of_coefficients; ++i)
    {
        Taylor_function(a, coefficients, degree - i, i, &res);
        (*list_of_new_coefficients)[i] = res;

        derivative(coefficients, degree - i, &res_coefficients);
        
        for (int j = 0; j < degree; ++j)
            coefficients[j] = res_coefficients[j];
    }
    
    free(coefficients);
    free(res_coefficients);
    return fsc_ok;
}

int main(int argc, const char * argv[])
{
    enum status_codes function_result = fsc_unknown;
    
    double eps = 000.1;
    double* list_of_new_coefficients = NULL;
    double a = -3.0;
    int degree = 5;
    function_result = create_new_polynom(eps, a, &list_of_new_coefficients, degree, 7.0, -5.0, 1.0, 6.0, 2.0, 1.0);
    
    if (function_result == fsc_ok)
    {
        for (int i = 0; i <= degree; ++i)
            printf("%lf*(x - %lf)^%d %s ", list_of_new_coefficients[i], a, i, (i < degree) ? "+" : "");
        printf("\n");
        free(list_of_new_coefficients);
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
