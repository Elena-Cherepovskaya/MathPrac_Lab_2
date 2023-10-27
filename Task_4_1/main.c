//
//  main.c
//  4_1
//
//  Created by Лена on 19.10.2023.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

typedef struct
{
    double x;
    double y;
}Vector2D;

bool polygon_is_convex (int count, ...)
{
    int tmp_count = count;
    
    if (tmp_count <= 2)
        return false;
    
    va_list ptr;
    va_start(ptr, count);
    
    Vector2D vertex_start;//Вершина с которой начинается обход
    vertex_start.x = va_arg(ptr, double);
    vertex_start.y = va_arg(ptr, double);
    tmp_count--;
    Vector2D vertex_1 = vertex_start;
    
    Vector2D vertex_2;
    vertex_2.x = va_arg(ptr, double);
    vertex_2.y = va_arg(ptr, double);
    tmp_count--;
    
    Vector2D vector_2;
    vector_2.x = vertex_1.x - vertex_2.x;
    vector_2.y = vertex_1.y - vertex_2.y;
    
    Vector2D vector_1;
    Vector2D vector_start = vector_2;
    
    for (int i = 0; i <= tmp_count + 1; ++i)
    {
        vertex_1 = vertex_2;
        vector_1 = vector_2;
        
        if (i > tmp_count)
        {
            vector_2 = vector_start; 
        }
        else
        {
            if (i == tmp_count)
            {
                vertex_2 = vertex_start;
            }
            else
            {
                vertex_2.x = va_arg(ptr, double);
                vertex_2.y = va_arg(ptr, double);
            }
            vector_2.x = vertex_1.x - vertex_2.x;
            vector_2.y = vertex_1.y - vertex_2.y;
        }
                

        double vector_product = vector_1.x * vector_2.y - vector_2.x * vector_1.y;
        if (vector_product < 0)
            return false;
    }

    return true;
}


int main(int argc, const char * argv[])
{
    printf("Многоугольник %s выпуклый\n", (polygon_is_convex(4, 1.0, 1.0, 2.0, 1.0, 2.0, 2.0, 1.0, 2.0)) ? "" : "не");
    
    return 0;
}
