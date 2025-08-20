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

static inline uint32_t htob32(uint64_t v)
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

    uint32_t *words;
    uint32_t s[2], newh[8], ch, t[2], maj;

    assert(block);
    assert(h);

    words = malloc(sizeof(uint32_t) * 64);
    memcpy(words, block, BLOCKSIZE);
    memset(words + BLOCKWORDS, 0, sizeof(uint32_t) * (64 - BLOCKWORDS));

    for(i=0; i<16; i++)
        words[i] = htob32(words[i]);

    for(; i<64; i++)
    {
        s[0] = sha256_rotateright(words[i-15], 7) 
             ^ sha256_rotateright(words[i-15], 18) 
             ^ (words[i-15] >> 3);
        s[1] = sha256_rotateright(words[i-2], 17)
             ^ sha256_rotateright(words[i-2], 19)
             ^ (words[i-2] >> 10);
        words[i] = words[i - 16] + s[0] + words[i - 7] + s[1];
    }

    memcpy(newh, h, sizeof(newh));
    for(i=0; i<64; i++)
    {
        s[1] = sha256_rotateright(newh[4], 6) ^ sha256_rotateright(newh[4], 11) ^ sha256_rotateright(newh[4], 25);
        ch = (newh[4] & newh[5]) ^ ((~newh[4]) & newh[6]);
        t[0] = newh[7] + s[1] + ch + sha256_k[i] + words[i];
        s[0] = sha256_rotateright(newh[0], 2) ^ sha256_rotateright(newh[0], 13) ^ sha256_rotateright(newh[0], 22);
        maj = (newh[0] & newh[1]) ^ (newh[0] & newh[2]) ^ (newh[1] & newh[2]);
        t[1] = s[0] + maj;

        for(j=7; j>0; j--)
            newh[j] = newh[j - 1];

        newh[4] += t[0];
        newh[0] = t[0] + t[1];
    }

    for(i=0; i<8; i++)
        h[i] += newh[i];

    free(words);
}

void sha256_hash(void* data, uint64_t len, uint32_t outhash[8])
{
    int i;

    uint32_t *paddeddata;
    uint64_t paddedlen, nblocks;
    uint32_t h[N_SQRTS];

    assert(data);
    assert(len);
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
        sha256_processblock(paddeddata + i * BLOCKWORDS, h);

    for(i=0; i<N_SQRTS; i++)
        outhash[i] = htob32(h[i]);

    free(paddeddata);
}

void sha256_setup(void)
{
    sha256_calcroots();
}