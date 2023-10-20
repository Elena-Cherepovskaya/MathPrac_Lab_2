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
    
    Vector2D vertex_start;
    vertex_start.x = va_arg(ptr, double);
    vertex_start.y = va_arg(ptr, double);
    tmp_count--;
    
    Vector2D vertex_1;
    vertex_1.x = va_arg(ptr, double);
    vertex_1.y = va_arg(ptr, double);
    tmp_count--;
    
    Vector2D list_of_sides[count];
    int ind_list_of_sides = 0;
    
    list_of_sides[ind_list_of_sides].x = vertex_start.x - vertex_1.x;
    list_of_sides[ind_list_of_sides].y = vertex_start.y - vertex_1.y;
    ++ind_list_of_sides;
    
    Vector2D vertex_2;
    for (int i = 0; i < tmp_count; ++i)
    {
        vertex_2.x = va_arg(ptr, double);
        vertex_2.y = va_arg(ptr, double);
        
        list_of_sides[ind_list_of_sides].x = vertex_1.x - vertex_2.x;
        list_of_sides[ind_list_of_sides].y = vertex_1.y - vertex_2.y;
        ++ind_list_of_sides;
        
        vertex_1 = vertex_2;
        
        //vertex_1.x = vertex_2.x;
        //vertex_1.y = vertex_2.y;
    }

    list_of_sides[ind_list_of_sides].x = vertex_2.x - vertex_start.x;
    list_of_sides[ind_list_of_sides].y = vertex_2.y - vertex_start.y;

    ind_list_of_sides = 0;
    double vector_product = list_of_sides[count - 1].x * list_of_sides[ind_list_of_sides].y - list_of_sides[ind_list_of_sides].x * list_of_sides[count - 1].y;
    
    bool difference_positive = (vector_product >= 0.0) ? true : false;

    for (int i = 0; i < count - 1; ++i)
    {
        vector_product = list_of_sides[i].x * list_of_sides[i + 1].y - list_of_sides[i + 1].x * list_of_sides[i].y;
        
        bool tmp_difference_positive = (vector_product >= 0.0);

        if (difference_positive != tmp_difference_positive)
            return false;
    }
    return true;
}


int main(int argc, const char * argv[])
{
    printf("Многоугольник %s выпуклый\n", (polygon_is_convex(3, 1.0, 1.0, 2.0, 1.0, 2.0, 2.0)) ? "" : "не");
    
    return 0;
}
