#!/usr/bin/env -S tcc -run
#include <stdio.h>
#include <stdlib.h>

union intfl
{
	unsigned long int  value;
	float              data;
};
typedef union intfl flint;  

int main (int argc, char **argv)
{
	flint  word;
	int    index            = 0;

	for (index = 1; index < argc; index++)
	{
		word.value          = strtol (argv[index], NULL, 16);
		fprintf (stdout, "[0x%.8lX] %.3f\n", word.value, word.data);
	}
	return (0);
}
