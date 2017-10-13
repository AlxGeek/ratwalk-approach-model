#include "util.h"

int readArgs(int argc, char *argv[], FILE **fileclass1, FILE **fileclass2)
{
	if (argc != 5)
	{
		printf("Uso: ./clasnn -fileclass1 matrizclase1.dat -fileclass2 matrizclase2.dat\n");
		return -1;
	}
	char *filename;
	if (strcmp("-fileclass1", argv[1]) == 0)
		filename = argv[2];
	else if (strcmp("-fileclass1", argv[3]) == 0)
		filename = argv[4];
	else
	{
		printf("Error, no se proporciono el nombre de fileclass1\n");
		return -1;
	}
	if ((*fileclass1 = fopen(filename, "r")) == NULL)
	{
		printf("Error al abrir el archivo: %s\n", filename);
		return -1;
	}
	if (strcmp("-fileclass2", argv[1]) == 0)
		filename = argv[2];
	else if (strcmp("-fileclass2", argv[3]) == 0)
		filename = argv[4];
	else
	{
		printf("Error, no se proporciono el nombre de fileclass2\n");
		return -1;
	}
	if ((*fileclass2 = fopen(filename, "r")) == NULL)
	{
		printf("Error al abrir el archivo: %s\n", filename);
		return -1;
	}
	return 1;
}

int compare(const void *a, const void *b)
{
	return (*(double *)b * 1000 - *(double *)a * 1000);
}

void createFilename(char *filename, char *str1, char *str2)
{
	strcpy(filename, "");
	strcat(filename, str1);
	strcat(filename, str2);
}