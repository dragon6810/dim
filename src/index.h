#ifndef _INDEX_H
#define _INDEX_H

#include <stdbool.h>
#include <unistd.h>

/*

index file format:

----------------

<path>
<path>
...

----------------

NOTES:
- strict formatting, parsing done with sscanf.

*/

typedef struct index_entry_s
{
    char path[PATH_MAX];

    struct index_entry_s *next;
} index_entry_t;

typedef struct index_s
{
    index_entry_t *entries;
} index_t;

bool index_hasentry(index_t* idx, const char* entry);

// prunes the index for any deleted files
void index_prune(void);
void index_freeentries(index_t* idx);
void index_load(index_t* out);
void index_write(index_t* idx);

#endif