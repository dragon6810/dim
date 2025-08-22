#include "ftree.h"

#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "index.h"

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
#if 0
static void ftree_loadfile(FILE* ptr, ftree_t* out)
{
    assert(out);
}
#endif

void ftree_loadindex(void)
{
    index_t idx;

    index_prune();
    index_load(&idx);

    

    index_freeentries(&idx);
}