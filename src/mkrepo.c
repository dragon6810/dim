#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "repo.h"

void mkrepo(int argc, char** argv)
{
    FILE *ptr;

    if(argc)
    {
        printf("mkrepo: no arguments expected.\n");
        exit(1);
    }

    if(checkrepo())
    {
        printf("mkrepo: repository already exists in current directory.\n");
        exit(1);
    }

    mkdir(".dim", S_IRWXU | S_IRWXG | S_IRWXO);
    ptr = fopen(".dim/index", "w");
    if(!ptr)
    {
        printf("mkrepo: couldn't create file \"%s\".\n", "./dim/index");
        exit(1);
    }
    fclose(ptr);
}