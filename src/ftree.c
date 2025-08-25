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

static ftree_child_t* ftree_findchild(ftree_t* node, const char* name, ftree_childtype_e type)
{
    ftree_child_t* child;

    assert(node);
    assert(name);

    child = node->children;
    while(child)
    {
        if(!strcmp(child->name, name))
            return child;
        child = child->next;
    }

    child = malloc(sizeof(ftree_child_t));
    memset(child, 0, sizeof(ftree_child_t));
    strcpy(child->name, name);
    child->type = type;
    child->next = node->children;
    node->children = child;

    if(type == FTREE_CHILD_TREE)
    {
        child->node = malloc(sizeof(ftree_t));
        memset(child->node, 0, sizeof(ftree_t));
        strcpy(child->node->path, name);
    }

    return child;
}

void ftree_loadpath(ftree_t* tree, const char* path)
{
    ftree_child_t *newchild;
    ftree_t *curnode;
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

    curnode = tree;
    start = relative;
    while(*start)
    {
        end = start;
        while(*end && *end != '/')
            end++;
        
        memcpy(slice, start, end - start);
        slice[end - start] = 0;

        if(!*end)
        {
            printf("adding blob %s\n", slice);
            ftree_findchild(curnode, slice, FTREE_CHILD_BLOB);
            return;
        }

        printf("adding tree %s\n", slice);

        newchild = ftree_findchild(curnode, slice, FTREE_CHILD_TREE);
        curnode = newchild->node;

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