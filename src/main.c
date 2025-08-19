#include <stdio.h>

#include "sha256.h"

int main(int argc, char** argv)
{
    const char msg[] = "this is a message";

    int i;

    uint32_t hashed[8];

    sha256_setup();

    sha256_hash((void*) msg, sizeof(msg), hashed);
    printf("hashed message: 0x");
    for(i=0; i<8; i++)
        printf("%08x", hashed[i]);
    printf("\n");
}