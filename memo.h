#ifndef MEMO_H
#define MEMO_H

#include <stdlib.h>
#include <string.h>

void **reservaMatriz(int tamElemento, int filas, int columnas);

void liberaMatriz(void **matriz);

void *reservaVector(int tamElemento, int n);

void liberaVector(void *vector);

int **reservaMatrizInt(int filas, int columnas);
float **reservaMatrizFloat(int filas, int columnas);
char **reservaMatrizChar(int filas, int columnas);
unsigned char **reservaMatrizUnsignedChar(int filas, int columnas);
double **reservaMatrizDouble(int filas, int columnas);

int *reservaVectorInt(int n);
float *reservaVectorFloat(int n);
char *reservaVectorChar(int n);
unsigned char *reservaVectorUnsignedChar(int n);
double *reservaVectorDouble(int n);

void **expandMatrix(void **mat, int byteElement, int rows, int cols, int newRows, int newCols);

void *expandVector(void *vec, int byteElement, int n);

void initMatrixZeros(void **mat, int size, int byteElement, int rows, int cols);
void initVectorOnes(void **vec, int byteElement, int n);

void initVectorInt(int *vec, int value, int n);

#endif //MEMO_H