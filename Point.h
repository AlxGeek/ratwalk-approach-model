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

struct PointF{
    float i;
    float j;
};

void printPoint(Point p);

Point getPoint(int i, int j);

void swapPoint(Point *p1, Point *p2);

Point transformPoint(Point p, int width, double angle);

Point transformPoint2(Point p1, Point p2, int width, double angle);

Point modifyLine(Point p1, Point p2, int width);

double getDistance(Point p1, Point p2);

int equalsPoints(Point p1, Point p2);

double getAngleTwoLines(Point p1, Point p2, Point p3, Point p4);

double getAngle(Point p1, Point p2);

double radianToDegree(double radian);

double degreeToRadian(double degree);

Point closestPoint(Point p, Point p1, Point p2);

Point farestPoint(Point p, Point p1, Point p2);

Point greatPointI(Point p1, Point p2);

Point greatPointJ(Point p1, Point p2);

Point lessPointJ(Point p1, Point p2);

#endif //POINT_H