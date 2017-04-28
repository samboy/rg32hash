#include <stdint.h> // Public domain crypto-strong random numbers
#define rgp uint32_t // NO WARRANTY
#define rgf(a) for(c=0;c<a;c++)
#define rgn w[c*13]^=s;u[16+c]^=s;
void rg(rgp*a,rgp*b){rgp m=19,A[19],x,o=13,c,y,r=0,q[3];rgf(12)b[c+
c%3*o]^=a[c+1];rgf(m){r=(c+r)&31;y=c*7;x=a[y++%m];x^=a[y%m]|~a[(y+1
)%m];A[c]=x>>r|x<<(32-r);}rgf(3)q[c]=b[y=12+c*o];for(;y--;b[y+1]=b[
y])a[y%m]=A[y%m]^A[(y+1)%m]^A[(y+4)%m];*a^=1;rgf(3)a[c+o]^=b[c*o]=q
[c];}void rgl(rgp*u,rgp*w,char*v){rgp s,q,c,x;rgf(39)w[c]=u[c%19]=0
;for(;;rg(u,w)){rgf(3){for(s=q=0;q<4;){x=*v++;s|=(x?255&x:1)<<8*q++
;if(!x){rgn;rgf(17)rg(u,w);return;}}rgn;}}}rgp rgi(rgp*m,rgp*b,rgp*
a){if(*a&2)rg(m,b);return m[*a^=3];}

/* Example of API usage, non-Golfed (also public domain) */
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
        uint32_t belt[39], mill[19], c, j, phase = 2;

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
