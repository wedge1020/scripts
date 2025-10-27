#!/usr/bin/env -S tcc -run
//
// float2rawhex.c: A C-Script which takes, as a command-line argument, a floating-
//                 point value, and stores it in a union shared with an integer, in
//                 order to display the raw hexadecimal representation of the data,
//                 for purposes of studying the IEEE-754 format
//
//         usage - float2rawhex FLOAT [FLOAT..]
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//
// Pre-processor directives
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////
//
// A delightful application of a union: quick and easy access to the storage via
// its integer member
//
union intfl
{
    uint32_t  value;
    float     data;
};
typedef union intfl FlInt;

int32_t  main (int32_t  argc, uint8_t **argv)
{
    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Declare variables
    //
    FlInt     word;
    int32_t   index         = 0;

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // For each argument given
    //
    for (index = 1; index < argc; index++)
    {
        ////////////////////////////////////////////////////////////////////////////////
        //
        // Store the converted-to-float number into the data element of the union
        //
        word.data           = strtof (*(argv+index), NULL);

        ////////////////////////////////////////////////////////////////////////////////
        //
        // Display the raw hexadecimal representation of the information to STDOUT,
        // along with binary and IEEE-754 bit layouts
        //
        fprintf (stdout, "float:         %.6f\n", word.data);
        fprintf (stdout, "hexadecimal:   0x%.8X\n", word.value);
        fprintf (stdout, "binary:        %.32b\n",  word.value);
        fprintf (stdout, "IEEE-754 bits: SEEEEEEEEMMMMMMMMMMMMMMMMMMMMMMM\n");
        fprintf (stdout, "===============================================\n");
    }

    return (0);
}
