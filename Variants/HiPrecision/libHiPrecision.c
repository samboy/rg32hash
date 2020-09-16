/* Quick and dirty arbitrary precision integers */
/* Donated to the public domain 2020 by Sam Trenholme */

#include <stdint.h>
#include <stdlib.h>

typedef struct arbNum {
    uint32_t val;
    struct arbNum *next; 
} arbNum;

// Addition
// The numbers are stored as little endian linked lists, where each
// byte is a node in the linked list, and least significant numbers come
// before more significant (bigger) numbers
// Base should be “256” but this code originally added base-10 numbers
// together.
// This creates a new arbNum with the output of the operation
arbNum *add(arbNum *a, arbNum *b, uint32_t base) {
    arbNum *out, *z;
    int carry = 0;
    out = malloc(sizeof(struct arbNum));
    if(out == NULL) {return NULL;}
    z = out;
    while(a != NULL || b != NULL) {
            int v1, v2;
            v1 = v2 = 0;
            if(a != NULL) {v1 = a->val;}
            if(b != NULL) {v2 = b->val;}
            z->val = v1 + v2 + carry;
            carry = 0;
            if(z->val >= base) {carry = 1;}
            z->val = (z->val % base);
            if(a != NULL) {a = a->next;}
            if(b != NULL) {b = b->next;}
            if(a == NULL && b == NULL) {
                if(carry != 0) {
                    z->next = malloc(sizeof(struct arbNum));
                    z = z->next;
                    z->next = NULL;
                    z->val = carry;
                    return out;
                }
                z->next = NULL;
                return out;
            }
            z->next = malloc(sizeof(struct arbNum));
            if(z->next == NULL) {return NULL;}
            z = z->next;
    }
    return out;
}

// Binary XOR
// What base we are in doesn’t matter with exclusive or; since XOR is base
// 2, the base will become a power of 2 by doing this operation
// This creates a new arbNum with the output of the operation
arbNum *xor(arbNum *a, arbNum *b) {
    arbNum *out, *z;
    out = malloc(sizeof(struct arbNum));
    if(out == NULL) {return NULL;}
    z = out;
    while(a != NULL || b != NULL) {
            int v1, v2;
            v1 = v2 = 0;
            if(a != NULL) {v1 = a->val;}
            if(b != NULL) {v2 = b->val;}
            z->val = v1 ^ v2;
            if(a != NULL) {a = a->next;}
            if(b != NULL) {b = b->next;}
            if(a == NULL && b == NULL) {
                z->next = NULL;
                return out;
            }
            z->next = malloc(sizeof(struct arbNum));
            if(z->next == NULL) {return NULL;}
            z = z->next;
    }
    return out;
}

// Binary AND
// This creates a new arbNum with the output of the operation
arbNum *band(arbNum *a, arbNum *b) {
    arbNum *out, *z;
    out = malloc(sizeof(struct arbNum));
    if(out == NULL) {return NULL;}
    z = out;
    while(a != NULL && b != NULL) {
            z->val = a->val & b->val;
            a = a->next;
            b = b->next;
            if(a == NULL || b == NULL) {
                z->next = NULL;
                return out;
            }
            z->next = malloc(sizeof(struct arbNum));
            if(z->next == NULL) {return NULL;}
            z = z->next;
    }
    return out;
}

// Binary OR
// This creates a new arbNum with the output of the operation
arbNum *bor(arbNum *a, arbNum *b) {
    arbNum *out, *z;
    out = malloc(sizeof(struct arbNum));
    if(out == NULL) {return NULL;}
    z = out;
    while(a != NULL || b != NULL) {
            int v1, v2;
            v1 = v2 = 0;
            if(a != NULL) {v1 = a->val;}
            if(b != NULL) {v2 = b->val;}
            z->val = v1 | v2;
            if(a != NULL) {a = a->next;}
            if(b != NULL) {b = b->next;}
            if(a == NULL && b == NULL) {
                z->next = NULL;
                return out;
            }
            z->next = malloc(sizeof(struct arbNum));
            if(z->next == NULL) {return NULL;}
            z = z->next;
    }
    return out;
}

