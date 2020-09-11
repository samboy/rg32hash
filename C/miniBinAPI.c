#include <stdint.h> // Public domain random numbers
#include <string.h> // Binary string support
void rg32_f(uint32_t*a,uint32_t*b){
	uint32_t m=19,A[45],x,o=13,c,y,r=0;
	for(c=0;c<12;c++)b[c+c%3*o]^=a[c+1];
	for(c=0;c<m;c++){
		r=(c+r)&31;y=c*7;x=a[y++%m];x^=a[y%m]|~a[(y+1)%m];
		A[c]=(x>>r)|(x<<((32-r)%32));
	}
        for(y=39;y--;b[y+1]=b[y])a[y%m]=A[y]^A[(y+1)%m]^A[(y+4)%m];
	*a^=1;
	for(c=0;c<3;c++)a[c+o]^=b[c*o]=b[c*o+o];}
void rg32_seed(uint32_t*u,uint32_t*w,char*v,uint32_t len){
	uint32_t s,q,c;
	for(c=0;c<40;c++)w[c]=u[c%19]=0;
	for(;;rg32_f(u,w)){
		for(c=0;c<3;c++){
			for(q=0;q<4;){	
				if(len > 0) { w[c*13]^=s=(255&*v++)<<8*q++; }
				else { w[c*13]^=s=1<<8*q++; }
				u[16+c]^=s; 
				if(len == 0){
					for(c=0;c<17;c++)rg32_f(u,w);
					return;}len--;}}}}
uint32_t rg32_rand32(uint32_t*m,uint32_t*b,uint32_t*a){
	if(*a&2)rg32_f(m,b);
	return m[*a^=3];
}

/* Example of API usage, also public domain */
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
        uint32_t belt[40], mill[19], c, j, phase = 2;

        if(argc < 2) {
                printf("Usage: rg32 '{input to hash}'\n");
                exit(1);
        }

        /* Seed random number generator */
        rg32_seed(mill,belt,argv[1],strlen(argv[1]));

        /* Show in hex first 256 bits of PRNG */
        for(c = 0; c < 8; c++) {
                j = rg32_rand32(mill, belt, &phase); /* Get number from PRNG */
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
