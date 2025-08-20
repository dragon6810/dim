#include "commands.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

bool checkrepo(void)
{
    struct stat st;

    if(stat(".dim", &st) == -1)
        return false;

    return true;
}

void mkrepo(int argc, char** argv)
{
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

    mkdir(".dim", 0666);
}