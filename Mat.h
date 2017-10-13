#ifndef MAT_H
#define MAT_H

#include <stdio.h>
#include <math.h>
#include "Point.h"
#include "memo.h"

typedef struct Mat Mat;

struct Mat{
    double **data;
    int rows;
    int cols;
    int ngray;
};

Mat createMat(int rows, int cols);

void pgmToFile(Mat pgm, char* filename);

Mat readMatFromFile(FILE *file);

Mat copyMat(Mat src);

void freeMat(Mat mat);

Mat openPGM(char* filename);

void printMat(Mat mat);

Mat getTrimmedImage(Mat mat);

int getFirstRow(Mat mat);

int getFirstCol(Mat mat);

int getLastRow(Mat mat);

int getLastCol(Mat mat);

int interpolate(int i, int i1, int j1, int i2, int j2);

int interpolateP(int i, Point p1, Point p2);

int interpolateM(int i, int i1, int j1, double m);

//void getFinalPoint(int i1, int j1, int &i2, int &j2, double value, double angle);

void getFinalPoint(Mat mat, int i1, int j1, int *i2, int *j2, double value, double angle);

double slope(Point p1, Point p2);

#endif //MAT_H