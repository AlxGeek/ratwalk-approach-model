#include "Point.h"

Point getPoint(int i, int j){
    Point p;
    p.i = i;
    p.j = j;
    return p;
}

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
    // pr.i = p.i + cos(angle) * width;
    // pr.j = p.j + sin(angle) * width;

    pr.i = p.i + cos(angle) * width;
    pr.j = p.j + sin(angle) * width;
    return pr;
}

Point transformPoint2(Point p1, Point p2, int width, double angle)
{
    Point p3;
    p3.i = p1.i + (p2.i - p1.i) * cos(angle) - (p2.j - p1.j) * sin(angle);
    p3.j = p1.j + (p2.i - p1.i) * sin(angle) + (p2.j - p1.j) * cos(angle);

    return modifyLine(p1, p3, width);
}

Point modifyLine(Point p1, Point p2, int width)
{
    Point p3;
    double distance = getDistance(p1, p2);
    double w = width - distance;
    p3.i = p2.i + w / distance * (p2.i - p1.i);
    p3.j = p2.j + w / distance * (p2.j - p1.j);
    return p3;
}

double getDistance(Point p1, Point p2)
{
    return sqrt(pow(p2.i - p1.i, 2) + pow(p2.j - p1.j, 2));
}

int equalsPoints(Point p1, Point p2){
    return p1.i == p2.i && p1.j == p2.j ? 1 : 0;
}