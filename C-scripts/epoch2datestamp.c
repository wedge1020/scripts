#!/usr/bin/env -S tcc -run
//
// epoch2datestamp: a C-script to take some number of seconds since the epoch and
//                  display its datestamp
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//
// Pre-processor directives
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////////////
//
// node structure for use as a linked list
//
struct node
{
    time_t       now;
    struct node *next;
};
typedef struct node Node;

int32_t  main (int32_t  argc, int8_t **argv)
{
    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Useful run-time variables
    //
    Node      *start                     = NULL;
    Node      *tmp                       = NULL;
    char      *datestamp                 = NULL;
    int32_t    index                     = 0;
    struct tm *mytime                    = NULL;

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Allocate (and verify) storage for our 16-byte datestamp array
    //
    datestamp                            = (char *) malloc (sizeof (char) * 16);
    if (datestamp                       == NULL)
    {
        fprintf (stderr, "Error allocating memory for array\n");
        exit (1);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Process for each given command-line argument
    //
    for (index = 1; index < argc; index++)
    {
        ////////////////////////////////////////////////////////////////////////////////
        //
        // If we've not yet started the list, do so here
        //
        if (start                       == NULL)
        {
            start                        = (Node *) malloc (sizeof (Node));
            if (start                   == NULL)
            {
                fprintf (stderr, "Error allocating memory for node\n");
                exit (2);
            }
            start -> next                = NULL;
            tmp                          = start;
        }

        ////////////////////////////////////////////////////////////////////////////////
        //
        // Allocate additional nodes for our list
        //
        else
        {
            tmp -> next                  = (Node *) malloc (sizeof (Node));
            if (tmp -> next             == NULL)
            {
                fprintf (stderr, "Error allocating memory for node\n");
                exit (3);
            }
            tmp                          = tmp -> next;
            tmp -> next                  = NULL;
        }
        tmp -> now                       = (time_t) atoi (*(argv+index));
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // If the list is empty, obtain data via STDIN
    //
    //setvbuf (stdin, NULL, _IONBF, 0);
    if (tmp                             == NULL)
    {
        while (!feof (stdin))
        {
            ////////////////////////////////////////////////////////////////////////////
            //
            // If we've not yet started the list, do so here
            //
            if (start                   == NULL)
            {
                start                    = (Node *) malloc (sizeof (Node));
                if (start               == NULL)
                {
                    fprintf (stderr, "Error allocating memory for node\n");
                    exit (2);
                }
                start -> next            = NULL;
                tmp                      = start;
            }

            ////////////////////////////////////////////////////////////////////////////
            //
            // Allocate additional nodes for our list
            //
            else
            {
                tmp -> next              = (Node *) malloc (sizeof (Node));
                if (tmp -> next         == NULL)
                {
                    fprintf (stderr, "Error allocating memory for node\n");
                    exit (3);
                }
                tmp                      = tmp -> next;
                tmp -> next              = NULL;
            }

            ////////////////////////////////////////////////////////////////////////////
            //
            // Needed? Construct input one byte at a time, utilize datestamp,
            // as it currently is not in use.
            //
            for (index = 0; index < 16; index++)
            {
                fread ((datestamp+index), 1, 1, stdin);
                if ((*(datestamp+index) == ' ')   ||
                    (*(datestamp+index) == '\n'))
                {
                    *(datestamp+index)   = '\0';
                    break;
                }
            }
            tmp -> now                   = (time_t) atoi (datestamp);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // If there are no collected requests to process, do it for the current
    // time
    //
    tmp                                  = start;
    if (tmp                             == NULL)
    {
        mytime                           = localtime (NULL);
        strftime (datestamp, 16, "%Y%m%d-%H%M%S", mytime);
        fprintf  (stdout,        "%s\n",          datestamp);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // Work through our amassed list to display final datestamp values, if
    // there are any to work through
    //
    while (tmp                          != NULL)
    {
        mytime                           = localtime (&(tmp -> now));
        strftime (datestamp, 16, "%Y%m%d-%H%M%S", mytime);
        fprintf  (stdout,        "%s\n",          datestamp);
        tmp                              = tmp -> next;
    }

    return (0);
}
