#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// this program outputs a reference genome of size LENSEQ with NCHROM

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf("ERROR: CHECK INPUTS\n./simRef OPFILE.fasta LENSEQ NCHROM\n");
		return(1); 
	}

	FILE *outf;
	outf = fopen(argv[1], "w");
	if (outf == NULL)
	{
		printf("ERROR: FILE CREATION ERROR\n");
		return(2);
	}
	
	int lseq;
	int nchrom;
	sscanf(argv[2], "%i", &lseq); 
	sscanf(argv[3], "%i", &nchrom);
	long seed = time(NULL);
	srand(seed);
	int opLen = 90;

	char formatOut = 0; // this should change with user input


	// make the chromosome counts
	int *chromSize = malloc(sizeof(int) * nchrom);
	int holdLseq = lseq;

	// we need some mimimum value for each chrom
	int minVal = (lseq / nchrom) / 3;

	int partLseq = (lseq - minVal * nchrom) / nchrom;


	// do this for each chrom
	for (int j = 0; j < nchrom - 1; j++)
	{
		chromSize[j] = rand() % partLseq + minVal;
		holdLseq = holdLseq - chromSize[j];
	}

	chromSize[nchrom - 1] = holdLseq;

	for (int k = 0; k < nchrom; k++)
	{
		fprintf(outf, ">chrom%i %i\n", k + 1, chromSize[k]);

		for (int i = 0; i < chromSize[k]; i++)
		{
			if (formatOut == 1)
			{
				if (i % opLen == 0 && i != 0)
				{
					fprintf(outf, "\n");
				}
			}
			char opt = 0;
	
			if (rand() % 2 == 1)
			{
				opt = opt + 65;
				if (rand() % 2 == 1)
				{
					opt = opt + 19;
				}
			}
			else
			{
				opt = opt + 67;
				if (rand() % 2 == 1)
				{
					opt = opt + 4;
				}
			}

			fprintf(outf, "%c", opt);
		}
		fprintf(outf, "\n");
	}

	free(chromSize);
	fclose(outf);
	
}