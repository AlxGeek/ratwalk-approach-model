
#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <stdlib.h>
#include <time.h>

#include "Mat.h"
#include "Draw.h"

#define crossoverProbability 50
#define mutationProbability 30

typedef struct Individual Individual;

struct Individual
{
    Point p1;
    Point p2;
    int width;
    double fitness;
};

void fixIndividual(Individual* individual);

void fixPopulation(Individual *population, int n);

void objetiveFunction(Mat mat, Individual *individual);

double linealObjetiveFunction(Mat mat, Point p1, Point p2);

double DDAlinealObjetiveFunction(Mat mat, Point p1, Point p2);

void printIndividual(Individual individual);

Individual initIndividual(Individual seed);

Individual *initPopulation(Individual seed, int n);

void setFitness(Mat mat, Individual *population, int n);

void binaryTournamet(Individual *population, int n, Individual *populationAux);

void copyPopulation(Individual *src, Individual *dst, int n);

Individual getElite(Individual *population, int n);

void crossover(Individual *population, int n);

void mutation(Individual *population, int n);

Individual optimize(Mat mat, Individual seed);

#endif //OPTIMIZATION_H