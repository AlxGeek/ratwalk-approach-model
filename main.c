#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "memo.h"
#include "Mat.h"
#include "util.h"
#include "optimization.h"
#include "Draw.h"

int main(int argc, char *argv[])
{
	srand(time(NULL));
	Mat pgm = openPGM( "image.pgm");
	Mat trimImg = getTrimmedImage(pgm);
	Mat optimizeImage = copyMat(trimImg);
	Mat solutionImage = copyMat(trimImg);
	Mat seedImage = copyMat(trimImg);
	Mat conectedImage = copyMat(trimImg);

	Individual seed[4];
	Individual solution[4];
	for (int i = 0; i < 4; i++)
		seed[i].width = 10;

	seed[0].p1.i = 0;
	for (seed[0].p1.j = 0; optimizeImage.data[seed[0].p1.i][seed[0].p1.j] == 0; seed[0].p1.j++)
		;

	getFinalPoint(optimizeImage, seed[0].p1.i, seed[0].p1.j, &seed[0].p2.i, &seed[0].p2.j, tan(-45 * M_PI / 180));

	solution[0] = optimize(optimizeImage, seed[0]);

	drawRect(optimizeImage, 0, solution[0].p1, solution[0].p2, solution[0].width);

	seed[1].p1 = solution[0].p2;
	seed[1].p1.i -= 10;

	getFinalPoint(solutionImage, seed[1].p1.i, seed[1].p1.j, &seed[1].p2.i, &seed[1].p2.j, tan(45 * M_PI / 180));

	solution[1] = optimize(optimizeImage, seed[1]);

	drawRect(optimizeImage, 0, solution[1].p1, solution[1].p2, solution[1].width);

	seed[2].p1 = solution[1].p2;
	seed[2].p1.i += 10;

	getFinalPoint(solutionImage, seed[2].p1.i, seed[2].p1.j, &seed[2].p2.i, &seed[2].p2.j, tan(-45 * M_PI / 180));

	solution[2] = optimize(optimizeImage, seed[2]);

	drawRect(optimizeImage, 0, solution[2].p1, solution[2].p2, solution[2].width);

	seed[3].p1 = solution[2].p2;

	getFinalPoint(solutionImage, seed[3].p1.i, seed[3].p1.j, &seed[3].p2.i, &seed[3].p2.j, tan(45 * M_PI / 180));

	solution[3] = optimize(optimizeImage, seed[3]);

	for (int i = 0; i < 4; i++)
	{
		printf("Seed %d\n", i);
		printIndividual(seed[i]);
		printf("Solution %d\n", i);
		printIndividual(solution[i]);
		drawRect(seedImage, 255 / 2, seed[i].p1, seed[i].p2, seed[i].width);
		drawRect(solutionImage, 255 / 2, solution[i].p1, solution[i].p2, solution[i].width);
		if (i != 3)
			solution[i].p2 = solution[i + 1].p1;
		drawRect(conectedImage, 255 / 2, solution[i].p1, solution[i].p2, solution[i].width);
	}

	pgmToFile(optimizeImage, "optimizeImage.pgm");
	pgmToFile(solutionImage, "solution.pgm");
	pgmToFile(seedImage, "seed.pgm");
	pgmToFile(conectedImage, "conected.pgm");

	dataToFile("data.csv", solution);

	freeMat(optimizeImage);
	freeMat(solutionImage);
	freeMat(seedImage);
	freeMat(conectedImage);

	return 0;
}