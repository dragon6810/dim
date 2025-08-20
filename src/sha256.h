#ifndef _SHA256_H
#define _SHA256_H

#include <stdint.h>
#include <stdio.h>

typedef uint32_t shahash_t[8];

// will NOT close ptr. ptr will point to the end of the file after this routine.
void sha256_hashfile(FILE* ptr, shahash_t outhash);
void sha256_hash(void* data, uint64_t len, shahash_t outhash);
void sha256_setup(void);

#endif