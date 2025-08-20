#ifndef _FTREE_H
#define _FTREE_H

#include <stdio.h>
#include <unistd.h>

/*

ftree file format:

----------------

ftree <path/from/repo/dir>
child <blob/tree> <name> <hash>
child <blob/tree> <name> <hash>
...

----------------

NOTES:
- strict formatting, parsing done with sscanf.

*/

typedef enum
{
    FTREE_CHILD_BLOB=0,
    FTREE_CHILD_TREE,
} ftree_childtype_e;

typedef struct ftree_child_s
{
    ftree_childtype_e type;
    char name[FILENAME_MAX];
    struct ftree_s *node;
    struct ftree_child_s *next;
} ftree_child_t;

typedef struct ftree_s
{
    char path[PATH_MAX];
    ftree_child_t *children;
} ftree_t;

// will NOT free node you give it, only recursive children
void ftree_free(ftree_t* tree);
void ftree_loadworking(ftree_t* out);

#endif