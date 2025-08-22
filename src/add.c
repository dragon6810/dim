#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "index.h"

void add(int argc, char** argv)
{
    int i;

    index_t index;
    index_entry_t *newentry;

    if(!argc)
    {
        printf("add: expected at least 1 file.\n");
        exit(1);
    }

    memset(&index, 0, sizeof(index));
    index_load(&index);

    for(i=0; i<argc; i++)
    {
        if(strlen(argv[i]) >= PATH_MAX)
        {
            printf("add: path exceeds max length \"%s\".\n", argv[i]);
            exit(1);
        }

        if(index_hasentry(&index, argv[i]))
        {
            printf("add: index already contains \"%s\". ignoring.\n", argv[i]);
            continue;
        }

        newentry = malloc(sizeof(index_entry_t));
        memset(newentry, 0, sizeof(index_entry_t));
        strcpy(newentry->path, argv[i]);
        newentry->next = index.entries;
        index.entries = newentry;
    }

    index_write(&index);
    index_freeentries(&index);
}