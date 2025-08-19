#include "sha256.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define N_SQRTS   8
#define N_CUBERTS 64

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
    for(i=3; i<=sqrtn; i++)
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
        sha256_h[i] = frac * 0xFFFFFFFFu;
    }

    sha256_firsprimes(N_CUBERTS, firstprimes);
    for(i=0; i<N_CUBERTS; i++)
    {
        root = cbrt(firstprimes[i]);
        frac = root - (int) root;
        sha256_k[i] = frac * 0xFFFFFFFFu;
    }
}

void sha256_setup(void)
{
    sha256_calcroots();
}