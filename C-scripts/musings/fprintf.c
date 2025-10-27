#!/usr/bin/env -S tcc -run
//
// fprintf - command-line `fprintf` tool as a tcc C-script; like the C library
//           function of the same name, takes a token of argv data, displaying
//           it to an indicated  file stream in accordance with a given format
//           specifier. If not given any arguments, read a value from STDIN.
//
//    usage: fprintf FILE FORMAT STRING [STRING...]
//
//     NOTE:  behaviour can be altered with the defining of some variables with
//            the indicated values:
//
// variable   value          description
// ========   ============   =========================================================
//    DEBUG   "true"         enable debug mode, generating operational output
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//
// A note on this C-script, and why it is in musings/:
//
// This  script  started  as  a  fork of  atoi.c,  once  I  realized  how
// useless  that one  had  become; I  realized what  I  could really  use
// was  a  command-line `fprintf`,  with  much  of  the same  layout  and
// functionality, so I started branching out in that direction.
//
// Ultimately, while  the intended  functionality of this  script remains
// useful, to pull off  the needed logic to do so  was more involved than
// the  time and  approach  I had  at  the time,  so I  rewrote  it as  a
// bash  script. Still,  there are  some interesting  details here  worth
// preserving for future  study. And who knows: maybe some  day I'll feel
// inclined to complete this implementation.
//
// What I think I will need to do, sticking to my current approach, is to
// explore va_args,  or otherwise finding  some way  to pass an  array or
// stack of parameters  to fprintf, without actually having  to spell out
// each parameter individually  (hence I why I diverted to  a bash script
// where I had considerably more flexibility).
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//
// Pre-processor directives
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int32_t  main (int32_t  argc, int8_t **argv)
{
    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Declare and initialize local variables
    //
    FILE    *stream      = NULL;
    int8_t   byte        = 0;
    int8_t  *debug       = NULL; // NULL or "true" (latter enables debug mode)
    int8_t   flag        = 0;
    int8_t  *format      = NULL;
    int8_t  *input       = NULL;
    int32_t  index       = 0;
    int32_t  offset      = 0;
    int32_t  result      = 0;

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Check for the existence of the DEBUG variable, assign a default of NULL if
    // no match
    //
    debug                = getenv ("DEBUG");

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Ensure we have the minimal number of arguments at runtime
    //
    if (argc            <  4)
    {
        fprintf (stderr, "[ERROR] Inadequate arguments specified!\n");
        exit (1);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Determine file stream output; first up: stdout
    //
    result               = strncasecmp ("stdout", *(argv+1), strlen ("stdout"));
    if (result          == 0)
    {
        stream           = stdout;
        if (debug       != NULL)
        {
            fprintf (stderr, "[debug] file stream:      stdout\n");
        }
    }

    else
    {
        ////////////////////////////////////////////////////////////////////////////////
        //
        // Determine file stream output; next up: stderr
        //
        result           = strncasecmp ("stderr", *(argv+1), strlen ("stderr"));
        if (result      == 0)
        {
            stream       = stderr;
            if (debug   != NULL)
            {
                fprintf (stderr, "[debug] file stream:      stderr\n");
            }
        }

        ////////////////////////////////////////////////////////////////////////////////
        //
        // Determine file stream output; custom file is specified
        //
        else
        {
            stream       = fopen (*(argv+1), "w");
            if (stream  == NULL)
            {
                fprintf (stderr, "[ERROR] Could not open '%s' for writing!\n",
                        *(argv+1));
                exit (2);
            }

            if (debug   != NULL)
            {
                fprintf (stderr, "[debug] file stream:      '%s'\n",   *(argv+1));
            }

            ////////////////////////////////////////////////////////////////////////////
            //
            // Set `flag` so we know to fclose(3) the file when done
            //
            flag         = 1;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Populate `format` with formatted format specifier information
    //
    offset                                   = sizeof (int8_t);
    offset                                   = offset * (strlen (*(argv+2)) + 1);
    format                                   = (int8_t *) malloc (offset);
    if (format                              == NULL)
    {
        fprintf (stderr, "[ERROR] Could not allocate memory for `format`\n");
        exit (3);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // As some sequences have special meanings (like '\n' and '\t'), we need to look
    // through the data for recognized combinations and 'specialize' them
    //
    offset                                   = 0;
    for (index = 0; index < strlen (*(argv+2)); index++)
    {
        ////////////////////////////////////////////////////////////////////////////////
        //
        // What kicks off specializing is the encountering of a backslash
        //
        if (*(*(argv+2)+index)              == '\\')
        {
            ////////////////////////////////////////////////////////////////////////////
            //
            // If the next character in sequence is an 'n', we have a '\n'
            //
            if (*(*(argv+2)+(index+1))      == 'n')
            {
                *(format+offset)             = '\n';
                index                        = index + 1;
            }

            ////////////////////////////////////////////////////////////////////////////
            //
            // If the next character in sequence is a 't', we have a '\t'
            //
            else if (*(*(argv+2)+(index+1)) == 't')
            {
                *(format+offset)             = '\t';
                index                        = index + 1;
            }

            ////////////////////////////////////////////////////////////////////////////
            //
            // If the next character in sequence is a '\', we have a '\\'
            //
            else if (*(*(argv+2)+(index+1)) == '\\')
            {
                *(format+offset)             = '\\';
                index                        = index + 1;
            }

            ////////////////////////////////////////////////////////////////////////////
            //
            // If the next character isn't a recognized character in a combination,
            // we just have a routine sequence (this likely will cause problems in
            // edge cases)
            //
            else
            {
                *(format+offset)             = *(*(argv+2)+index);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////
        //
        // Average case: copy the character into our `format` string
        //
        else
        {
            *(format+offset)                 = *(*(argv+2)+index);
        }

        ////////////////////////////////////////////////////////////////////////////////
        //
        // Increment `offset`, which manages how far into `format` we are
        //
        offset                               = offset + 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Debugging output
    //
    if (debug           != NULL)
    {
        fprintf (stderr, "[debug] argument count:   %d\n",     argc);
        fprintf (stderr, "[debug] format specifier: \"%s\"\n", *(argv+2));
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // For each argv token provided, do an atoi conversion
    //
    for (index = 3; index < argc; index++)
    {
        ////////////////////////////////////////////////////////////////////////////////
        //
        // Point `input` at the current argv token
        //
        input            = *(argv+index);

        ////////////////////////////////////////////////////////////////////////////////
        //
        // Run fprintf(3) on the `input`, processing via `format` string, outputting
        // to the indicated file `stream`
        //
        fprintf (stream, format, input);
        //fprintf (stdout, format, atoi (input));
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // If flag is set, we have opened a file that needs closing
    //
    if (flag            == 1)
    {
        fclose (stream);
    }

    return (0);
}