// We need to specify a length when doing a “not” operation
// Note that this will return an error on non-power of 2 bases
// This creates a new arbNum with the output of the operation
arbNum *bnot(arbNum *a, int32_t len, int32_t base) {
    if((base & (base - 1)) != 0) {return NULL;} // Must be power of 2
    arbNum *out, *z;
    int32_t n = 0;
    out = malloc(sizeof(struct arbNum));
    if(out == NULL) {return NULL;}
    z = out;
    while(n < len) {
            int v = 0;
            if(a != NULL) {v = a->val;}
            z->val = (v ^ (base - 1));
	    if(a != NULL) a = a->next;
	    n++;
            if(a == NULL || n >= len) {
                z->next = NULL;
                return out;
            }
            z->next = malloc(sizeof(struct arbNum));
            if(z->next == NULL) {return NULL;}
            z = z->next;
    }
    return out;
}

// Truncate an arbNum after a certain number of “digits”
// Frees memory used by excess digits.  This is a crude “modulo” 
// operation (remember, the further down the linked list we are, the
// bigger the numbers are).  Use this as truncateArb(a,0) to zap
// entire number
// Output: arbNum if still there, NULL if completely zapped
arbNum *truncateArb(arbNum *a, int32_t len) {
    arbNum *out;
    out = a;
    if(a == NULL) { return NULL; }
    while(len > 0 && a != NULL) {
        a = a->next;
        len--;
    }
    while(a != NULL) { 
        arbNum *tmp;
        tmp = a;
        a = a->next;
        free(tmp);
    }
    if(len == 0) {return NULL;}
    return out;
} 

// Expand a number to be N words in size with 0 padding (if needed).  If the
// number is too big, do nothing.  Create the number as needed.  We only need
// to look at the return value if creating a new number.
arbNum *zeroPadArb(arbNum *a, int32_t len) {
    arbNum *z = NULL;
    arbNum *top = NULL;
    top = a;
    while(a != NULL && len > 0) {
        z = a;
        a = a->next;
        len--;
    }
    if(a != NULL) {return top;}
    if(z == NULL && len > 0) {
        z = malloc(sizeof(struct arbNum));
        top = z;
        z->val = 0;
        z->next = NULL;
    }
    while(len > 0) {
	z->next = malloc(sizeof(struct arbNum));
	if(z->next == NULL) {return NULL;}
        z = z->next;
	z->val = 0;
	len--;
    }
    if(z != NULL) {z->next = NULL;}
    return top;
}

// Rotate a word “right” n bits.      
// Instead of specifying a maximum value, we specify the word size for
// each ”digit” in bits.  This is 8 for 0-255 digits (the most common
// value to use)
// We rotate in place, but this can move the pointer to the top of the
// linked list
// We need to update the pointer to the arbNum based on the return value
// of this function, but memory is not allocated (we move its top to rotate
// the bits).
arbNum *rotateRightArb(arbNum *a, uint32_t rotateBits, uint32_t digitBits) {
    arbNum *top, *last;
    uint32_t v = 0;

    uint32_t mask = 1;
    mask = 1 << digitBits;
    mask--;

    // First, make it a circular linked list (easier to rotate)
    top = a;
    if(top == NULL) {return NULL;}
    while(a->next != NULL) {
	a = a->next;
    }
    a->next = top;

    // Now, move the top digit based on the rotation amount
    while(rotateBits > digitBits) {
        top = top->next;
        rotateBits -= digitBits;
    }

    // Now, rotate the bits in each digit
    a = top;
    v = a->val;
    do {
        if(a->next == top) {
	    a->val = (a->val >> rotateBits) | (v << (digitBits - rotateBits));
        } else {
	    a->val = (a->val >> rotateBits) | 
                     (a->next->val << (digitBits - rotateBits));
        }
        a->val &= mask;
        last = a;
	a = a->next;
    } while(a != top);
    last->next = 0;
    return top;
}

// Add a digit to the end of the linked list.  This is used for the
// input mapping.  Create linked list if needed; point to either
// existing or created linked list (we only need to look at return
// value when making new list)
// The digit will be a new higher significance digit.  For example,
// if the value is 0x7f (127) and we add 0xff, the resulting value
// is 7f->ff, or 0xff7f (65407)
arbNum *addDigitToArb(arbNum *a, int32_t digit) {
    arbNum *new;
    arbNum *top;
    top = a;
    new = malloc(sizeof(struct arbNum));
    if(new == NULL){return NULL;} 
    new->val = digit;
    new->next = NULL;
    if(a == NULL){return new;}
    while(a->next != NULL) {a = a->next;}
    a->next = new;
    return top;
}

