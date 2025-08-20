#include <stdio.h>

#include <string.h>

#include "commands.h"

void usage(void)
{
    printf("usage: dim ");
    printf("[-h | --help]");
    printf("<command> [<args>]\n");

    printf("\n");

    printf("commands:\n");
    printf("    mkrepo\n");
    printf("        creates a dim repository in the current directory.\n");
    printf("    mkblob <file>\n");
    printf("        creates a blob for the given filename, if it exists.\n");
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        usage();
        return 1;
    }

    if     (!strcmp("-h", argv[1]) ||
            !strcmp("--help", argv[1]))
        usage();
    else if(!strcmp("mkrepo", argv[1]))
        mkrepo(argc - 2, argv + 2);
    else
    {
        printf("unrecognized command \"%s\".\n", argv[1]);
        usage();
        return 1;
    }

    return 0;
}