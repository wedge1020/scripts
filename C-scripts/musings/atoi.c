#!/usr/bin/env -S tcc -run
//
// atoi - command-line `atoi` tool as C-script; like the C standard library
//        function of the same name, takes a token of argv data, converting
//        it into a signed integer,  displaying it to STDOUT.  If not given
//        any arguments, read a value from STDIN.
//
// usage: atoi STRING [STRING...]
//
// NOTE:  behaviour can be altered with the defining of some variables with
// the indicated values:
//
// variable   value          description
// ========   ============   =========================================================
//    DEBUG   "true"         enable debug mode, generating operational output
//   ENDING   string         string to use for line ending (default is "\n")
//   FORMAT   format token   '%d' for signed int, '%u' for unsigned, '%x' for hex
//                           (default is "%d")
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//
// A note on this C-script, and why it is in musings/:
//
// In many respects, the functionality of this tool is utterly pointless,
// as there exists no distinction between strings and integers in bash on
// the command-line,  where this  tool is meant  to be  used. Development
// started from  a much simpler  tool, and I thought  to chase a  line of
// functionality that moved it so far beyond usefulness it is amazing.
//
// But: usefulness isn't  necessarily the only reason  for such pursuits;
// my interest was driven by  exploring some concepts of operability very
// much worth  my time, and  could end up being  used as a  reference for
// other scripts down the road. For instance, I forked this script into a
// C-script fprintf.c, until that too proved too much of an uphill battle
// to work to completion  (and that is present here too  as a musing, for
// similar reasons).
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//
// Pre-processor directives
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int32_t  main (int32_t  argc, int8_t **argv)
{
    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Declare and initialize local variables
    //
    int8_t   byte        = 0;
    int8_t  *debug       = NULL; // NULL or "true" (latter enables debug mode)
    int8_t  *ending      = NULL; // line ending
    int8_t  *format      = NULL;
    int8_t  *input       = NULL;
    int32_t  index       = 0;
    int32_t  result      = 0;

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Check for the existence of the DEBUG variable, assign a default of NULL if
    // no match
    //
    debug                = getenv ("DEBUG");

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Check for the existence of the ENDING variable, assign a default of "\n" if no
    // match
    //
    ending               = getenv ("ENDING");
    if (ending          == NULL)
    {
        ending           = (int8_t *) malloc (sizeof (int8_t) * 2);
        if (ending      == NULL)
        {
            fprintf (stderr, "[ERROR] Could not allocate memory for 'ending'\n");
            exit (1);
        }

        *(ending+0)      = '\n';
        *(ending+1)      = '\0';
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Check for the existence of the FORMAT variable, assign a default of "%d" if no
    // match
    //
    format               = getenv ("FORMAT");
    if (format          == NULL)
    {
        format           = (int8_t *) malloc (sizeof (int8_t) * 3);
        if (format      == NULL)
        {
            fprintf (stderr, "[ERROR] Could not allocate memory for 'format'\n");
            exit (1);
        }

        *(format+0)      = '%';
        *(format+1)      = 'd';
        *(format+2)      = '\0';
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Debugging output
    //
    if (debug           != NULL)
    {
        fprintf (stderr, "[debug] argument count:   %d\n",     argc);
        fprintf (stderr, "[debug] format specifier: \"%s\"\n", format);
        fprintf (stderr, "[debug] line ending:      0x%hhX\n", *(ending+0));
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // For each argv token provided, do an atoi conversion
    //
    for (index = 1; index < argc; index++)
    {
        ////////////////////////////////////////////////////////////////////////////////
        //
        // Point `input` at the current argv token
        //
        input            = *(argv+index);

        ////////////////////////////////////////////////////////////////////////////////
        //
        // If in debug mode, display more information about the transaction
        //
        if (debug       != NULL)
        {
            fprintf (stderr, "[debug] *(argv+%d): \"%s\" -> integer: ", index, input);
        }

        ////////////////////////////////////////////////////////////////////////////////
        //
        // Call atoi(3) and convert the string `input` to our integer `output`
        //
        result           = atoi (input);

        ////////////////////////////////////////////////////////////////////////////////
        //
        // Display the resulting atoi(3) returned value to STDOUT, in accordance
        // with the desired formatting
        //
        fprintf (stdout, format, result);
        fprintf (stdout, ending);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // If no arguments provided, obtain individual characters from STDIN (until EOF),
    // displaying them to STDOUT in the desired format
    //
    if (argc            == 1)
    {
        while (!feof (stdin))
        {
            byte         = fgetc (stdin);
            fprintf (stdout, format, byte);
            fprintf (stdout, ending);
        }
    }

    return (0);
}
