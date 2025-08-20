#include "repo.h"

#include <sys/stat.h>

bool checkrepo(void)
{
    struct stat st;

    if(stat(".dim", &st) == -1)
        return false;

    return true;
}