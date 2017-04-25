/* Donated the the public domain 2012, 2015 Sam Trenholme.  Written in 2009,
 * 2012, and 2015. 
 *
 * This software is provided 'as is' with no guarantees of correctness or
 * fitness for purpose.
 */

/* This is a tiny implementation of the Radio Gatun hash function/
 * stream cipher by Sam Trenholme */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* This determines the word size we use for this particular Radio Gatun
 * implementation; wordsize needs to be a multiple of 8 */
#define DWR_WORD uint64_t
uint64_t wordsize = 32;
uint64_t millsize = 19;
uint64_t beltcol = 13;
uint64_t beltfeed = 12;
uint64_t blankrounds = 16;
uint64_t wordmask = 0xffffffff;

/* These are hard coded in the Radio Gatun specification */
#define DWR_BELTROWS 3

/* The "mill" part of Radio Gatun */
void dwr_mill(DWR_WORD *a) {
	DWR_WORD A[millsize];
	DWR_WORD x;
	int i = 0;
	int y = 0;
	int r = 0;
	int z = 0;
	int q = 0;
	for(i = 0; i < millsize ; i++) {
		y = (i * 7) % millsize;
		r = ((i * (i + 1)) / 2) % wordsize;
		x = a[y] ^ (a[ ((y + 1) % millsize) ] | 
		    (~a[ ((y + 2) % millsize) ]));
		x &= wordmask;
		A[i] = (x >> r) | (x << (wordsize - r));
		A[i] &= wordmask;
	}
	for(i = 0; i < millsize ; i++) {
		y = i;
		z = (i + 1) % millsize;
		q = (i + 4) % millsize;
		a[i] = A[y] ^ A[z] ^ A[q];
	}
	a[0] ^= 1;
}	

/* The "belt" part of Radio Gatun */
void dwr_belt(DWR_WORD *a, DWR_WORD *b) {
	DWR_WORD q[DWR_BELTROWS];
	int s = 0;
	int i = 0;
	int v = 0;
	for(s = 0; s < DWR_BELTROWS ; s++) {
		q[s] = b[((s * beltcol) + beltcol - 1)];
	}
	for(i = beltcol - 1; i > 0; i--) {
		for(s = 0; s < DWR_BELTROWS ; s++) {
			v = i - 1;
			if(v < 0) {
				v = beltcol - 1;
			}
			b[((s * beltcol) + i)] = 
				b[((s * beltcol) + v)];
		}
	}
	for(s = 0; s < DWR_BELTROWS; s++) {
		b[(s * beltcol)] = q[s];
	}
	for(i = 0; i < beltfeed ; i++) {
		s = (i + 1) + ((i % DWR_BELTROWS) * beltcol);
		b[s] ^= a[(i + 1)];
	}
	dwr_mill(a);
	for(i = 0; i < DWR_BELTROWS; i++) {
		a[(i + beltcol)] ^= q[i];
	}
	//int zz,zy;for(zz=0;zz<millsize;zz++){printf("%08x\n",a[zz]);}//DEBUG
}

/* Convert a null-terminated string in to a Radio Gatun state (doesn't
 * include padding and what not) */
void dwr_input_map(DWR_WORD *a, DWR_WORD *b, uint8_t *v) {
	DWR_WORD p[3];
	int q = 0;
	int c = 0;
	int r = 0;
	int done = 0;
	for(;;) {
		p[0] = p[1] = p[2] = 0;
		for(r = 0; r < 3; r++) {
			for(q = 0; q < wordsize / 8; q++) {
				int x = 0;
				x = (int)*v;
				v++;
				x &= 0xff;
				if(x == 0) {
					done = 1;
					x = 1; /* Append with single byte
                                                * w/ value of 1 */
				}
				p[r] |= x << (q * 8);
				if(done == 1) {
					for(c = 0; c < 3; c++) {
						b[c * 13] ^= p[c];
						a[16 + c] ^= p[c];
					}
					dwr_belt(a,b);
					return;
				}
			}
		}
		for(c = 0; c < 3; c++) {
			b[c * 13] ^= p[c];
			a[16 + c] ^= p[c];
		}
		dwr_belt(a,b);
	}
}

/* A structure contining a RadioGatun state */
typedef struct {
        DWR_WORD *a;
        DWR_WORD *b;
	int index;
} dwr_rg;

/* These are the "public methods" for this library */

/* Given a null-terminated string, create a RG state using the string
 * as the input */
dwr_rg *dwr_init_rg(char *v) {
	dwr_rg *out;
	int c = 0;
	
	out = (dwr_rg *)malloc(sizeof(dwr_rg));
	if(out == 0) {
		return 0;
	}
	out->a = (DWR_WORD *)malloc(millsize * sizeof(DWR_WORD) + 1);
	out->b = (DWR_WORD *)malloc(DWR_BELTROWS * beltcol * 
			sizeof(DWR_WORD) + 1);
	if(out->a == 0 || out->b == 0) {
		free(out);
		return 0;
	}

	for(c = 0; c < millsize; c++) {
		out->a[c] = 0;
	}
	for(c = 0; c < DWR_BELTROWS * beltcol; c++) {
		out->b[c] = 0;
	}
	out->index = 0;
	dwr_input_map(out->a,out->b,v);
	for(c = 0; c < blankrounds; c++) {
		dwr_belt(out->a,out->b);
	}
	return out;
}

/* Destroy a created RG state */
void dw_zap_rg(dwr_rg *tozap) {
	if(tozap == 0) {
		return;
	}
	if(tozap->a != 0) {
		free(tozap->a);
	}
	if(tozap->b != 0) {
		free(tozap->b);
	}
	free(tozap);
}

/* Given a RG state, give out a pseudo-random number and update the
 * state */
DWR_WORD dwr_rng(dwr_rg *state) {
	DWR_WORD out;
	if((state->index & 1) == 0) {
		dwr_belt(state->a,state->b);
		out = state->a[1];	
	} else {
		out = state->a[2];
	}
	(state->index)++;
	return out;
}

/* Main function; remove this if using the above as an API in your program */
int main(int argc, char **argv) {
	dwr_rg *hash;
	int a, b;
	int32_t x;
	if(argc < 2) {
		printf("Usage: rg {input to hash} {word size}\n");
		exit(1);
	}
	if(argc >= 3) {
		wordsize = atoi(argv[2]);
		if(wordsize % 8 != 0) {
			printf("Wordsize must be a multiple of 8\n");
			exit(1);
		}
		if(wordsize < 8 || wordsize > 56) {
			printf("Wordsize must be between 8 and 56\n");
		}
	}
	wordmask = (1ULL << wordsize) - 1;
	if(!wordmask) { wordmask = 0xffffffffffffffffULL; }
		
	hash = dwr_init_rg(argv[1]);
	if(hash == 0) {
		exit(1);
	}
	for(a=0;a<8;a++) {
		x = dwr_rng(hash);
		for(b = 0; b < wordsize / 8; b++) {
			printf("%02x",((x >> (b * 8)) & 0xff));
		}
	}
	printf("\n");
}
	
