#include "index.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "repo.h"

#define INDEX_PATH ".dim/index"

bool index_hasentry(index_t* idx, const char* entry)
{
    index_entry_t *cur;

    assert(idx);
    assert(entry);

    cur = idx->entries;
    while(cur)
    {
        if(!strcmp(cur->path, entry))
            return true;
        cur = cur->next;
    }

    return false;
}

void index_prune(void)
{
    index_t idx;
    char newpath[PATH_MAX];
    index_entry_t *cur, *last, *next;

    index_load(&idx);

    last = NULL;
    cur = idx.entries;
    while(cur)
    {
        next = cur->next;
        
        if(!access(cur->path, F_OK) && pathinrepo(cur->path))
        {
            last = cur;
            cur = next;

            pathrelativetorepo(cur->path, newpath);
            strcpy(cur->path, newpath);

            continue;
        }

        if(last)
            last->next = next;
        else
            idx.entries = next;

        free(cur);
        
        cur = next;
    }

    index_write(&idx);
    
    index_freeentries(&idx);
}

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
    char terminated[PATH_MAX];

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

        // chop off \n
        memcpy(terminated, line, linelen - 1);
        terminated[linelen] = 0;

        newentry = malloc(sizeof(index_entry_t));
        memset(newentry, 0, sizeof(index_entry_t));
        strcpy(newentry->path, terminated);
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
        fprintf(ptr, "%s\n", entry->path);
        entry = entry->next;
    }

    fclose(ptr);
}