#include "sha256.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t squareroots[8] = {};

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
    for(i=3; i<sqrtn; i++)
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
            continue;;
        primes[nprimes++] = i;
        printf("%d is prime.\n", i);
    }
}

static void sha256_squareroots(void)
{
    int i;

    int firstprimes[8];

    sha256_firsprimes(8, firstprimes);

    for(i=0; i<8; i++)
        printf("h%d := 0x%08x\n", i, squareroots[i]);
}

void sha256_setup(void)
{
    sha256_squareroots();
}