// How many digits does a given arbNum have?
// return 0 is NULL (no digits)
uint32_t arbLen(arbNum *a) {
    uint32_t out;
    if(a == NULL){return 0;}
    out = 1;
    while(a->next != NULL){
        out++;
        a = a->next;
    }
    return out;
}

// Initialize an array of arbNum elements
// Input:
// elements: Number of arbNum elements
// digits: Number of digits each arbNum element has (set to 0 here; if
//     this is 0, we just make an array of NULL pointers)
arbNum **makeArbNumArray(uint32_t elements, uint32_t digits) {
    arbNum **out = NULL;
    uint32_t a, b;
    out = malloc(elements * sizeof(arbNum *));
    if(out == NULL) {return NULL;}
    for(a = 0; a < elements; a++) {
        out[a] = NULL;
        for(b = 0; b < digits; b++) {
            out[a] = addDigitToArb(out[a], 0);
        }
    }
    return out;
}

// Copy an arbNum 
arbNum *copyArb(arbNum *a) {
    arbNum *out = NULL;
    arbNum *z;
    if(a == NULL) {return NULL;}
    out = malloc(sizeof(struct arbNum));
    z = out;
    while(a != NULL) {
        z->val = a->val;
        if(a->next != NULL) {
            z->next = malloc(sizeof(struct arbNum));
        } else {
            z->next = NULL;
        }
        z = z->next;
        a = a->next;
    }
    return out;
}

// RadioGatún specific stuff
// The following number has to be the second number in a “sexy prime”
// pair, such as 11, 13, 17, 19, 23, 29, 37, 43, 47, 53, etc. (see
// https://oeis.org/A046117 ).  It is 19 for RadioGatún.
#define MILLSIZE 19
// Return the RadioGatun Belt and Mill 
// Input: 
//   Belt (array of 39 RadioGatún elements)
//   Mill (array of 19 RadioGatún elements)
//   len: The number of digits in each elements
//   base: The number of possible value per digit (usually 256) must be
//         a power of 2
void RGbeltMill(arbNum **belt, arbNum **mill, int32_t len, int32_t base) {
    if(belt == NULL || mill == NULL) { return; }
    if(base < 2) { return; }
    int digitBits = 0;
    int a;
    int rotate = 0;
    arbNum **millPrime;
    arbNum *k;
    a = 1;
    while(a < base) {
         digitBits++;
         a <<= 1;
         if(a > 16777216) { break; }
    }
    if(a != base) { return; }
    millPrime = makeArbNumArray(MILLSIZE, 0);
    // Belt to mill feedforward
    for(a = 0; a < MILLSIZE - 7; a++) {
        arbNum *z;
        int n = a + ((a % 3) * (MILLSIZE - 6)); // Belt element to change
        z = xor(belt[n],mill[a + 1]);
        truncateArb(belt[n],0);
        belt[n] = z;
    }
    // Core mill function
    for(a = 0; a < MILLSIZE; a++) {
        int j;
        arbNum *z, *y;
        j = (a * 7) % MILLSIZE;
        rotate += a;
        z = bnot(mill[(j + 2) % MILLSIZE], len, base);
        y = bor(mill[(j + 1) % MILLSIZE],z);
        truncateArb(z,0);
        z = xor(mill[j],y);
        truncateArb(y,0);
        z = rotateRightArb(z, rotate, digitBits);
        millPrime[a] = z;
    } 
    for(a = 0; a < MILLSIZE; a++) {
        arbNum *z, *y;
        z = xor(millPrime[a], millPrime[(a + 1) % MILLSIZE]);
        y = xor(z, millPrime[(a + 4) % MILLSIZE]);
        truncateArb(z,0);
        truncateArb(mill[a],0);
        mill[a] = y;
    }
    for(a = 0; a < MILLSIZE; a++) {
        truncateArb(millPrime[a],0);
    }
    free(millPrime);
    // Belt rotate 
    k = belt[(3 * (MILLSIZE - 6)) - 1];
    for(a = (3 * (MILLSIZE - 6)) - 1; a > 0; a--) {
        belt[a] = belt[a - 1];
    }
    // Final “move down” to make single belt rotate 3-row rotate
    belt[0] = belt[MILLSIZE - 6];
    belt[MILLSIZE - 6] = belt[2 * (MILLSIZE - 6)];
    belt[2 * (MILLSIZE - 6)] = k;
    // Belt to mill
    for(a = 0; a < 3; a++) {
        arbNum *z;
        z = xor(mill[(MILLSIZE - 6) + a],belt[(MILLSIZE - 6) * a]); 
        truncateArb(mill[(MILLSIZE - 6) + a],0);
        mill[(MILLSIZE - 6) + a] = z;
    }
    // Iota; quick and dirty
    mill[0]->val ^= 1;
}

