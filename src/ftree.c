#include "ftree.h"

#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>

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

static void ftree_loadfile(FILE* ptr, ftree_t* out)
{
    assert(out);
}

void ftree_loadworking(ftree_t* out)
{
    const char* treepath = ".dim/working-ftree";

    FILE *ptr;

    if(!access(treepath, F_OK))
    {
        ptr = fopen(treepath, "r");
        if(!ptr)
        {
            printf("couldn't open working tree at \"%s\".\n", treepath);
            exit(1);
        }

        ftree_loadfile(ptr, out);

        fclose(ptr);
        return;
    }
}