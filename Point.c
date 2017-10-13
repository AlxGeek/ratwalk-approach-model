#include "Point.h"

void printPoint(Point p)
{
    printf("i = %d j = %d\n", p.i, p.j);
}

void swapPoint(Point *p1, Point *p2)
{
    Point aux = *p1;
    *p1 = *p2;
    *p2 = aux;
}

Point transformPoint(Point p, int width, double angle)
{
    Point pr;
    pr.i = p.i + cos(angle) * width;
    pr.j = p.j + sin(angle) * width;
    return pr;
}