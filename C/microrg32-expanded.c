// cc -o microrg32 microrg32.c ; WORK=3 ; LEN=4 #######
// SECRET="Something random like qhohxks5mx9elv6ujgx3"
// export P="$LEN:$SECRET:x.org" ## Public domain code
// ./microrg32 $WORK $LEN | head -1 | tail
#include <stdio.h> 
#include <stdint.h> 
#include <stdlib.h> 

uint32_t c,mill[42],belt[42],g=19,h=13,r,s,n[45],i,k;
char*q,y;

// This is the cryptographic core of RadioGatun[32], a combination of
// its belt and mill.  The reference specification for this algorithm is
// at http://radiogatun.noekeon.org/RadioGatun.pdf
void beltMill() {
	int c, j = 0;

	// Mill to belt feedforward
	for(c = 0; c < 12; c++) {
		belt[c+c%3*h] ^= mill[c+1];
	}

	for(c=0;c<19;c++) {
		// Non-linearity
		i = c * 7 % g;
		k = mill[i++];
		k ^= mill[i % g] | ~mill[(i + 1) % g];

		// dispersion
		j = (j + c) % 32;
		n[c] = k >> j | k << (32 - j) % 32;
		n[c + g] = n[c];
	}

	for(c=39;c--;belt[c+1]=belt[c]) { // Belt rotation part one
		mill[c] = n[c]^n[c+1]^n[c+4]; // diffusion
	}

	mill[0] ^= 1; // asymmetry

	for(c=0;c<3;c++) { 
		belt[c*h] = belt[c*h+h]; // Belt rotation part two
		mill[c+h] ^= belt[c*h]; // Belt to mill feedforward
	}
}

void generateOutput(char **v) {
	int c,r;

	// Depending on the work amount they specify, make it harder to
	// generate a given password from the master password
	for(c=0;c<17+(1<<*v[1]%32);c++) {
		beltMill();
	}

	// Output a bunch of passwords on standard output
	for(c=0;c<983;c++) {
		s = mill[1+c%2];
		if(c%2) {
			beltMill();
		}
		for(r=0;r<4;r++) {
			y = y>0?y:*v[2]%16;
			i = s;
			s >>= 8;

			// Convert binary in to base32 ASCII
			if(y!=0) {
				i &= 31;
				i += i<8?50:89;
			}

		printf(y?"%c":"%02x",i&255);
		}
		if(--y<1||c>981) {
			puts("");
		}
	}
}

int main(int p,char **v){
	q=getenv("P");
	if(q && p>2) {
		for(;;beltMill()) {
			for(c=0;c<3;c++) {
				for(r=0;r<4;r++) {
					k = (*q?*q&255:1)<<8*r;
					belt[c*h] ^= k;
					mill[16+c] ^= k;
					if(!*q) {
						generateOutput(v);
						return 0;
					}
					q++;
				}
			}
		}
	}
}
