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
    if (equalsPoints(p1, p2))
    {
        return;
    }
    Point pp1, pp2, p3, p4;    
    p3 = transformPoint2(p2, p1, width, -M_PI_2);
    p4 = transformPoint2(p1, p2, width, M_PI_2);

    int steps, steps1, steps2;
    float Iincrement1, Jincrement1, Iincrement2, Jincrement2;
    getDDAdata(p1, p4, &Iincrement1, &Jincrement1, &steps1);
    getDDAdata(p2, p3, &Iincrement2, &Jincrement2, &steps2);

    if (steps1 != steps2)
    {
        printf("s1 = %d ", steps1);
        printf("s2 = %d\n", steps2);
    }
    steps = fmin(steps1, steps2);

    float i1, i2, j1, j2;
    i1 = p1.i;
    i2 = p2.i;
    j1 = p1.j;
    j2 = p2.j;

    for (int v = 0; v <= steps; v++)
    {
        pp1.i = round(i1);
        pp1.j = round(j1);
        pp2.i = round(i2);
        pp2.j = round(j2);
        DDAline(mat, value, pp1, pp2);
        i1 += Iincrement1;
        j1 += Jincrement1;
        i2 += Iincrement2;
        j2 += Jincrement2;
    }
}

void DDAline(Mat mat, double value, Point p1, Point p2)
{
    int steps;
    float Iincrement, Jincrement;
    getDDAdata(p1, p2, &Iincrement, &Jincrement, &steps);

    float i = p1.i;
    float j = p1.j;
    for (int v = 0; v <= steps; v++)
    {
        mat.data[(int)round(i)][(int)round(j)] = value;
        mat.data[(int)round(i)][(int)round(j)+1] = value;
        i += Iincrement;
        j += Jincrement;
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
