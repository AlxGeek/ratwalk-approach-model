#include "Mat.h"

Mat createMat(int rows, int cols)
{
	Mat mat;
	mat.rows = rows;
	mat.cols = cols;
	mat.data = reservaMatrizDouble(rows, cols);
	return mat;
}

Mat readMatFromFile(FILE *file)
{
	Mat mat;
	char number[30];
	int i = 0, j = 0, k = 0;
	char c;
	int cols = 500, rows = 100;
	mat.data = reservaMatrizDouble(rows, cols);
	while ((c = getc(file)) != EOF)
	{
		if (c == ',')
		{
			number[k] = '\0';
			k = 0;
			mat.data[i][j++] = atof(number);
		}
		else if (c == '\n')
		{
			i++;
			mat.cols = j;
			j = 0;
		}
		else
		{
			number[k++] = c;
		}
		if (i >= rows && j >= cols)
		{
			mat.data = (double **)expandMatrix((void **)mat.data, sizeof(double), rows, cols, rows + 200, cols + 200);
			cols += 200;
			rows += 200;
		}
		else if (i >= rows)
		{
			mat.data = (double **)expandMatrix((void **)mat.data, sizeof(double), rows, cols, rows + 200, cols);
			rows += 200;
		}
		else if (j >= cols)
		{
			mat.data = (double **)expandMatrix((void **)mat.data, sizeof(double), rows, cols, rows, cols + 500);
			cols += 500;
		}
	}
	mat.rows = i;
	fclose(file);
	return mat;
}

Mat copyMat(Mat src)
{
	Mat dst = createMat(src.rows, src.cols);
	dst.ngray = src.ngray;
	for (int i = 0; i < dst.rows; i++)
		for (int j = 0; j < dst.cols; j++)
			dst.data[i][j] = src.data[i][j];
	return dst;
}

void freeMat(Mat mat)
{
	mat.rows = 0;
	mat.cols = 0;
	liberaMatriz((void **)mat.data);
	mat.data = NULL;
}

Mat openPGM(char *filename)
{
	FILE *file = fopen(filename, "r");
	Mat image;
	image.data = NULL;
	if (!file)
	{
		printf("Error al abrir el archivo: %s\n", filename);
		return image;
	}
	char str[10];
	fscanf(file, "%s", str);
	fscanf(file, "%d %d", &image.cols, &image.rows);
	fscanf(file, "%d", &image.ngray);
	image.data = reservaMatrizDouble(image.rows, image.cols);
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			fscanf(file, "%lf", &image.data[i][j]);
		}
	}
	fclose(file);
	return image;
}

void printMat(Mat mat)
{
	for (int i = 0; i < mat.rows; i++)
	{
		for (int j = 0; j < mat.cols; j++)
		{
			printf("%f ", mat.data[i][j]);
		}
		printf("\n");
	}
}

void printMat2(Mat mat)
{
	for (int i = 0; i < mat.rows; i++)
	{
		for (int j = 0; j < mat.cols; j++)
		{

			if (mat.data[i][j] != 0 && i < 200)
			{
				printf("r - %d c - %d\n", i, j);
			}
		}
	}
}

Mat getTrimmedImage(Mat mat)
{
	int firstRow = getFirstRow(mat);
	int firstCol = getFirstCol(mat);
	int lastRow = getLastRow(mat);
	int lastCol = getLastCol(mat);
	Mat trimImg = createMat(lastRow - firstRow + 1, lastCol - firstCol + 1);
	trimImg.ngray = mat.ngray;
	int i, j, n, m;
	for (i = 0, n = firstRow; n <= lastRow; n++, i++)
	{
		for (j = 0, m = firstCol; m <= lastCol; m++, j++)
		{
			trimImg.data[i][j] = mat.data[n][m];
		}
	}
	return trimImg;
}

int interpolate(int i, int i1, int j1, int i2, int j2)
{
	return (i - i1) / (float)(i2 - i1) * (j2 - j1) + j1;
}

int interpolateP(int i, Point p1, Point p2)
{
	return (i - p1.i) / (float)(p2.i - p1.i) * (p2.j - p1.j) + p1.j;
}

int interpolateM(int i, int i1, int j1, double m)
{
	return round((i - i1) * m) + j1;
}

int interpolateM2(int i, int i1, int j1, double m)
{
	return (i - i1) * m + j1;
}

void getFinalPoint(Mat mat, int i1, int j1, int *i2, int *j2, double m)
{
	*i2 = i1;
	*j2 = interpolateM(*i2, i1, j1, m);

	while (mat.data[*i2][*j2] != 0)
	{
		(*i2)++;
		*j2 = interpolateM(*i2, i1, j1, m);
	}
}

int getFirstRow(Mat mat)
{
	for (int i = 0; i < mat.rows; i++)
		for (int j = 0; j < mat.cols; j++)
			if (mat.data[i][j] != 0)
				return i;
	return -1;
}

int getFirstCol(Mat mat)
{
	for (int j = 0; j < mat.cols; j++)
		for (int i = 0; i < mat.rows; i++)
			if (mat.data[i][j] != 0)
				return j;
	return -1;
}

int getLastRow(Mat mat)
{
	for (int i = mat.rows - 1; i >= 0; i--)
		for (int j = mat.cols - 1; j >= 0; j--)
			if (mat.data[i][j] != 0)
				return i;
	return -1;
}

int getLastCol(Mat mat)
{
	for (int j = mat.cols - 1; j >= 0; j--)
		for (int i = mat.rows - 1; i >= 0; i--)
			if (mat.data[i][j] != 0)
				return j;
	return -1;
}

void pgmToFile(Mat image, char *filename)
{
	FILE *file = fopen(filename, "w");
	if (!file)
	{
		printf("Error al escribir el archivo: %s\n", filename);
		return;
	}
	fprintf(file, "P2\n");
	fprintf(file, "%d %d\n", image.cols, image.rows);
	fprintf(file, "%d\n", image.ngray);
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			fprintf(file, "%d ", (int)image.data[i][j]);
		}
		fprintf(file, "\n");
	}
	fclose(file);
}

double slope(Point p1, Point p2)
{
	return (p2.j - p1.j) / (float)(p2.i - p1.i);
}

void fixPoint(Mat mat, Point *point)
{
	if (point->i < 0)
	{
		point->i = 0;
	}
	if (point->j < 0)
	{
		point->j = 0;
	}
	if (point->i >= mat.rows)
	{
		point->i = mat.rows - 1;
	}
	if (point->j >= mat.cols)
	{
		point->j = mat.cols - 1;
	}
}

int validatePoint(Mat mat, Point point)
{
	if (point.i < 0 || point.j < 0 || point.i >= mat.rows || point.j >= mat.cols)
		return 0;
	else
		return 1;
}