#include "commands.h"

#include <string.h>

#include "ftree.h"

void commit(int argc, char** argv)
{
    ftree_t tree;

    memset(&tree, 0, sizeof(ftree_t));

    ftree_loadindex(&tree);
}