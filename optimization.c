#include "optimization.h"

void objetiveFunction(Mat mat, Individual *individual)
{
    double angle;
    Point pp1, pp2, p3, p4;
    individual->fitness = 0;
    pp1 = individual->p1;
    pp2 = individual->p2;
    angle = atan2(individual->p2.j - individual->p1.j, individual->p2.i - individual->p1.i) + 1.5708;
    p3 = transformPoint(individual->p1, individual->width, angle);
    //angle = atan2(individual->p1.j - individual->p2.j, individual->p1.i - individual->p2.i) + 1.5708;
    p4 = transformPoint(individual->p2, individual->width, angle);

    if (p3.i < 0 || p3.j < 0 || p4.i < 0 || p4.j < 0)
    {
        printf("Hola\n");
        return;
    }

    int steps, steps1,steps2;
    float Iincrement1, Jincrement1,  Iincrement2, Jincrement2;
    getDDAdata(individual->p1, p3, &Iincrement1, &Jincrement1, &steps1);
    printf("s1 = %d ",steps1);
    getDDAdata(individual->p2, p4, &Iincrement2, &Jincrement2, &steps2);
    printf("s2 = %d\n",steps2);
    steps = fmin(steps1, steps2);

    pp1 = individual->p1;
    pp2 = individual->p2;

    for (int v = 0; v <= steps; v++)
    {
        individual->fitness += DDAlinealObjetiveFunction(mat, pp1, pp2);    
        pp1.i += Iincrement1;
        pp1.j += Jincrement1;
        pp2.i += Iincrement2;
        pp2.j += Jincrement2;
    }

}

double DDAlinealObjetiveFunction(Mat mat, Point p1, Point p2)
{
    double fitness = 0;
    int steps;
    float Iincrement, Jincrement;
    getDDAdata(p1, p2, &Iincrement, &Jincrement, &steps);

    float i = p1.i;
    float j = p1.j;
    for (int v = 0; v < steps; v++)
    {
        i += Iincrement;
        j += Jincrement;
        if (mat.data[(int)round(i)][(int)round(j)] != 0)
        {
            fitness++;
        }
        else
        {
            fitness--;
        }
    }
    return fitness;
}

void printIndividual(Individual individual)
{
    printf("Point 1:\n");
    printPoint(individual.p1);
    printf("Point 2:\n");
    printPoint(individual.p2);
    printf("Width: %d\n", individual.width);
    printf("Fitness: %f\n", individual.fitness);
}

Individual initIndividual(Individual seed)
{
    Individual individual;
    individual.p1.i = seed.p1.i + fmax(0, rand() % 50);
    individual.p1.j = seed.p1.j + fmax(0, -25 + rand() % 50);
    individual.p2.i = seed.p2.i + fmax(0, -25 + rand() % 50);
    individual.p2.j = seed.p2.j + fmax(0, -25 + rand() % 50);
    individual.width = seed.width + fmax(0, -25 + rand() % 50);
    return individual;
}

Individual *initPopulation(Individual seed, int n)
{
    Individual *population = (Individual *)malloc(sizeof(Individual) * n);
    for (int i = 0; i < n; i++)
    {
        population[i] = initIndividual(seed);
    }
    return population;
}

void fixIndividual(Individual *individual)
{
    if (individual->p1.i > individual->p2.i)
    {
        swapPoint(&individual->p1, &individual->p2);
    }
}

void fixPopulation(Individual *population, int n)
{
    for (int i = 0; i < n; i++)
    {
        fixIndividual(&population[i]);
    }
}

void setFitness(Mat mat, Individual *population, int n)
{
    for (int i = 0; i < n; i++)
    {
        objetiveFunction(mat, &population[i]);
    }
}

void binaryTournamet(Individual *population, int n, Individual *populationAux)
{
    for (int i = 0; i < n; i++)
    {
        int a = rand() % n;
        int b = rand() % n;
        if (population[a].fitness > population[b].fitness)
        {
            populationAux[i] = population[a];
        }
        else
        {
            populationAux[i] = population[b];
        }
    }
    copyPopulation(populationAux, population, n);
}

void copyPopulation(Individual *src, Individual *dst, int n)
{
    memcpy(dst, src, n * sizeof(Individual));
}

Individual getElite(Individual *population, int n)
{
    double max = population[0].fitness;
    int index = 0;
    for (int i = 1; i < n; i++)
    {
        if (max < population[i].fitness)
        {
            max = population[i].fitness;
            index = i;
        }
    }
    return population[index];
}

void crossover(Individual *population, int n)
{

    for (int i = 0; i + 1 < n; i += 2)
    {
        if (rand() % 100 < crossoverProbability)
        {
            int aux = population[i].p1.i;
            population[i].p1.i = population[i + 1].p1.i;
            population[i + 1].p1.i = aux;
        }
        if (rand() % 100 < crossoverProbability)
        {
            int aux = population[i].p2.i;
            population[i].p2.i = population[i + 1].p2.i;
            population[i + 1].p2.i = aux;
        }
        if (rand() % 100 < crossoverProbability)
        {
            int aux = population[i].width;
            population[i].width = population[i + 1].width;
            population[i + 1].width = aux;
        }
    }
}

void mutation(Individual *population, int n)
{

    for (int i = 0; i < n; i++)
    {
        if (rand() % 100 < mutationProbability)
        {
            switch (rand() % 5)
            {
            case 0:
                population[i].p1.i = fmax(0, rand() % 50);
                break;
            case 1:
                population[i].p1.j = fmax(0, -25 + rand() % 50);
                break;
            case 2:
                population[i].p2.i = fmax(0, rand() % 50);
                break;
            case 3:
                population[i].p2.j = fmax(0, -25 + rand() % 50);
                break;
            case 4:
                population[i].width = fmax(1, -25 + rand() % 50);
                break;
            }
        }
    }
}

Individual optimize(Mat mat, Individual seed)
{
    srand(time(NULL));
    Individual elite;
    int populationSize = 50;
    Individual *populationAux = (Individual *)malloc(sizeof(Individual) * populationSize);
    Individual *population = initPopulation(seed, populationSize);

    int ite = 0;
    int conv = 0;
    double lfitness = 0;
    do
    {
        //fixPopulation(population, populationSize);
        setFitness(mat, population, populationSize);
        elite = getElite(population, populationSize);
        binaryTournamet(population, populationSize, populationAux);
        population[0] = elite;
        crossover(population, populationSize);
        mutation(population, populationSize);
        printIndividual(elite);
        printf("ite = %d\n", ite++);
        if (fabs(elite.fitness - lfitness) < 0.00000001)
        {
            conv++;
        }
        else
        {
            conv = 0;
        }
        lfitness = elite.fitness;
    } while (elite.fitness < 10000 && ite < 10000 && conv < 200);
    return elite;
}
