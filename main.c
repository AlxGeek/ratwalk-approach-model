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
	char filename[FILENAME_LENGTH];
	
	for(int i = 1; i <= 21; i++){		
		sprintf(filename, "%d.pgm", i);
		printf("%s\n",filename);
		optimizeStructure(filename);
	}

	return 0;
}