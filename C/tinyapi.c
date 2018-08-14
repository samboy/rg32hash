#include <stdint.h> // Public domain random numbers
#define rg uint32_t // NO WARRANTY
#define rgp(a) for(c=0;c<a;c++)
#define rgn w[c*13]^=s;u[16+c]^=s;
void rgf(rg*a,rg*b){rg m=19,A[45],x,o=13,c,y,r=0;rgp(12)b[c+c%3*o]^=a
[c+1];rgp(m){r=(c+r)&31;y=c*7;x=a[y++%m];x^=a[y%m]|~a[(y+1)%m];A[c]=A
[c+m]=x>>r|x<<(32-r)%32;}for(y=39;y--;b[y+1]=b[y])a[y%m]=A[y]^A[y+1]^
A[y+4];*a^=1;rgp(3)a[c+o]^=b[c*o]=b[c*o+o];}void rgl(rg*u,rg*w,char*v
){rg s,q,c,x;rgp(40)w[c]=u[c%19]=0;for(;;rgf(u,w)){rgp(3){for(s=q=0;q
<4;){x=*v++;s|=(x?255&x:1)<<8*q++;if(!x){rgn;rgp(17)rgf(u,w);return;}
}rgn;}}}rg rgi(rg*m,rg*b,rg*a){if(*a&2)rgf(m,b);return m[*a^=3];}

/* Example of API usage, non-Golfed (also public domain) */
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
        uint32_t belt[40], mill[19], c, j, phase = 2;

        if(argc < 2) {
                printf("Usage: rg32 '{input to hash}'\n");
                exit(1);
        }

        /* Seed random number generator */
        rgl(mill,belt,argv[1]);

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
