# You are not expected to understand this

In order to make nanorg32.c as compact as possible, the code is
a little hard to read.

If this were an IOCCC entry and not a personal side project, I would
just say “if you were a real programmer, you would be able to read 
this code.” But, I think a better expanation is in order.

# Why it is so compact

This code is compact based on my experience writing this code:

http://samiam.org/software/ossuary.c

Here, we have 237 lines of code, of which over 200 are used by the 
random number generator.  I wanted to be able to have a quality
random number generator that would be compact and seen as a “black
box” where a string is given to it, and random numbers come out.

nanorg32 is the latest version of this, which has the RadioGatún[32]
algorithm in only 607 bytes of code (600 if we remove unnecessary 
newlines).

# How nanorg32 and RadioGatún[32] work

The official RadioGatún[32] specification is here:

>http://radiogatun.noekeon.org/

In the interests of making the code as compact as possible,
nanorg32 implements RadioGatún[32] somewhat differently (but, of
course, it returns the exact same test vectors — it _acts_ the 
same). 

I will explain how RadioGatún[32] is implemented in nanorg32.c

## nanorg32.c

Here is the hard to read code:

```c
#include <stdio.h>
#include <stdint.h>
#define b(z) for(c=0;c<z;c++)
#define d f[c*13]^=s;e[16+c]^=s;
uint32_t c,e[19],f[40],g=19,h=13,r,s,t,n[19],i,k;void m(){int c
,j=0;b(12)f[c+c%3*h]^=e[c+1];b(g){j=(c+j)&31;i=c*7%g;k=e[i++];k
^=e[i%g]|~e[(i+1)%g];n[c]=k>>j|k<<(32-j);}for(i=39;i--;f[i+1]=f
[i])e[i%g]=n[i%g]^n[(i+1)%g]^n[(i+4)%g];*e^=1;b(3)e[c+h]^=f[c*h
]=f[c*h+h];}int main(int p,char**v){char *q=v[--p];b(40)f[c]=e[
c%19]=0;for(;;m()){b(3){for(s=r=0;r<4;){t=*q++;s|=(t?t&255:1)<<
8*r++;if(!t){d;b(17)m();t=2;b(8){if(t&2)m();r=c;s=e[t^=3];b(4){
printf("%02x",s&255);s>>=8;}c=r;}return puts("");}}d;}}}
```

I will go over this.

## The BeltMill function

In addition to the required main() function, nanorg32.c also has
a BeltMill function called m():

```c
void m()
```

While the RadioGatún spec has a separate mill function, the mill
is only called precisely once during the RadioGatún belt function,
so it makes more sense to just have a single combined Belt + Mill
function.

## The belt

In the RadioGatún spec, the belt is three arrays of 13 elements each.
In nanorg32.c, the belt is a 40 element one dimensional array.
We use 40 elements to make the belt function rotation use less 
code (see below).

## The mill to belt feedforward

In the RadioGatún spec, the mill to belt feedforward is called after
the simple belt rotation function.

It makes more sense to call it first, because that 
saves us from having to do one unnecessary addition.

The mill to belt feedforward looks like this:

```c
b(12)f[c+c%3*h]^=e[c+1];
```

Here is how it would look if expanded:

```c
for(counter=0 ; counter<12 ; counter++) 
	{
	belt[ counter + ( (counter % 3) * 13) ] ^= mill[ counter + 1 ]
	}
```

b(z) is a shortcut for “for(c=0;c<z;c++)”; we use this macro because
for() loops are common enough that having a compact form of the standard
form saves a lot of code.

The “( (counter % 3) * 13)” lets us change the “row” of the belt being
affected.

13 is the width of each row in the belt.

Note that each element of the belt and mill are unsigned 32-bit integers.
32-bit because this is an implementation of RadioGatún[32].

## The core mill function

The core mill function, which implements most of MILL(A) 
as described in the RadioGatún spec, looks like this in nanorg32.c

```
b(g){j=(c+j)&31;i=c*7%g;k=e[i++];k^=e[i%g]|~e[(i+1)%g];n[c]=k>>j|k<<(32-j);}
```

Expanded, we would get something like this:

```c
for(counter=0; counter < 19; counter++) 
	{
	rotateAmount = (rotateAmount + counter) % 32;
	index = (counter * 7) % 19;
	temp = mill[ index ];
	temp ^=  mill[ ( index + 1 ) % 19 ] |~ mill [ ( index + 2 ) % 19 ];
	millcopy[ counter ] = circularRotateRight(temp,rotateAmount); 
	}
```