// See how many mill rounds we need before we get full coverage    
// Here, “full coverage” means, that if a single bit is set, and
// we replace all of RadioGatún’s operations with or, how many
// rounds of the mill do we need to run before every bit is set
// In other words, how long before a given bit affects all bits in 
// the mill.
void RGcoverage(arbNum **mill, int32_t len, int32_t digitBits) {
    if(mill == NULL) { return; }
    int a;
    int rotate = 0;
    arbNum **millPrime;
    millPrime = makeArbNumArray(MILLSIZE, 0);
    // Core mill function
    for(a = 0; a < MILLSIZE; a++) {
        int j;
        arbNum *z, *y;
        j = (a * 7) % MILLSIZE;
        rotate += a;
        y = bor(mill[(j + 1) % MILLSIZE],mill[(j + 2) % MILLSIZE]);
        z = bor(mill[j],y);
        truncateArb(y,0);
        z = rotateRightArb(z, rotate, digitBits);
        millPrime[a] = z;
    } 
    for(a = 0; a < MILLSIZE; a++) {
        arbNum *z, *y;
        z = bor(millPrime[a], millPrime[(a + 1) % MILLSIZE]);
        y = bor(z, millPrime[(a + 4) % MILLSIZE]);
        truncateArb(z,0);
        truncateArb(mill[a],0);
        mill[a] = y;
    }
    for(a = 0; a < MILLSIZE; a++) {
        truncateArb(millPrime[a],0);
    }
    free(millPrime);
}

// Is a given array of arbNum “saturated”, i.e. is every value 0xff (allOnes)?
int isSat(arbNum **a, int32_t elements, int32_t allOnes) {
    int z;
    for(z = 0; z < elements; z++) {
        arbNum *x;
        x = a[z];
        while(x != NULL) {
            if(x->val != allOnes) {
                return 0;
            }
            x = x->next;
        }
    }
    return 1;
}

// Get information about the number of rounds for every bit in the
// mill of a given arbitrary word size to influence every other bit
int roundsToMillSat(int32_t words) {
    int b, max, d;
    max = 0;
    for(b = 0; b < MILLSIZE; b++) {
        int c;
        for(c = 0; c < words * 8; c++) {
            arbNum **a;
            int z;
            a = makeArbNumArray(MILLSIZE, words);
            a[b]->val = 1;
            a[b] = rotateRightArb(a[b],c,8);
            d = 0;
            while(isSat(a, MILLSIZE, 0xff) == 0) {
                RGcoverage(a, words, 8);
                d++;
            }
            if(d > max) { max = d; }
            for(z=0;z<MILLSIZE;z++){a[z]=truncateArb(a[z],0);}free(a);//Cleanup
        } 
    }
    return max;
}

// Convert the low 32-bits of a high precision number in to an unsigned
// 32-bit number.  Assumes each “digit” in arbNum is 8 bits in size
uint32_t convertArb32(arbNum *a) {
    uint32_t out;
    int z;
    out = 0;
    for(z = 0; z < 4; z++) {
        if(a == NULL) {break;}
	out |= ((a->val & 0xff) << 8 * z);
	a = a->next;
    }
    return out;
}

