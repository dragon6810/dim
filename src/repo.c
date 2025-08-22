#include "repo.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

bool checkrepo(void)
{
    struct stat st;

    if(stat(".dim", &st) == -1)
        return false;

    return true;
}

bool checkhash(shahash_t hash)
{
    char filename[PATH_MAX];
    struct stat st;

    snprintf(filename, PATH_MAX-1, ".dim/obj/%08x%08x%08x%08x%08x%08x%08x%08x",
        hash[0], hash[1], hash[2], hash[3], hash[4], hash[5], hash[6], hash[7]);

    return stat(filename, &st) != -1;
}

bool pathinrepo(const char* path)
{
    int cwdlen;
    char cwd[PATH_MAX];
    char abspath[PATH_MAX];

    getcwd(cwd, PATH_MAX);
    realpath(path, abspath);

    cwdlen = strlen(cwd);
    
    return !strncmp(cwd, abspath, cwdlen) && (abspath[cwdlen] == '\0' || abspath[cwdlen] == '/');
}

void pathrelativetorepo(const char* path, char *outpath)
{
    assert(strlen(path) < PATH_MAX);
    int cwdlen;
    char cwd[PATH_MAX];
    char abspath[PATH_MAX];

    getcwd(cwd, PATH_MAX);
    realpath(path, abspath);

    cwdlen = strlen(cwd);
    
    if(strncmp(cwd, abspath, cwdlen) || (abspath[cwdlen] != '\0' && abspath[cwdlen] != '/'))
    {
        outpath[0] = 0;
        return;
    }

    strcpy(outpath, abspath + cwdlen);
}