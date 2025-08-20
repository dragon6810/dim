#include "sha256.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_SQRTS    8
#define N_CUBERTS  64
#define BLOCKSIZE  64
#define BLOCKWORDS (BLOCKSIZE / 4)

static uint32_t sha256_h[N_SQRTS] = {};
static uint32_t sha256_k[N_CUBERTS] = {};

static bool sha256_isprime(int n)
{
    int i;

    int sqrtn;

    if(n < 0)
        n = -n;

    if(n <= 1)
        return false;
    if(n < 4)
        return true;
    if(~n & 1)
        return false;

    sqrtn = sqrt(n);
    for(i=3; i<=sqrtn; i+=2)
        if(!(n % i))
            return false;

    return true;
}

static void sha256_firsprimes(int n, int* primes)
{
    int i;

    int nprimes;

    assert(primes);

    for(i=nprimes=0; nprimes<n; i++)
    {
        if(!sha256_isprime(i))
            continue;
        primes[nprimes++] = i;
    }
}

static void sha256_calcroots(void)
{
    int i;

    int firstprimes[N_CUBERTS];
    double root;
    double frac;

    sha256_firsprimes(N_SQRTS, firstprimes);
    for(i=0; i<N_SQRTS; i++)
    {
        root = sqrt(firstprimes[i]);
        frac = root - (int) root;
        sha256_h[i] = frac * 4294967296.0;
    }

    sha256_firsprimes(N_CUBERTS, firstprimes);
    for(i=0; i<N_CUBERTS; i++)
    {
        root = cbrt(firstprimes[i]);
        frac = root - (int) root;
        sha256_k[i] = frac * 4294967296.0;
    }
}

// https://stackoverflow.com/questions/28303232/rotate-right-using-bit-operation-in-c
static inline uint32_t sha256_rotateright(uint32_t x, int n)
{
    n &= 31;
    return (x >> n) | (x << (32 - n));
}

static inline uint32_t swap32(uint32_t v)
{
    return ((v & 0x000000FFu) << 24) |
           ((v & 0x0000FF00u) <<  8) |
           ((v & 0x00FF0000u) >>  8) |
           ((v & 0xFF000000u) >> 24);
}

static inline uint32_t btoh32(uint32_t v)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return swap32(v);
#else
    return v;
#endif
}

static inline uint64_t htob64(uint64_t v)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return ((uint64_t) swap32((uint32_t) (v >> 32))) |
           ((uint64_t) swap32((uint32_t) v) << 32);
#else
    return v;
#endif
}

static void sha256_processblock(void* block, uint32_t h[N_SQRTS])
{
    int i, j;

    uint32_t words[64];
    uint32_t s[2], addh[N_SQRTS], ch, t[2], maj;

    assert(block);
    assert(h);

    memcpy(words, block, BLOCKSIZE);
    memset(words + BLOCKWORDS, 0, sizeof(uint32_t) * (64 - BLOCKWORDS));

    for(i=0; i<64; i++)
        words[i] = btoh32(words[i]);

    for(i=BLOCKWORDS; i<64; i++)
    {
        s[0] = sha256_rotateright(words[i-15], 7) 
             ^ sha256_rotateright(words[i-15], 18) 
             ^ (words[i-15] >> 3);
        s[1] = sha256_rotateright(words[i-2], 17)
             ^ sha256_rotateright(words[i-2], 19)
             ^ (words[i-2] >> 10);
        words[i] = words[i - 16] + s[0] + words[i - 7] + s[1];
    }

    memcpy(addh, h, sizeof(addh));
    for(i=0; i<64; i++)
    {
        s[1] = sha256_rotateright(addh[4], 6) ^ sha256_rotateright(addh[4], 11) ^ sha256_rotateright(addh[4], 25);
        ch = (addh[4] & addh[5]) ^ (~addh[4] & addh[6]);
        t[0] = addh[7] + s[1] + ch + sha256_k[i] + words[i];
        s[0] = sha256_rotateright(addh[0], 2) ^ sha256_rotateright(addh[0], 13) ^ sha256_rotateright(addh[0], 22);
        maj = (addh[0] & addh[1]) ^ (addh[0] & addh[2]) ^ (addh[1] & addh[2]);
        t[1] = s[0] + maj;

        for(j=N_SQRTS-1; j>0; j--)
            addh[j] = addh[j - 1];

        addh[4] += t[0];
        addh[0] = t[0] + t[1];
    }

    for(i=0; i<N_SQRTS; i++)
        h[i] += addh[i];
}

void sha256_hash(void* data, uint64_t len, uint32_t outhash[8])
{
    int i;

    void *paddeddata;
    uint64_t paddedlen, nblocks;
    uint32_t h[N_SQRTS];

    assert(data);
    assert(outhash);

    nblocks = (len + BLOCKSIZE - 1 + 9) / BLOCKSIZE;
    paddedlen = nblocks * BLOCKSIZE;
    paddeddata = malloc(paddedlen);

    memcpy(paddeddata, data, len);
    memset((char*) paddeddata + len, 0, paddedlen - len);
    ((char*)paddeddata)[len] = 0x80;
    ((uint64_t*)paddeddata)[paddedlen / 8 - 1] = htob64(len * 8);

    memcpy(h, sha256_h, sizeof(h));
    for(i=0; i<nblocks; i++)
        sha256_processblock((char*) paddeddata + i * BLOCKSIZE, h);

    for(i=0; i<N_SQRTS; i++)
        outhash[i] = h[i];

    free(paddeddata);
}

void sha256_setup(void)
{
    sha256_calcroots();
}