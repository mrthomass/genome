#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// this file really only supports fasta files

struct Chrome 
{
	unsigned int len;
	unsigned long pos;
};

int main(int argc, char *argv[])
{
	if (argc != 6)
	{
		printf("ERROR: CHECK INPUTS\n");
		printf("./simRead REFFILE.fa READFILE.fa nRead minReadLen deltaUpRlen\n");
		return(1);
	}
	
	FILE *inpf;
	FILE *optf;
	inpf = fopen(argv[1], "r");
	optf = fopen(argv[2], "w");
	if (inpf == NULL || optf == NULL)
	{
		printf("ERROR: FILES NOT FOUND\n");
		return(2);
	}

	unsigned short nchrom = 0;
	while (!feof(inpf))
	{
		fscanf(inpf, ">%*[^>]");
		nchrom++;
		
	}
	rewind(inpf);


	unsigned int nRead;
	sscanf(argv[3], "%u", &nRead);
	unsigned int minRead;
	sscanf(argv[4], "%u", &minRead);
	unsigned int deltaUpRead;
	sscanf(argv[5], "%u", &deltaUpRead);


	unsigned int minChromLen;
	struct Chrome *chromLen = malloc(sizeof(struct Chrome) * nchrom);

	for (int i = 0; i < nchrom; i++)
	{
		fscanf(inpf, ">%*[^ ] %i\n", &chromLen[i].len);
		chromLen[i].pos = ftell(inpf); // this one is to adjust for the first character of the chrom or scaf
		fscanf(inpf, "%*[^>]");
		if (i == 0)
		{
			minChromLen = chromLen[i].len;
		}
		if (minChromLen > chromLen[i].len)
		{
			minChromLen = chromLen[i].len;
		}
	}

	if (minRead + deltaUpRead > minChromLen)
	{
		fclose(inpf);
		fclose(optf);
		free(chromLen);
		printf("ERROR: READ LENGTH AND DELTA TOO BIG FOR THIS INPUT\n");
		return(3);
	}


	long SEED = time(NULL);
	srand(SEED);	
	
	// first we need to know how which chromosome we need to go to, then we need a chrom len, then we need to find a starting position

	// temporary buffer variables
	unsigned int thisLen;
	unsigned short thisChrom;
	unsigned int startPos;
	char megaBuff;

	for (int i = 0; i < nRead; i++)
	{
		thisLen = minRead + rand() % (deltaUpRead + 1); // this is because we need the users delta to be actual
		thisChrom = rand() % nchrom;

		if (thisLen != chromLen[thisChrom].len)
		{
			startPos = rand() % (chromLen[thisChrom].len - thisLen + 1);
		}
		else
		{
			startPos = 0;
		}

		fseek(inpf, chromLen[thisChrom].pos + startPos, SEEK_SET);

		fprintf(optf, ">%i %u\n", i + 1, thisLen);
		
		for (int j = 0; j < thisLen; j++)
		{
			fscanf(inpf, "%c", &megaBuff);
			fprintf(optf, "%c", megaBuff);
		}

		fprintf(optf, "\n");

	}


	free(chromLen);
	fclose(inpf);
	fclose(optf);
	return(0);
	
}