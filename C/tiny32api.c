#include <stdint.h> // Public domain random numbers
#define rg uint32_t // NO WARRANTY
#define rgp(a) for(c=0;c<a;c++)
void rgf(rg*a,rg*b){rg m=19,A[45],x,o=13,c,y,r=0;rgp(12)b[c+c%3*o]^=a
[c+1];rgp(m){r=(c+r)&31;y=c*7;x=a[y++%m];x^=a[y%m]|~a[(y+1)%m];A[c]=A
[c+m]=x>>r|x<<(32-r)%32;}for(y=39;y--;b[y+1]=b[y])a[y%m]=A[y]^A[y+1]^
A[y+4];*a^=1;rgp(3)a[c+o]^=b[c*o]=b[c*o+o];}void rgl(rg*u,rg*w,rg v){
rg c;rgp(40)u[c%19]=w[c]=0;*w=u[16]=v;w[13]=u[17]=1;rgp(17)rgf(u,w);}
rg rgi(rg*m,rg*b,rg*a){if(*a&2){rgf(m,b);}return m[*a^=3];}

/* This version of RadioGatún takes a 32-bit unsigned int as its seed.  This
 * way, we can reduce the code size in applications where we simply use RG32
 * as a source of random numbers.  This is a compliant subset of 
 * RadioGatún[32]: the generated numbers are the same as the ones the
 * reference RadioGatún[32] implementation generates when it's given a 
 * 32-bit little endian number as the hash input (e.g. the seed
 * 875770417 generates the same numbers as when RadioGatun[32] hashes
 * the string '1234') */

/* Example of API usage, non-Golfed (also public domain) */
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
        uint32_t belt[40], mill[19], c, j, phase = 2;

        if(argc < 2) {
                printf("Usage: rg32 {seed}\n");
                exit(1);
        }

        /* Seed random number generator */
        rgl(mill,belt,atoi(argv[1]));

        /* Show in hex first 256 bits of PRNG */
        for(c = 0; c < 8; c++) {
                j = rgi(mill, belt, &phase); /* Get number from PRNG */
                /* This isn't needed for good numbers, but test vector
                 * compatibility needs an endian swap on little-endian 
                 * (x86) machines */
                j = (j << 24 |
                     (j & 0xff00) << 8 |
                     (j & 0xff0000) >> 8 |
                     j >> 24);
                printf("%08x",j);
        }
        printf("\n");
        return 0;
}
