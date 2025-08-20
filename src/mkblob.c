#include "commands.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "repo.h"
#include "sha256.h"

void mkobjdir(void)
{
    struct stat st;

    if(stat(".dim/obj", &st) != -1)
        return;

    mkdir(".dim/obj", S_IRWXU | S_IRWXG | S_IRWXO);
}

void writeblob(FILE* text, shahash_t hash)
{
    char blobpath[PATH_MAX];
    FILE *blob;
    void *textdata;
    uint64_t textlen;

    assert(text);
    assert(hash);

    if(checkhash(hash))
        return;

    mkobjdir();

    fseek(text, 0, SEEK_END);
    textlen = ftell(text);
    fseek(text, 0, SEEK_SET);
    textdata = malloc(textlen);
    fread(textdata, 1, textlen, text);
    
    snprintf(blobpath, PATH_MAX-1, ".dim/obj/%08x%08x%08x%08x%08x%08x%08x%08x",
        hash[0], hash[1], hash[2], hash[3], hash[4], hash[5], hash[6], hash[7]);

    blob = fopen(blobpath, "w");
    if(!blob)
    {
        printf("mkblob: could not open file for writing \"%s\".\n", blobpath);
        exit(1);
    }

    fwrite(textdata, 1, textlen, blob);
    fclose(blob);

    free(textdata);
}

void mkblob(int argc, char** argv)
{
    FILE *ptr;

    shahash_t hash;

    if(!checkrepo())
    {
        printf("mkblob: repository does not exist in current directory.\n");
        exit(1);
    }

    if(argc != 1)
    {
        printf("mkblob: expected filename.\n");
        exit(1);
    }

    if(access(argv[0], F_OK))
    {
        printf("mkblob: file does not exist \"%s\".\n", argv[0]);
        exit(1);
    }

    ptr = fopen(argv[0], "r");
    if(!ptr)
    {
        printf("mkblob: could not access file \"%s\".\n", argv[0]);
        exit(1);
    }

    sha256_setup();
    sha256_hashfile(ptr, hash);

    writeblob(ptr, hash);
    printf("%08x%08x%08x%08x%08x%08x%08x%08x\n",
        hash[0], hash[1], hash[2], hash[3], hash[4], hash[5], hash[6], hash[7]);

    fclose(ptr);
}