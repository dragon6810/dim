#ifndef _SHA256_H
#define _SHA256_H

#include <stdint.h>

void sha256_hash(void* data, uint64_t len, uint32_t outhash[8]);
void sha256_setup(void);

#endif