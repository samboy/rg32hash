# You are not expected to understand this

Well, that’s one way to put it, but not very helpful.

Yes, in order to make nanorg32.c as compact as possible, the code is
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

Each element of the belt is an unsigned 32-bit integer, since this
is an implementation of RadioGatún[32].

Since C, of course, uses 0-indexed arrays, element 0 is the first
element and element 39 is the 40th element.

## The mill

The mill in nanorg32.c is the same as in the RadioGatún spec for
RadioGatún[32] implementations: It is an array of 19 unsigned
32-bit integers.

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

Note that the ~ operator inverts bits; with 32-bit numbers, it is 
equivalent to performing an exclusive or against 0xffffffff.

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

Note that this does most, but not all, of the belt rotation.
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

## main(): Starting the code

```c
int main(int p,char**v)
```

This is, in C, where code execution starts.  “p” is usually called
argc (number of arguments given to the program.  If nanorg32 is called
as just “nanorg32”, argc will be one.  If called as “nanorg32 foo”, argc 
is two, “nanorg32 foo foo” would set argc to three, and so on).  “v”,
usually called argv, is an array of strings (the first star means “array of”
and the second star means “string”, since, in C, a string is an array
of characters).

This is what the above function call would normally look like:

```c
int main(int argc, char **argv)
```

Note that a number of historic IOCCC entries simply call this function as
main() without the “int”, but doing so generates a warning in modern 
compilers, and one of nanorg32.c’s goals is to generate no warnings
when compiled with GCC, Clang/LLVM, and TCC.

## Grabbing the string to hash

Here is how we grab the string to hash:

```c
char *q=v[--p];
```

If expanded, this would look like this:

```c
char *toHash = argv[argc - 1];
```

In other words, we always hash the final argument given to the
program, regardless of the number of arguments given to it.

If this program was called as “nanorg32”, we would hash the string
“nanorg32”; if called as “nanorg32 foo”, we would hash “foo”; and if
called as “nanorg32 foo bar”, we would hash “bar”.

## Initializing the RadioGatún state

The RadioGatún spec requires that we set all words in the belt and mill
to 0 before starting the hash:

```c
b(40)f[c]=e[c%19]=0;
```

This, if exapanded, would look like:

```c
for(counter = 0; counter < 40; counter++) 
	{
	belt[counter] = 0;
	mill[counter % 19] = 0;
	}
```

Actually, it would probably look like:

```c
for(counter = 0; counter < 40; counter++) 
	{
	belt[counter] = 0;
	}
for(counter = 0; counter < 19; counter++)
	{
	mill[counter] = 0;
	}
```

## The input map

### An infinite loop

```c
for(;;m())
```

We run the input map in an infinite loop. m() is here to save a character;
if this were expanded

```c
for(;;) // Some C programmers perfer while(1)
	{
	// A whole bunch of code I am about to explain
	beltMill();
	}
```

### We run the belt and mill function every three words

```c
b(3){
```

The RadioGatún spec has us run the mill function every three words, which
we do here.  Expanded, this would be:

```c
for(counter = 0 ; counter < 3 ; counter++) 
	{
	// Some more code I am about to explain
	}
```

### We convert 8-bit characters in to 32-bit words 

The input mapper converts 8-bit characters in to 32-bit words,
and when the input ends, it adds a final 8-bit binary 1 to the 
RadioGatún string.  This is done thusly here:

```c
for(s=r=0;r<4;){t=*q++;s|=(t?t&255:1)<<8*r++;if(!t){
```

Expanded version:

```c
inputWord = 0;
for(rotateAmount = 0; rotateAmount < 32; rotateAmount += 8) 
	{
	// We convert the current character in the string we are hashing
	// in to a 32-bit word with an 8-bit value
	temp = (uint32_t)*toHash;
	toHash++; // Get ready to look at the next character in the string
	// We have to do the following step so that high-bit (Unicode, etc.)
	// characters remain 8-bit; otherwise 128 (0x80) would become
	// 0xffffff80
	temp &= 255; 
	// The nanorg32 code assumes that the input string is a 
	// null-terminated string; while RadioGatún allows for arbitrary
	// binary strings, our code does not.
	if(temp != 0) 
		{
		temp <<= rotateAmount;
		inputWord |= temp;
		}
	else // The input mapping is done
		{
		inputWord |= 1 << rotateAmount;
		postInputMap();
		}
	}
```

The reason for 32 is, again, because this is an implementation of
RadioGatún[32].

### postInputMap(): Hold on

While we call postInputMap() in the code (but not as a separate function)
before we finish the input mapping for the sake of making the code as
compact as possible, we will explain it after the input map is fully
explained.

Here is the code in question which we will get back to:

```c
d;b(17)m();t=2;b(8){if(t&2)m();r=c;s=e[t^=3];b(4){
printf("%02x",s&255);s>>=8;}c=r;}return puts("");
```

### Make a given word in the input map affect the RadioGatún state

Here is the final bit of code in the input mapper:

```c
}}d;}}}
```

“d” was defined earlier in the code, since we use the exact same code
twice:

```
#define d f[c*13]^=s;e[16+c]^=s;
```

Expanded, the outer loops of the input mapper look like this:

```c
for(;;) // As per the code above
	{
	for(counter = 0 ; counter < 3 ; counter++)
		{
		// A bunch of stuff described above
		// [...]
		belt [ counter * 13 ] ^= inputWord;
		mill [ counter + 16 ] ^= inputWord;
		}
	beltMill();
	}
```

### The input mapper in a nutshell

Now that we have gone over each part of the input mapper, here it is
in a nutshell:

Here is the compact code:

```c
for(;;m()){b(3){for(s=r=0;r<4;){t=*q++;s|=(t?t&255:1)<<
8*r++;if(!t){// We put postInputMap() here!
}}d;}}
```

Which is expanded to:

```c
for(;;) // We terminate when the string ends
	{
	for(counter = 0 ; counter < 3 ; counter++)
		{
		// This is more throughly explained above
		inputWord = 0;
		for(rotateAmount = 0; rotateAmount < 32; rotateAmount += 8) 
			{
			temp = (uint32_t)*toHash;
			toHash++;
			temp &= 255; 
			if(temp != 0) 
				{
				temp <<= rotateAmount;
				inputWord |= temp;
				}
			else // The input mapping is done
				{
				inputWord |= 1 << rotateAmount;
				postInputMap();
				}
			}
		belt [ counter * 13 ] ^= inputWord;
		mill [ counter + 16 ] ^= inputWord;
		}
	beltMill();
	}
```

### And, now, on to postInputMap()

As I promised before, here is an explanation of postInputMap().
As noted above, the code looks like this:

```c
d;b(17)m();t=2;b(8){if(t&2)m();r=c;s=e[t^=3];b(4){
printf("%02x",s&255);s>>=8;}c=r;}return puts("");
```

Here is how it looks expanded. First, it makes sense to make this
a separate function:

```c
void postInputMap()
	{
```

Then, let’s run the input map one final time:

```c
	belt [ counter * 13 ] ^= inputWord;
	belt [ counter * 13 ] ^= inputWord;
```

In actual production code, we would probably not have “counter” be a 
global variable, but have it so here to make the explanation more concise.
In the production code I have written, I tend to have the input mapper
be its own function, and return from the function once the input string
has ended and we run the beltMill() a few more times.  I also tend to
pass beltMill() a copy of the RadioGatún state.

### Preparing the RadioGatún state for output

As per the RadioGatún spec, we need to run the BeltMill a number
of times to “mash up” its state.  We run the belt and mill 17 times.

In the compact code:

```c
b(17)m();
```

Expanded:

```c
	for(counter = 0; counter < 17; counter++) 
		{
		beltMill();
		}
```

### Getting pseudo-random numbers from RadioGatún’s state

Now, we’re getting ready to convert the RadioGatún state in to a hash
to show the user.  While we run RadioGatún’s Belt/Mill once every 
three words (96 bits, in the case of RadioGatún[32]) during the input 
map, we run the Belt/Mill every two words when generating the 
arbitrarily long hash output.

As per the RadioGatún spec, we first output mill[1] then mill[2],
followed by running the Belt/Mill function, followed by looking at
mill[1] and so on until we have all the pseudo-random bits we need
for our application.  In other words, for RadioGatún[32]:

* We get 32 bits of random numbers from mill[1]
* We get another 32 bits of random numbers from mill[2]
* We run the Belt + Mill function once to “mash” the hash
* We go back to the first step in this list to get more pseudo-random bits.
  
There are some tricks in the code to minimize the code size.

Here is how it looks:

```c
t=2;b(8){if(t&2)m();r=c;s=e[t^=3];
```

Expanded literally:

```c
	wordView = 2;
	for(counter = 0; counter < 8; counter++) 
		{
		if(wordView & 2)
			{
			beltMill();
			}
		counterSave = counter; // We’ll get to this later
		outWord = mill [ wordView ^= 3 ];	
		// Some more code to output the hash bits as hex
```

To make it more readable:
	
```c
	wordView = 1;
	for(counter = 0; counter < 8; counter++) 
		{
		if(wordView == 1)
			{
			beltMill();
			}
		outWord = mill [ wordView ];
		wordView++;
		if(wordView == 3)
			{
			wordView = 1;
			}
		counterSave = counter; // We’ll get to this later
		// Some more code to output the hash bits as hex
```

Which acts the same way.

### Converting the outWord in to a hex number

Because of endian issues, we have to swap the words we get from the
RadioGatún state to generate correct test vectors; this is an optional
step and, if test vector compatibility is not needed, can be ignored.

Since nanorg32.c generates correct test vectors, we do it thusly:

```c
r=c;
// We put s=[t^=3]; in the actual code, but that is already
// explained above
b(4){printf("%02x",s&255);s>>=8;}c=r;}return puts("");};
```

Expanded:

```c
		counterSave = counter;
		for(counter = 0; counter < 4; counter++)
			{
			printf("%02x",outWord & 255);
			outWord >>= 8;
			}
		counter = counterSave;
		}
	return puts("");
	}
```

Or, to clean it up:

```c
		for(counterTwo = 0; counterTwo < 4; counterTwo++)
			{
			printf("%02x",outWord & 0xff);
			outWord >>= 8;
			}
		}
	puts(""); // Newline at end of hash
	return 0;
	}
```

## So, there you go

nanorg32.c let’s us see how we can very compactly define RadioGatún[32]
in places where code size is more important than code speed.

I also have a similar version of this code which can be used as an API
in a larger program; that code is in tinyapi.c.

