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
void truncateArb(arbNum *a, int32_t len) {
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
