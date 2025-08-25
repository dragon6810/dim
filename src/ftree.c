#include "ftree.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "index.h"
#include "repo.h"

void ftree_free(ftree_t* tree)
{
    ftree_child_t *child, *next;

    assert(tree);

    child = tree->children;
    while(child)
    {
        next = child->next;
        ftree_free(child->node);
        free(child->node);
        free(child);
        child = next;
    }
}

void ftree_loadpath(ftree_t* tree, const char* path)
{
    char relative[PATH_MAX], slice[PATH_MAX];
    char *start, *end;

    assert(tree);
    assert(path);

    if(!pathinrepo(path))
    {
        printf("can't load file \"%s\" into ftree since it is not in the repo directory.\n", path);
        exit(1);
    }

    pathrelativetorepo(path, relative);

    start = relative;
    while(*start)
    {
        end = start;
        while(*end && *end != '/')
            end++;
        
        memcpy(slice, start, end - start);
        slice[end - start] = 0;

        printf("slice: %s\n", slice);

        if(!*end)
            break;
        start = end + 1;
    }
}

void ftree_loadindex(ftree_t* outftree)
{
    index_entry_t *cur;

    index_t idx;

    assert(outftree);

    memset(&idx, 0, sizeof(index_t));

    index_prune();

    memset(&idx, 0, sizeof(index_t));
    
    index_load(&idx);

    cur = idx.entries;
    while(cur)
    {
        if(access(cur->path, F_OK))
        {
            cur = cur->next;
            continue;
        }

        ftree_loadpath(outftree, cur->path);
        cur = cur->next;
    }

    index_freeentries(&idx);
}