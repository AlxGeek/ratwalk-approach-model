#include "memo.h"

void **reservaMatriz(int tamElemento, int filas, int columnas)
{
	void **matriz = malloc(sizeof(void *) * filas);
	//1024*1024*10 = 10485760
	if (filas * columnas * tamElemento < 10485760 && (matriz[0] = malloc(tamElemento * filas * columnas)))
	{
		void *temp = matriz[0];
		for (int i = 1; i < filas; i++)
		{
			temp = (char *)temp + (columnas * tamElemento);
			matriz[i] = temp;
		}
	}
	else
	{
		for (int i = 0; i < filas; i++)
		{
			matriz[i] = malloc(tamElemento * columnas);
		}
	}
	return matriz;
}
void *reservaVector(int tamElemento, int n)
{
	return malloc(tamElemento * n);
}

int **reservaMatrizInt(int filas, int columnas)
{
	return (int **)reservaMatriz(sizeof(int), filas, columnas);
}

float **reservaMatrizFloat(int filas, int columnas)
{
	return (float **)reservaMatriz(sizeof(float), filas, columnas);
}

char **reservaMatrizChar(int filas, int columnas)
{
	return (char **)reservaMatriz(sizeof(char), filas, columnas);
}

unsigned char **reservaMatrizUnsignedChar(int filas, int columnas)
{
	return (unsigned char **)reservaMatriz(sizeof(unsigned char), filas, columnas);
}

double **reservaMatrizDouble(int filas, int columnas)
{
	return (double **)reservaMatriz(sizeof(double), filas, columnas);
}

int *reservaVectorInt(int n)
{
	return (int *)reservaVector(sizeof(int), n);
}

float *reservaVectorFloat(int n)
{
	return (float *)reservaVector(sizeof(float), n);
}

char *reservaVectorChar(int n)
{
	return (char *)reservaVector(sizeof(char), n);
}

unsigned char *reservaVectorUnsignedChar(int n)
{
	return (unsigned char *)reservaVector(sizeof(unsigned char), n);
}

double *reservaVectorDouble(int n)
{
	return (double *)reservaVector(sizeof(double), n);
}

void liberaMatriz(void **matriz)
{
	free(matriz[0]);
	free(matriz);
}

void liberaVector(void *vector)
{
	free(vector);
}

void **expandMatrix(void **mat, int byteElement, int rows, int cols, int newRows, int newCols)
{
	int sizeRow = byteElement * newCols;
	if (rows != newRows)
	{
		mat = realloc(mat, sizeof(void *) * newRows);
	}
	if (rows * cols * byteElement < 10485760 && newRows * newCols * byteElement >= 10485760)
	{
		void **aux = reservaMatriz(byteElement, newRows, newCols);
		for (int i = 0; i < rows; i++)
			memcpy(aux[i], mat[i], byteElement * cols);
		free(mat);
		mat = aux;
	}
	else if (newRows * newCols * byteElement < 10485760 && (mat[0] = realloc(mat[0], byteElement * newRows * newCols)))
	{
		for (int i = 1; i < newRows; i++)
		{
			mat[i] = (char *)mat[i - 1] + sizeRow;
		}
	}
	else
	{
		for (int i = 0; i < rows; i++)
		{
			mat[i] = realloc(mat[i], sizeRow);
		}
		for (int i = rows; i < newRows; i++)
		{
			mat[i] = malloc(sizeRow);
		}
	}
	return mat;
}

void *expandVector(void *vec, int byteElement, int n)
{
	return realloc(vec, byteElement * n);
}

void initMatrixZeros(void **mat, int size, int byteElement, int rows, int cols)
{
	//1024*1024*10 = 10485760
	if (size < 10485760)
	{
		memset(mat[0], 0, rows * cols * byteElement);
	}
	else
	{
		int sizeRow = byteElement * cols;
		for (int i = 0; i < rows; i++)
		{
			memset(mat[i], 0, sizeRow);
		}
	}
}

void initVectorInt(int *vec, int value, int n)
{
	for (int i = 0; i < n; i++)
		vec[i] = value;
}

void initIntVectorValue(void *vec, int byteElement, int n)
{
	memset(vec, 1, n * byteElement);
}