// Convert a 32-bit unsigned number in to a 4-digit, 8-bit-per-digit
// arbNum.  Allocates a new arbNum
arbNum *makeArb32(uint32_t a) {
    arbNum *out;
    arbNum *z;
    int c;
    out = malloc(sizeof(struct arbNum));
    if(out == NULL) {return NULL;}
    z = out;
    for(c = 0; c < 4; c++) {
	z->val = (a & 0xff);
        a >>= 8;
        if(c < 3) {
            z->next = malloc(sizeof(struct arbNum));
            if(z->next == NULL) {return NULL;}
            z = z->next;
        } else {
            z->next = NULL;
        }
    }
    return out;
}

// Given 8-bit digits, initialize the RG state based on an input string
// and any word size.  To keep things simple, the input string is a 
// NUL-terminated string, and we’ll just have a global state.
// Digits is the number of 8-bit words this RadioGatún variant uses.
arbNum **gBelt;
arbNum **gMill;
char *inputMapRG(arbNum **belt, arbNum **mill, char *in, int32_t digits) {
    int z;
    for(z = 0; z < 3; z++) {
        int y;
        for(y = 0; y < digits; y++) {
             arbNum *m, *b;
             m = gMill[(MILLSIZE - 3) + z]; 
             b = gBelt[(MILLSIZE - 6) * z];
             int32_t w, x;
             w = *in;
             w &= 0xff;
             in++;
             for(x = 0; x < y; x++) {
		 m = m->next;
                 b = b->next;
             }
             if(w == 0) {
		 m->val ^= 1;
                 b->val ^= 1;
                 return NULL;
             } else {
                 m->val ^= w;
                 b->val ^= w;
             }
        }         
    } 
    RGbeltMill(belt, mill, digits, 256);
    return in;
}

// Given an input string, and the word size (in bytes; e.g. RadioGatún[32]
// has a value of 4), initialize the RadioGatún state
void initRG(char *in, int32_t digits) {
    int z;
    int blankRounds = 18;
    if(digits > 8) { blankRounds++; }
    if(digits < 1) { return; }
    gBelt = makeArbNumArray((MILLSIZE - 6) * 3,digits);
    gMill = makeArbNumArray(MILLSIZE,digits);
    while(in != NULL) { in = inputMapRG(gBelt, gMill, in, digits); }
    for(z = 0; z < blankRounds; z++) {
        RGbeltMill(gBelt, gMill, digits, 256);
    }
}

// Free the memory used by an initialized RG32 state
void cleanRG() {
    int z;
    for(z = 0; z < MILLSIZE; z++) {
	gMill[z] = truncateArb(gMill[z],0);
    }
    for(z = 0; z < (MILLSIZE - 6) * 3; z++) {
	gBelt[z] = truncateArb(gBelt[z],0);
    }
    free(gMill);
    free(gBelt);
}

#ifdef TEST
#include <stdio.h>

// Initialize a RG32 state with a 32-bit fixed-length seed
// This is in the TEST code because this has too many “business
// logic” assumptions.
void initRG32(uint32_t seed) {
    int z;
    gBelt = makeArbNumArray((MILLSIZE - 6) * 3,4);
    gMill = makeArbNumArray(MILLSIZE,4);
    truncateArb(gBelt[0],0);
    gBelt[0] = makeArb32(seed);
    gBelt[MILLSIZE - 6]->val ^= 1;
    truncateArb(gMill[MILLSIZE - 3],0);
    gMill[MILLSIZE - 3] = makeArb32(seed);
    gMill[MILLSIZE - 2]->val ^= 1;
    for(z = 0; z < 18; z++) { 
        RGbeltMill(gBelt, gMill, 4, 256);
    }
}

// Print the contents of an arbNum
// Mode:
// 1: 1->2->3->4 followed by new line
// 2: 1->2->3->4 followed by space
// 3: 01020304 followed by nothing
void printArbNum(arbNum *a, char *fmt, int mode) {
    if(fmt == NULL){fmt = "%x";}
    while(a != NULL) {
        printf(fmt, a->val);
        if(a->next != NULL && mode != 3) {
            printf("->");
        }
        a = a->next;
    }
    if(mode == 1) {puts("");}
    if(mode == 2) {printf(" ");}
}

