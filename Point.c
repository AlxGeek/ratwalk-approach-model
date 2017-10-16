#include "Point.h"

Point getPoint(int i, int j)
{
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

int equalsPoints(Point p1, Point p2)
{
    return p1.i == p2.i && p1.j == p2.j ? 1 : 0;
}

double getAngleTwoLines(Point p1, Point p2, Point p3, Point p4)
{
    double angle1 = atan2(p2.j - p1.j, p2.i - p1.i);
    double angle2 = atan2(p4.j - p3.j, p4.i - p3.i);
    return angle1 - angle2;
}

double getAngle(Point p1, Point p2)
{
    return atan2(p2.j - p1.j, p2.i - p1.i);
}

double radianToDegree(double radian)
{
    return radian * 180 / M_PI;
}

double degreeToRadian(double degree){
    return degree *  M_PI / 180;
}

Point closestPoint(Point p, Point p1, Point p2)
{
    if (getDistance(p, p1) < getDistance(p, p2))
        return p1;
    else
        return p2;
}

Point farestPoint(Point p, Point p1, Point p2)
{
    if (getDistance(p, p1) > getDistance(p, p2))
        return p1;
    else
        return p2;
}

Point greatPointI(Point p1, Point p2)
{
    if (p1.i > p2.i)
        return p1;
    else
        return p2;
}


Point greatPointJ(Point p1, Point p2)
{
    if (p1.j > p2.j)
        return p1;
    else
        return p2;
}

Point lessPointJ(Point p1, Point p2)
{
    if (p1.j < p2.j)
        return p1;
    else
        return p2;
}