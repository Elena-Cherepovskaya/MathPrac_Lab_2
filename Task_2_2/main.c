//
//  main.c
//  2_2
//
//  Created by Лена on 18.10.2023.
//

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

double _fast_pow (double n, int p)
{
    if (p == 0)
        return  1.0;
    
    if ((p & 1) == 0)
    {
        double tmp_res = _fast_pow(n, p / 2);
        return tmp_res * tmp_res;
    }
    else
        return _fast_pow(n, p - 1) * n;
}

double fast_pow(double n, int p)
{
    return p >= 0 ? _fast_pow(n, p) : 1.0 / _fast_pow(n, -p);
}

int main(int argc, const char * argv[])
{
    printf("%lf\n", fast_pow(0, 0));
    
    return 0;
}
