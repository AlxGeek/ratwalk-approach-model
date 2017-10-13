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
	Mat pgm = openPGM("image.pgm");
	Mat trimImg = getTrimmedImage(pgm);
	Mat cpy = copyMat(trimImg);
	Mat imgTest = copyMat(trimImg);

	Individual seed[4];
	Individual solution[4];
	for (int i = 0; i < 4; i++)
		seed[i].width = 10;

	seed[0].p1.i = 0;
	for (seed[0].p1.j = 0; imgTest.data[seed[0].p1.i][seed[0].p1.j] == 0; seed[0].p1.j++);

	getFinalPoint(imgTest, seed[0].p1.i, seed[0].p1.j, &seed[0].p2.i, &seed[0].p2.j, 255 / 2, tan(-45 * M_PI / 180));

	solution[0] = optimize(imgTest, seed[0]);

	drawRect(imgTest, 0, solution[0].p1, solution[0].p2, solution[0].width);
	
	pgmToFile(imgTest, "test.pgm");

	seed[1].p1 = solution[0].p2;

	getFinalPoint(trimImg, seed[1].p1.i, seed[1].p1.j, &seed[1].p2.i, &seed[1].p2.j, 255 / 2, tan(45 * M_PI / 180));
	
	solution[1] = optimize(imgTest, seed[1]);
	
	


	for(int i = 0; i<2;i++){
		drawRect(trimImg, 255 / 2, solution[i].p1, solution[i].p2, solution[i].width);
		drawRect(cpy, 255 / 2, seed[i].p1, seed[i].p2, seed[i].width);
	}
	
	pgmToFile(trimImg, "nuevaElite.pgm");
	pgmToFile(cpy, "initial.pgm");
	return 0;
}