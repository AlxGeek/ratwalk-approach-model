#ifndef DRAW_H
#define DRAW_H

#define _USE_MATH_DEFINES

#include <math.h>
#include "Mat.h"

void BresenhamsLine(Mat mat, double value, Point p1, Point p2);

void drawLine(Mat mat, double value, Point p1, Point p2);

void drawRect(Mat mat, double value, Point p1, Point p2, int width);

void DDAline(Mat mat, double value, Point p1, Point p2);

void getDDAdata(Point p1, Point p2, float *Iincrement, float *Jincrement, int *steps);


#endif //DRAW_H