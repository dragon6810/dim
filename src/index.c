#include "index.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INDEX_PATH ".dim/index"

void index_freeentries(index_t* idx)
{
    index_entry_t *entry, *next;

    assert(idx);

    entry = idx->entries;
    while(entry)
    {
        next = entry->next;
        free(entry);
        entry = next;
    }
}

void index_load(index_t* out)
{
    FILE *ptr;
    char *line;
    size_t linelen;
    index_entry_t *newentry, *entrytail;

    assert(out);

    index_freeentries(out);

    ptr = fopen(INDEX_PATH, "r");
    if(!ptr)
    {
        printf("can't open index file \"%s\".\n", INDEX_PATH);
        exit(1);
    }

    entrytail = NULL;
    while((line = fgetln(ptr, &linelen)))
    {
        if(linelen >= PATH_MAX)
        {
            printf("path too long in index file \"%s\".\n", line);
            exit(1);
        }

        newentry = malloc(sizeof(index_entry_t));
        memset(newentry, 0, sizeof(index_entry_t));
        strcpy(newentry->path, line);
        free(line);
        if(entrytail)
            entrytail->next = newentry;
        else
            out->entries = newentry;

        entrytail = newentry;
    }

    fclose(ptr);
}

void index_write(index_t* idx)
{
    FILE *ptr;
    index_entry_t *entry;

    assert(idx);

    ptr = fopen(INDEX_PATH, "w");
    if(!ptr)
    {
        printf("can't open index file for writing \"%s\".\n", INDEX_PATH);
        exit(1);
    }

    entry = idx->entries;
    while(entry)
    {
        fputs(entry->path, ptr);
        entry = entry->next;
    }

    fclose(ptr);
}