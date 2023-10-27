//
//  main.c
//  7
//
//  Created by Лена on 27.10.2023.
//

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

double f_1 (double x)
{
    return log(x) - 1.0;
}

double f_2 (double x)
{
    return cos(x) + 1.0;
}

double f_3 (double x)
{
    return exp(x) - 2.0;
}

double dichotomy_method (double begin, double end, double eps, double (*f_ptr) (double))
{
    double f_a, f_b, f_c;
    double delta = eps / 100.0;
    
    double a = begin;
    double b = end;
    
    while (true)
    {
        double c = (a+b) / 2.0;
        double x1 = (a+b-delta)/2.0;
        double x2 = (a+b+delta)/2.0;
        
        f_a = f_ptr(x1);
        f_b = f_ptr(x2);
        f_c = f_ptr(c);
        if ((fabs(f_c) < eps) || ((b - a) < delta))
            return c;
        else
        {
            if (fabs(f_a) > fabs(f_b))
                a = c + delta;
            else
                b = c - delta;
        }
    }
    
}
int main(int argc, const char * argv[])
{
    printf("%lf\n", dichotomy_method(1.0, 3.0, 0.001, f_1));
    printf("%lf\n", dichotomy_method(1.0, 4.0, 0.001, f_2));
    printf("%lf\n", dichotomy_method(0.0, 1.0, 0.001, f_3));
    return 0;
}
