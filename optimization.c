#include "optimization.h"

void objetiveFunction(Mat mat, Individual *individual)
{

    Point pp1, pp2, p3, p4;
    individual->fitness = 0;
    if (equalsPoints(individual->p1, individual->p2))
    {
        individual->fitness = -1;
        printf("Hola1\n");
        return;
    }

    p3 = transformPoint2(individual->p2, individual->p1, individual->width, -M_PI_2);
    p4 = transformPoint2(individual->p1, individual->p2, individual->width, M_PI_2);

    if (validatePoint(mat, p3) == 0 || validatePoint(mat, p4) == 0)
    {
        individual->fitness = -1;
        printf("Hola2 w = %d\n", individual->width);
        return;
    }

    int steps, steps1, steps2;
    float Iincrement1, Jincrement1, Iincrement2, Jincrement2;
    getDDAdata(individual->p1, p4, &Iincrement1, &Jincrement1, &steps1);
    getDDAdata(individual->p2, p3, &Iincrement2, &Jincrement2, &steps2);

    steps = fmin(steps1, steps2);

    float i1, i2, j1, j2;
    i1 = individual->p1.i;
    i2 = individual->p2.i;
    j1 = individual->p1.j;
    j2 = individual->p2.j;

    for (int v = 0; v <= steps; v++)
    {
        pp1.i = round(i1);
        pp1.j = round(j1);
        pp2.i = round(i2);
        pp2.j = round(j2);
        individual->fitness += DDAlinealObjetiveFunction(mat, pp1, pp2);
        i1 += Iincrement1;
        j1 += Jincrement1;
        i2 += Iincrement2;
        j2 += Jincrement2;
    }

    individual->fitness += getDistance(individual->p1, individual->p2) * 12;
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
            fitness -= 10;
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
    individual.p1.i = seed.p1.i - 25 + rand() % 50;
    individual.p1.j = seed.p1.j - 25 + rand() % 50;
    // individual.p2.i = seed.p2.i + individual.p1.i - 25 + rand() % 50;
    // individual.p2.j = seed.p2.j + individual.p1.j - 25 + rand() % 50;
    individual.p2.i = seed.p2.i - 25 + rand() % 50;
    individual.p2.j = seed.p2.j - 25 + rand() % 50;
    individual.width = fmax(2, seed.width - 20 + rand() % 50);

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

void fixIndividual(Individual *individual, Mat mat)
{
    while (equalsPoints(individual->p1, individual->p2) == 1)
    {
        *individual = initIndividual(*individual);
    }

    fixPoint(mat, &individual->p1);
    fixPoint(mat, &individual->p2);

    Point p3 = transformPoint2(individual->p2, individual->p1, individual->width, -M_PI_2);
    while (validatePoint(mat, p3) == 0 && individual->width >= MIN_WIDTH)
    {
        individual->width--;
        p3 = transformPoint2(individual->p2, individual->p1, individual->width, -M_PI_2);
    }

    Point p4 = transformPoint2(individual->p1, individual->p2, individual->width, M_PI_2);
    while (validatePoint(mat, p4) == 0 && individual->width >= MIN_WIDTH)
    {
        individual->width--;
        p4 = transformPoint2(individual->p1, individual->p2, individual->width, M_PI_2);
    }

    if (individual->width < MIN_WIDTH)
    {
        *individual = initIndividual(*individual);
        fixIndividual(individual, mat);
    }
}

void fixPopulation(Individual *population, int n, Mat mat)
{
    for (int i = 0; i < n; i++)
    {
        fixIndividual(&population[i], mat);
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
                population[i].p1.i = population[i].p1.i - 25 + rand() % 50;
                break;
            case 1:
                population[i].p1.j = population[i].p1.j - 25 + rand() % 50;
                break;
            case 2:
                population[i].p2.i = population[i].p2.i - 15 + rand() % 50;
                break;
            case 3:
                population[i].p2.j = population[i].p2.j - 15 + rand() % 50;
                break;
            case 4:
                population[i].width = fmax(MIN_WIDTH, population[i].width - 20 + rand() % 50);
                break;
            }
        }
    }
}

Individual optimize(Mat mat, Individual seed, char *filename)
{
    FILE *file = fopen(filename, "w");
    srand(time(NULL));
    Individual elite;
    int populationSize = 100;
    Individual *populationAux = (Individual *)malloc(sizeof(Individual) * populationSize);
    Individual *population = initPopulation(seed, populationSize);
    fixPopulation(population, populationSize, mat);

    int ite = 0;
    int conv = 0;
    double lfitness = 0;
    do
    {
        setFitness(mat, population, populationSize);
        elite = getElite(population, populationSize);
        binaryTournamet(population, populationSize, populationAux);
        crossover(population, populationSize);
        fixPopulation(population, populationSize, mat);
        mutation(population, populationSize);
        fixPopulation(population, populationSize, mat);
        population[0] = elite;
        //printIndividual(elite);
        //printf("ite = %d\n", ite++);
        if (fabs(elite.fitness - lfitness) < 0.000000001)
        {
            conv++;
        }
        else
        {
            conv = 0;
        }
        lfitness = elite.fitness;
        ite++;
        fprintf(file, "%d,%f\n", ite, lfitness);
    } while (elite.fitness < 1000000 && ite < 10000 && conv < 600);

    free(populationAux);
    free(population);
    printIndividual(elite);

    return elite;
}

