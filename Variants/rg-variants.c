/* Donated the the public domain 2012-2017 Sam Trenholme.  Written in 2009,
 * 2012, 2015, 2017. 
 *
 * This software is provided 'as is' with no guarantees of correctness or
 * fitness for purpose.
 */

/* This version of RadioGatun[32] allows one to experiment with variants
 * It takes a number of arguments:
 * 1) The input to hash
 * All other arguments are optional:
 * 2) The word size in bits.  Must be a multiple of 8 between 8 and 64.
 *    Defaults to 32.
 * 3) The mill size in words.  Is 19 in RadioGatun, 17 in RV200
 * 4) The operation to perform.  This is a numeric value:
 *    0. or-not in gamma step (standard RadioGatun)
 *    1. subtract in gamma step (RV200)
 *    2. not-add in gamma step (RadioAddin)
 * 5) The number of blank rounds.  To have every bit in the belt or mill
 *    affect every bit in the mill, have this be the mill size minus 3
 *    16 in both RadioGatun and RV200
 * 6) The constant we exclusive or the first byte of the mill with during
 *    the iota step.  This is 1 in RadioGatun
 */

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
uint64_t iota = 1;
/* Operation in Gamma: 
 * 0: or-not
 * 1: subtraction
 * 2: not-addition
 */
int operation = 0; 

/* These are hard coded in the Radio Gatun specification */
#define DWR_BELTROWS 3

/* The "mill" part of Radio Gatun */
void dwr_mill(DWR_WORD *a, DWR_WORD *A) {
	DWR_WORD x;
	int i = 0;
	int y = 0;
	int r = 0;
	int z = 0;
	int q = 0;
	for(i = 0; i < millsize ; i++) {
		y = (i * 7) % millsize;
		r = ((i * (i + 1)) / 2) % wordsize;
		if(operation == 0) {
			x = a[y] ^ (a[ ((y + 1) % millsize) ] | 
		    	    (~a[ ((y + 2) % millsize) ]));
		} else if(operation == 1) {
			x = a[y] ^ (a[ ((y + 1) % millsize) ] -
			    (a[ ((y + 2) % millsize) ]));
		} else if(operation == 2) {
			x = a[y] ^ (a[ ((y + 1) % millsize) ] +
			    (~a[ ((y + 2) % millsize) ]));
		} 
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
	a[0] ^= iota;
}	

/* The "belt" part of Radio Gatun */
void dwr_belt(DWR_WORD *a, DWR_WORD *b, DWR_WORD *A) {
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
	dwr_mill(a,A);
	for(i = 0; i < DWR_BELTROWS; i++) {
		a[(i + beltcol)] ^= q[i];
	}
}

/* Convert a null-terminated string in to a Radio Gatun state (doesn't
 * include padding and what not) */
void dwr_input_map(DWR_WORD *a, DWR_WORD *b, DWR_WORD *A, uint8_t *v) {
	DWR_WORD p[3];
	int q = 0;
	int c = 0;
	int r = 0;
	int done = 0;
	for(;;) {
		p[0] = p[1] = p[2] = 0;
		for(r = 0; r < 3; r++) {
			for(q = 0; q < wordsize / 8; q++) {
				DWR_WORD x = 0;
				x = (DWR_WORD)*v;
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
						b[c * beltcol] ^= p[c];
						a[c + millsize - 3] ^= p[c];
					}
					dwr_belt(a,b,A);
					return;
				}
			}
		}
		for(c = 0; c < 3; c++) {
			b[c * beltcol] ^= p[c];
			a[c + millsize - 3] ^= p[c];
		}
		dwr_belt(a,b,A);
	}
}

/* A structure contining a RadioGatun state */
typedef struct {
        DWR_WORD *a;
        DWR_WORD *b;
        DWR_WORD *A;
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
	out->A = (DWR_WORD *)malloc(millsize * sizeof(DWR_WORD) + 1);
	out->b = (DWR_WORD *)malloc(DWR_BELTROWS * beltcol * 
			sizeof(DWR_WORD) + 1);
	if(out->a == 0 || out->b == 0 || out->A == 0) {
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
	dwr_input_map(out->a,out->b,out->A,v);
	for(c = 0; c < blankrounds; c++) {
		dwr_belt(out->a,out->b,out->A);
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
	if(tozap->A != 0) {
		free(tozap->A);
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
		dwr_belt(state->a,state->b,state->A);
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
	int a, b, bytes_seen = 0;
	DWR_WORD x;
	if(argc < 2) {
		printf("Usage: rg {input to hash} {word size} {mill size}");
		printf("\n\t{operation} {blank rounds} {iota}\n");
		exit(1);
	}
	if(argc >= 3) {
		wordsize = atoi(argv[2]);
		if(wordsize % 8 != 0) {
			printf("Wordsize must be a multiple of 8\n");
			exit(1);
		}
		if(wordsize < 8 || wordsize > 64) {
			printf("Wordsize must be between 8 and 64\n");
		}
	}
	if(argc >= 4) {
		millsize = atoi(argv[3]);
		if(millsize < 11) {
			printf("Millsize must be 11 or higher (sexy prime)\n");
			exit(1);
		}
		blankrounds = millsize - 3;
		if(blankrounds < 16) { blankrounds = 16; }
		if(millsize > 40) { blankrounds++; }
		blankrounds += (millsize / 100);
		beltcol = millsize - 6;
		beltfeed = beltcol - 1;
	}
	if(argc >= 5) {
		operation = atoi(argv[4]);
		if(operation < 0 || operation > 2) {
			printf("Operation must be 0, 1, or 2\n");
			printf("0: or-not in gamma step (standard RG)\n");
			printf("1: subtract in gamma step (RV200)\n");
			printf("2: not-add in gamma step (RadioAddin)\n");
			exit(1);
		}
	}
	if(argc >= 6) {
		blankrounds = atoi(argv[5]);
	}
	if(argc >= 7) {
		iota = atoi(argv[6]);
	}
	wordmask = (1ULL << wordsize) - 1;
	if(!wordmask) { wordmask = 0xffffffffffffffffULL; }
		
	hash = dwr_init_rg(argv[1]);
	if(hash == 0) {
		exit(1);
	}
	for(a=0;a<99999;a++) {
		x = dwr_rng(hash);
		for(b = 0; b < wordsize / 8; b++) {
			printf("%02x",((x >> (b * 8)) & 0xff));
			if(++bytes_seen >= 32) {
				printf("\n");
				return 0;
			}
		}
	}
	printf("\n");
}
	
