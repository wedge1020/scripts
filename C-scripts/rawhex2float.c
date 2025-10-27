#!/usr/bin/env -S tcc -run
//
// rawhex2float.c: A C-Script which takes, as a command-line argument, a raw hex
//                 value, stores it in a union shared with a float, and proceeds
//                 to display it in corresponding floating point form,  in order
//                 to better study the IEEE-754 format.
//                 for purposes of studying the IEEE-754 format
//
//         usage - rawhex2float HEX [HEX..]
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
// its integer member or floating point member
//
union intfl
{
    uint64_t  value;
    float     data;
};
typedef union intfl FlInt;

int32_t  main (int32_t  argc, int8_t **argv)
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
        // Store the converted-to-integer number into the value element of the union
        //
        word.value          = strtol (*(argv+index), NULL, 16);

        ////////////////////////////////////////////////////////////////////////////////
        //
        // Display the floating-point representation of the information to STDOUT,
        // along with binary, hex, and IEEE-754 bit layouts
        //
        fprintf (stdout, "hexadecimal:   0x%.8X\n", word.value);
        fprintf (stdout, "binary:        %.32b\n",  word.value);
        fprintf (stdout, "IEEE-754 bits: SEEEEEEEEMMMMMMMMMMMMMMMMMMMMMMM\n");
        fprintf (stdout, "float:         %.6f\n", word.data);
        fprintf (stdout, "===============================================\n");
    }

    return (0);
}
