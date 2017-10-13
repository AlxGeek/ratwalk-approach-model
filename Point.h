#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include <math.h>
#include "memo.h"

typedef struct Point Point;

struct Point{
    int i;
    int j;
};

void printPoint(Point p);

void swapPoint(Point *p1, Point *p2);

Point transformPoint(Point p, int width, double angle);

#endif //POINT_H