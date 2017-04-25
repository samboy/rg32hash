/* Donated the the public domain 2012, 2015 Sam Trenholme.  Written in 2009,
 * 2012, and 2015. 
 *
 * This software is provided 'as is' with no guarantees of correctness or
 * fitness for purpose.
 */

// In this variant, we count the number of rounds before every belt or
// mill bit affects every mill bit in the 64-bit version.  Millsize can
// be set on the command line; make sure the new millsize is the
// second term in a sexy prime pair (11,13,17,19,23,29,37,etc. A046117)
// This is used to determine how many blank rounds our RadioGatún
// variant should have

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* This determines the word size we use for this particular Radio Gatun
 * implementation; DWR_WORDSIZE needs to be a multiple of 8 */
#define DWR_WORD uint64_t
#define DWR_WORDSIZE 64
/* This is all ones in the DWR_WORD */
#define DWR_ONES 0xffffffffffffffffULL

/* These are hard coded in the Radio Gatun specification */
int millsize = 19;
int beltcol = 13;
int beltfeed = 12;
int beltrows = 3;
int millmult = 7; // Make this relatively prime to millsize

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
		y = (i * millmult) % millsize;
		r = ((i * (i + 1)) / 2) % DWR_WORDSIZE;
		x = a[y] | (a[ ((y + 1) % millsize) ] | 
		    (a[ ((y + 2) % millsize) ]));
		//x &= DWR_ONES;
		A[i] = (x >> r) | (x << (DWR_WORDSIZE - r));
		//A[i] &= DWR_ONES;
	}
	for(i = 0; i < millsize ; i++) {
		y = i;
		z = (i + 1) % millsize;
		q = (i + 4) % millsize;
		a[i] = A[y] | A[z] | A[q];
	}
}	

/* The "belt" part of Radio Gatun */
/* a: mill b: belt */
void dwr_belt(DWR_WORD *a, DWR_WORD *b) {
	DWR_WORD q[beltrows];
	int s = 0;
	int i = 0;
	int v = 0;
	for(s = 0; s < beltrows ; s++) {
		q[s] = b[((s * beltcol) + beltcol - 1)];
	}
	for(i = beltcol - 1; i > 0; i--) {
		for(s = 0; s < beltrows ; s++) {
			v = i - 1;
			if(v < 0) {
				v = beltcol - 1;
			}
			b[((s * beltcol) + i)] = 
				b[((s * beltcol) + v)];
		}
	}
	for(s = 0; s < beltrows; s++) {
		b[(s * beltcol)] = q[s];
	}
	for(i = 0; i < beltfeed ; i++) {
		s = (i + 1) + ((i % beltrows) * beltcol);
		b[s % (beltrows * beltcol)] |= a[(i + 1)];
	}
	dwr_mill(a);
	for(i = 0; i < beltrows; i++) {
		a[(i + beltcol)] |= q[i];
	}
}

int is_sat(DWR_WORD *b, DWR_WORD *m) {
	int a;
	// Actually, the RadioGatun designers did not care if all of
	// the Belt bits were affected when deciding on the number of
	// blank rounds.  See section 7.3 of the RG paper.
	/*for(a=0;a<(beltrows * beltcol);a++) {
		if(b[a] != DWR_ONES) {
			return 0;
		}
	}*/
	for(a=0;a<millsize;a++) {
		if(m[a] != DWR_ONES) {
			return 0;
		}
	}
	return 1;
}

void clean(DWR_WORD *b, DWR_WORD *m) {
	int a;
	for(a=0;a<(beltrows * beltcol);a++) {
		b[a] = 0;
	}
	for(a=0;a<millsize;a++) {
		m[a] = 0;
	}
}

int count(DWR_WORD *b, DWR_WORD *m) {
	int a=0;
	while(!is_sat(b,m)) {
		a++;
		dwr_belt(m,b);
		if(a > 100000) {
			return -1;
		}
	}
	return a;
}

void show_sat_rounds() {
	DWR_WORD b[(beltrows * beltcol)];
	DWR_WORD m[millsize];
	int a,c,z;
	for(a=0;a<(beltrows * beltcol);a++) {
		for(c=0;c<DWR_WORDSIZE;c++) {
			clean(b,m);
			b[a] = ((DWR_WORD)1 << c);
			z = count(b,m);
			printf("%d Belt %d %d saturation rounds: %d\n",
					z,a,c,z);
		}
	}
	for(a=0;a<millsize;a++) {
		for(c=0;c<DWR_WORDSIZE;c++) {
			clean(b,m);
			m[a] = ((DWR_WORD)1 << c);
			z = count(b,m);
			printf("%d Mill %d %d saturation rounds %d\n",
					z,a,c,z);
		}
	}
}
		

/* Main function; remove this if using the above as an API in your program */
int main(int argc, char **argv) {
	millsize = 19;
	if(argc == 2 || argc == 3) {
		millsize = atoi(argv[1]);
	}
	if(argc == 3) {
		millmult = atoi(argv[2]);
	}
	if(millsize < 11) {
		millsize = 19;
	}
	beltcol = millsize - 6;
	beltfeed = beltcol - 1;
	beltrows = 3;
	printf("millsize %d millmult: %d\n",millsize,millmult);
	show_sat_rounds();
	printf("millsize %d\n\n",millsize);
}
	
