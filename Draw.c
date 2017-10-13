#include "Draw.h"

void drawLine(Mat mat, double value, Point p1, Point p2)
{
    if (p1.i > p2.i)
    {
        swapPoint(&p1, &p2);
    }
    for (int i = p1.i; i < p2.i; i++)
    {
        mat.data[i][interpolate(i, p1.i, p1.j, p2.i, p2.j)] = value;
    }
}

void BresenhamsLine(Mat mat, double value, Point p1, Point p2)
{
    if (p1.i > p2.i)
    {
        swapPoint(&p1, &p2);
    }
    float deltai = p2.i - p1.i;
    float deltaj = p2.j - p1.j;
    float deltaerr = fabs(deltaj / deltai); // Assume deltax != 0 (line is not vertical),
                                            // note that this division needs to be done in a way that preserves the fractional part
    float error = 0.0;                      // No error at start
    int j = p1.j;
    for (int i = p1.i; i < p2.i; i++)
    {
        mat.data[i][j] = value;
        error = error + deltaerr;
        if (error >= 0.5)
        {
            j = j + 1;
            error = error - 1.0;
        }
    }
}
void drawRect(Mat mat, double value, Point p1, Point p2, int width)
{
    double angle;
    Point pp1, pp2, p3, p4;
    pp1 = p1;
    pp2 = p2;
    angle = atan2(p2.j - p1.j, p2.i - p1.i) + 1.5708;
    p3 = transformPoint(p1, width, angle);
    angle = atan2(p1.j - p2.j, p1.i - p2.i) + 1.5708;
    p4 = transformPoint(p2, width, angle);
    
    int steps;
    float Iincrement1, Jincrement1,  Iincrement2, Jincrement2;
    getDDAdata(p1, p3, &Iincrement1, &Jincrement1, &steps);    
    getDDAdata(p2, p4, &Iincrement2, &Jincrement2, &steps);    

    pp1 = p1;
    pp2 = p2;

    for (int v = 0; v <= steps; v++)
    {
        DDAline(mat, value, pp1, pp2);
        pp1.i += Iincrement1;
        pp1.j += Jincrement1;
        pp2.i += Iincrement2;
        pp2.j += Jincrement2;
    }


}

void DDAline(Mat mat, double value, Point p1, Point p2)
{
    int steps;
    float Iincrement, Jincrement;
    getDDAdata(p1, p2, &Iincrement, &Jincrement, &steps);

    float i = p1.i;
    float j = p1.j;
    for (int v = 0; v < steps; v++)
    {
        i += Iincrement;
        j += Jincrement;
        mat.data[(int)round(i)][(int)round(j)] = value;
    }
}

void getDDAdata(Point p1, Point p2, float *Iincrement, float *Jincrement, int *steps)
{
    int di = p2.i - p1.i;
    int dj = p2.j - p1.j;
    *steps = abs(di) > abs(dj) ? abs(di) : abs(dj);
    *Iincrement = di / (float)*steps;
    *Jincrement = dj / (float)*steps;
}