With circularRotateRight being something like this:

```c
uint32_t circularRotateRight(in, rotateAmount) 
	{
	return (in >> rotateAmount) | (in << (32 - rotateAmount)); 
	}
```

We have a few magic constants in the above code:

7: This is defined as part of the Pi step in the RadioGatún specification

19: The mill width, as defined in the RadioGatún specification

32: The word size, which is 32 because we are implementing RadioGatún[32]

Note that there are a couple of pieces of the mill we have not done yet;
these will be done later.

## The belt rotation and mill Theta diffusion

In order to make this code as compact as possible, we:

* We separate the mill rotation function in to two different steps
* We perform both one part of the belt rotation and the mill Theta
  diffusion in the same for loop.

Here is the code which does both in a single for() loop:

```c
for(i=39;i--;f[i+1]=f[i])e[i%g]=n[i%g]^n[(i+1)%g]^n[(i+4)%g];
```

Expanded, this would look something like:

```c
for(counter = 38; counter >= 0; counter--) 
	{	
	belt [ counter + 1 ] = belt [ counter ];
	mill [ counter % 19 ] = millcopy [ counter % 19 ];
	mill [ counter % 19 ] ^= millcopy [ ( counter + 1 ) % 19 ];
	mill [ counter % 19 ] ^= millcopy [ ( counter + 4 ) % 19 ]; 
	}
```

Or, perhaps:

```c
for(counter = 38; counter >= 0; counter--) 
	{
	belt [ counter + 1 ] = belt [ counter ]; 
	}
for(counter = 0; counter < 19; counter++) 
	{
	mill [ counter ] = millcopy [ counter ];
	mill [ counter ] ^= millcopy [ ( counter + 1 ) % 19 ];
	mill [ counter ] ^= millcopy [ ( counter + 4 ) % 19 ];
	}
```

Note that this does most, but not all of the belt rotation.
The values for indices 0, 13, and 26 in the belt have the wrong
values; we correct these later.

## The Iota step

One thing I like about RadioGatún, compared to its direct successor
SHA-3, is how the Iota step is very simple.  Here it is in
nanorg32.c:

```c
*e^=1;
```

Expanded, this would be something like

```c
mill [ 0 ] ^= 1;
```

Here, the XOR constant is always 1, which is perfect for an experimental
research cipher where we expect cryptographers to analyze reduced bit 
versions of RadioGatún.

Compare this to how the SHA-3 (a.k.a. Keccak) Iota step XOR constant
is calculated:

```c
/* Keccak iota constant calculator */
uint64_t kIotaNum(int *LFSR) {
        int j, b;
        uint64_t out = 0;
        for(j = 0; j < 7; j++) {
                b = (1 << j) - 1; // The bit to alter
                if(*LFSR & 1) { out |= (1ULL << b); }
                // And now, an 8-bit Galois LFSR in three lines of code
                *LFSR &= 0xff;
                *LFSR <<= 1;
                if(*LFSR & 256) { *LFSR ^= 0x171; }
        }
        return out;
}
```

The reason why the SHA-3 form of Iota is more complicated is because:

* To quote the Keccak 3.0 reference: “As l [the number of non-zero 
  bits in the Iota XOR constant] increases, the round constants 
  add more and more asymmetry.”
* A LFSR is used so hardware implementations need fewer gates to implement
  the Iota step (software implementations are expected to just use a
  table with Iota constants precalculated).

## Belt to mill feedforward

We now both finish up the belt rotation and perform the belt to mill
feedforward in a single step:

```
b(3)e[c+h]^=f[c*h]=f[c*h+h];
```

Expanded, this would look like this:

```
for( counter = 0; counter < 3; counter++ ) 
	{
	// Finish up the belt rotate
	belt [ counter * 13 ] = belt [ ( counter * 13 ) + 13 ];
	// Mill to belt feedforward
	mill [ counter + 13 ] ^= belt [ counter * 13 ];
	}
```

## That ends the beltmill function

And, with that, we have finished performing the belt mill function.
The rest of nanorg32.c performs input mapping (converting a string in
to numbers that alter the belt and mill) as well as output generation
(we generate a 256-bit hash).

I hope to have time in the future to explain how that code works in 
nanorg32.c