void dataToFile(char *filename, Individual *individual)
{
    FILE *file = fopen(filename, "w");
    fprintf(file, "p1 i,p1 j,p2 i,p2 j,p3 i,p3 j,p4 i,p4 j,p5 i,p5 j,l1,l2,l3,l4,Angulo 1,Angulo 2,Angulo 3,Angulo 4\n");
    for (int i = 0; i < 4; i++)
    {
        fprintf(file, "%d,%d,", individual[i].p1.i, individual[i].p1.j);
    }
    fprintf(file, "%d,%d,", individual[3].p2.i, individual[3].p2.j);
    for (int i = 0; i < 4; i++)
    {
        fprintf(file, "%f,", getDistance(individual[i].p1, individual[i].p2));
    }
    for (int i = 0; i < 4; i++)
    {
        fprintf(file, "%f,", radianToDegree(getAngle(individual[i].p1, individual[i].p2)));
    }
    fclose(file);
}

void optimizeStructure(char *image)
{
    char filename[FILENAME_LENGTH];
    createFilename(filename, IN_IMAGES_FOLDER, image);
    Mat pgm = openPGM(filename);
    Mat optimizeImage = getTrimmedImage(pgm);
    Mat solutionImage = copyMat(optimizeImage);
    Mat seedImage = copyMat(optimizeImage);
    Mat conectedImage = copyMat(optimizeImage);

    createFilename(filename, CROPPED_IMAGE_FOLDER, image);
    pgmToFile(optimizeImage, filename);

    Individual seed[4];
    Individual solution[4];

    for (int i = 0; i < 4; i++)
        seed[i].width = 10;

    seed[0].p1.i = 0;
    for (seed[0].p1.j = 0; optimizeImage.data[seed[0].p1.i][seed[0].p1.j] == 0; seed[0].p1.j++)
        ;

    getFinalPoint(optimizeImage, seed[0].p1.i, seed[0].p1.j, &seed[0].p2.i, &seed[0].p2.j, tan(degreeToRadian(-45)));

    createFilename(filename, DATA_FOLDER, image);
    dataToFile(strcat(filename, "fitness_1.csv"), solution);
    solution[0] = optimize(optimizeImage, seed[0], filename);

    drawRect(optimizeImage, 0, solution[0].p1, solution[0].p2, solution[0].width);

    seed[1].p1 = greatPointI(solution[0].p1, solution[0].p2);
    seed[1].p1.j += 5;

    getFinalPoint(solutionImage, seed[1].p1.i, seed[1].p1.j, &seed[1].p2.i, &seed[1].p2.j, tan(degreeToRadian(45)));

    createFilename(filename, DATA_FOLDER, image);
    dataToFile(strcat(filename, "fitness_2.csv"), solution);
    solution[1] = optimize(optimizeImage, seed[1], filename);

    drawRect(optimizeImage, 0, solution[1].p1, solution[1].p2, solution[1].width);

    seed[2].p1 = farestPoint(seed[1].p1, solution[1].p1, solution[1].p2);

    getFinalPoint(solutionImage, seed[2].p1.i, seed[2].p1.j, &seed[2].p2.i, &seed[2].p2.j, tan(degreeToRadian(-45)));

    createFilename(filename, DATA_FOLDER, image);
    dataToFile(strcat(filename, "fitness_3.csv"), solution);
    solution[2] = optimize(optimizeImage, seed[2], filename);

    drawRect(optimizeImage, 0, solution[2].p1, solution[2].p2, solution[2].width);

    seed[3].p1 = farestPoint(solution[1].p2, solution[2].p1, solution[2].p2);

    getFinalPoint(solutionImage, seed[3].p1.i, seed[3].p1.j, &seed[3].p2.i, &seed[3].p2.j, tan(degreeToRadian(45)));

    createFilename(filename, DATA_FOLDER, image);
    dataToFile(strcat(filename, "fitness_4.csv"), solution);
    solution[3] = optimize(optimizeImage, seed[3], filename);

    int color = 50;
    for (int i = 0; i < 4; i++, color += 50)
    {
        // printf("Seed %d\n", i);
        // printIndividual(seed[i]);
        // printf("Solution %d\n", i);
        // printIndividual(solution[i]);
        drawRect(seedImage, color, seed[i].p1, seed[i].p2, seed[i].width);
        drawRect(solutionImage, color, solution[i].p1, solution[i].p2, solution[i].width);
        if (i != 3)
        {
            //solution[i].p2 = closestPoint(solution[i].p2, solution[i + 1].p1, solution[i + 1].p2);
            solution[i].p2 = solution[i + 1].p2;
            fixIndividual(&solution[i], conectedImage);
        }
        drawRect(conectedImage, color, solution[i].p1, solution[i].p2, solution[i].width);
    }

    createFilename(filename, SOLUTION_IMAGE_FOLDER, image);
    pgmToFile(solutionImage, filename);
    createFilename(filename, SEED_IMAGE_FOLDER, image);
    pgmToFile(seedImage, filename);
    createFilename(filename, FINAL_SOLUTION_IMAGE_FOLDER, image);
    pgmToFile(conectedImage, filename);
    createFilename(filename, DATA_FOLDER, image);
    dataToFile(strcat(filename, ".csv"), solution);
    dataToFile(strcat(filename, "seed.csv"), seed);

    freeMat(pgm);
    freeMat(optimizeImage);
    freeMat(solutionImage);
    freeMat(seedImage);
    freeMat(conectedImage);
}