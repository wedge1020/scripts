#!/usr/bin/env -S tcc -run
#include <stdio.h>
#include <stdlib.h>

union intfl
{
	unsigned int  value;
	float         data;
};
typedef union intfl flint;  

int main (int argc, char **argv)
{
	flint  word;
	int    index            = 0;

	for (index = 1; index < argc; index++)
	{
		word.data           = strtof (argv[index], NULL);
		fprintf (stdout, "[%.3f] 0x%.8X\n", word.data, word.value);
	}
	return (0);
}