// Print the full RG belt and mill
void printRG() {
    int a;
    int n = MILLSIZE - 6;
    for(a = 0; a < n; a++) {
        printArbNum(gMill[a],"%02x",2);
        printArbNum(gBelt[a],"%02x",2);
        printArbNum(gBelt[a + n],"%02x",2);
        printArbNum(gBelt[a + n + n],"%02x",1);
    }
    for(;a < MILLSIZE; a++) {
        printArbNum(gMill[a],"%02x",1);
    }
}

// Print n * 2 words based on the rg32 state
void printRGnum(arbNum **mill, arbNum **belt, int n, int words, int base) {
    while(n > 0) {
        printArbNum(mill[1],"%02x",3);
        printArbNum(mill[2],"%02x",3);
        RGbeltMill(belt, mill, words, base);
        n--;
    }
    puts("");
}

int main(int argc, char **argv) {
    if(argc == 2) {
        initRG(argv[1],4);
        printRGnum(gMill, gBelt, 4, 4, 256);
        cleanRG();
        return 0;
    }
    if(argc == 3) {
        int w = atoi(argv[2]); // Number of 8-bit bytes in word length
        int x = 16 / w;
        if(x < 1) { x = 1; }
        initRG(argv[1],w);
        printRGnum(gMill, gBelt, x, w, 256);
        cleanRG();
        return 0;
    }
    arbNum *a, *b, *c;
    int z;
    puts("Test #1: makeArb32.  Should be 04->03->02->01");
    a = makeArb32(0x01020304);
    printArbNum(a,"%02x",1);
    puts("Should be 40->30->20->10");
    b = makeArb32(0x10203040);
    printArbNum(b,"%02x",1);
    puts("Test #2: XOR.  Should be 44->33->22->11");
    c = xor(a,b);
    printArbNum(c,"%02x",1);
    c = truncateArb(c,0);
    puts("Test #3: OR.  Should be 44->33->22->11");
    c = bor(a,b);
    printArbNum(c,"%02x",1);
    a = truncateArb(a,0);
    b = truncateArb(b,0);
    c = truncateArb(c,0);
    puts("Test #4: Rotate 32 times.");
    a = makeArb32(0x80000000);
    for(z = 0; z <= 32; z++) {
	printArbNum(a,"%02x",1);
        a = rotateRightArb(a,1,8);
    }
    puts("Test #5: Rotate test #2");
    for(z = 0; z <= 32; z++) {
	a = truncateArb(a,0);
        a = makeArb32(0x80000000);
        a = rotateRightArb(a,z,8);
        printArbNum(a,"%02x",1);
    }
    puts("Test #6: not test");
    for(z = 0; z <= 32; z++) {
	a = truncateArb(a,0);
        b = truncateArb(b,0);
        a = makeArb32(0x80000000);
        a = rotateRightArb(a,z,8);
	b = bnot(a,4,256);
        printArbNum(b,"%02x",1);
    }
    a = truncateArb(a,0);
    b = truncateArb(b,0);
    puts("Test #7: addDigitToArb and arbLen test");
    for(z = 0; z <= 32; z++) {
        a = addDigitToArb(a, z);
        printArbNum(a,"%02x",1);
        printf("%d\n",arbLen(a));
    }
    a = truncateArb(a,0);
    puts("Test #8: copyArb test");
    a = makeArb32(0x12345678);
    b = copyArb(a);
    printArbNum(a,"%02x",1);
    printArbNum(b,"%02x",1);
    a = truncateArb(a,0);
    b = truncateArb(b,0);
    puts("Test #9: RG32 test");
    initRG32(0x34333231); // "1234"
    printRGnum(gMill, gBelt, 4, 4, 256);
    cleanRG(); 
    initRG("1234",4);
    printRGnum(gMill, gBelt, 4, 4, 256);
    cleanRG();
    for(z = 1 ; z <= 18; z++) {
	printf("Words: %d Sat: %d\n",z,roundsToMillSat(z));
    }
    for(z = 20; z < 32; z+=4) {
	printf("Words: %d Sat: %d\n",z,roundsToMillSat(z));
    }
    for(z = 32; z <= 64; z+= 16) {
	printf("Words: %d Sat: %d\n",z,roundsToMillSat(z));
    }
    return 0;
}
#endif // TEST
