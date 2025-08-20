#include <stdio.h>

#include <string.h>

#include "sha256.h"

int main(int argc, char** argv)
{
    int i;

    FILE *ptr;

    shahash_t hash;

    sha256_setup();

    ptr = fopen("file.c", "r");
    if(!ptr)
        return 1;
    sha256_hashfile(ptr, hash);
    fclose(ptr);

    printf("0x");
    for(i=0; i<8; i++)
        printf("%08x", hash[i]);
    printf("\n");

    return 0;
}