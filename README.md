# About

Radio Gatun (RG) is the direct predecessor of SHA-3 (Keccak) that the
same team of world-renowned cryptographers developed. It is a secure
pseudo-random number generator and probably a secure hash function for
generating 256-bit hashes. There are two principal variants of RadioGatun:
A 32-bit version (256-bit to 512-bit hashes) and a 64-bit version (may
be able to make up to 1024-bit hashes). All implementations here are of
the 32-bit version.

More information about the algorithm is here:

http://radiogatun.noekeon.org/

This repo has 14 implementations of RadioGatún[32]:

* Nine C-language implementations
* A C++ implementation
* A PHP implementation
* A Python2 and a Python3 implementation
* A Node-compatible Javascript implementation
* A C# (.NET) implementation

# Other RadioGatun[32] implementations

Before going in to these implementations, let me link to some other
RadioGatun[32] implementations out there:

* [A Pascal implementation](https://github.com/Xor-el/HashLib4Pascal/blob/master/HashLib/src/Crypto/HlpRadioGatun32.pas)
* [A very fast C implementation](https://github.com/samboy/sphlib/blob/master/c/radiogatun.c)
* [A Java implementation](https://github.com/samboy/sphlib/blob/master/java/fr/cryptohash/RadioGatun32.java)
* [Another Python implementation](https://github.com/doegox/python-cryptoplus/blob/master/src/CryptoPlus/Hash/pyradiogatun.py)

# Compile

A Makefile is included to compile the C programs here. To use:

```
make
```

The C programs compiler are in the “C” directory; the compiled programs
will be placed in the “bin/” directory.

# Using rg32hash

This program will be compiled in to the “bin/” directory.

To use this program:

```
rg32hash . > RG32SUMS
```

This will recursively perform a RadioGatun hash on all files and folders
in the current working directory, putting the result in a file named 
"RG32SUMS"

# Using rg32-bin

This program will be compiled in to the “bin/” directory.

rg32-bin.c will compile in to a program which outputs an arbitrary long
binary sequence of pseudo-random numbers.

# rg32-floats

This program will be compiled in to the “bin/” directory.

rg32-floats.c will compile in to a program which outputs an arbitrary long
sequence of pseudo-random floating point numbers between 0 and 1 (it could
be 0 but never 1).  Note that each number has 32 bits of entropy.

I used this program to verify that RadioGatún[32] passes the “Small Crush”
version of TestU01.

# tinyrg32: Somewhat less bloat

This program will be compiled in to the “bin/” directory.

For people that feel rg32-bin.c and rg32hash.c are too bloated, I have
also included tinyrg32.c, which is a somewhat unbloated program which will
output either the 256-bit hex hash of a string given to the program as
an argument, an arbitrary long binary string, an arbitrarily long list
of 32-bit random hex numbers, or even an arbitrarily long continuous
stream of 32-character long strings of random numbers and letters.

Its source code is as follows:

```c
#include <stdio.h> // cc -o tinyrg32 tinyrg32.c ; ./tinyrg32 --binary '12'
#include <stdint.h> // ./tinyrg32 --hex --numbers 'Hello' // Public domain
#define b(z) for(c=0;c<(z);c++) // ./tinyrg32 --passwords _Q 3 'Xz' | head
uint32_t c,e[40],f[40],g=19,h=13,r,s,t,n[45],i,k,y,z;void m(){int c,j=0;b(
12)f[c+c%3*h]^=e[c+1];b(g){j=(c+j)&31;i=c*7%g;k=e[i++];k^=e[i%g]|~e[(i+1)%
g];n[c]=n[c+g]=k>>j|k<<(32-j)%32;}for(c=39;c--;f[c+1]=f[c])e[c]=n[c]^n[c+1
]^n[c+4];*e^=1;b(3)e[c+h]^=f[c*h]=f[c*h+h];}int main(int p,char**v){char*q
=v[--p],*x=0;for(;;m()){b(3){for(r=0;r<4;){f[c*h]^=k=(*q?*q&255:1)<<r++*8;
e[16+c]^=k;if(!*q++){b(17)m();b(p<3?8:89*p){if(~t&1)m();s=e[(t&1)+1];r=(p&
3)-2?c:1;b(4){i=s;if(p&4){x=v[p-2];y=z=z?z:*v[p-1]%16;i&=31;i+=i<8?50:89;}
s>>=8;printf(p==2||p&4?"%c":"%02x",255&i);}if(((++t%8==0||(p&22)==2)&&!y&&
p-2)||r+2>89*p){puts("");}c=r;if(y&&!--z)puts(*x==95?x:"");}return 0;}}}}}
```

Some documentation for tinyrg32.c is included in its source code.  
Full documentation is in the file tinyrg32.md.  To summarize:

The final argument given to tinyrg32.c is always the key used to 
choose the random numbers generated; the same key will always
generate the same numbers.

The current behavior is this:

```
tinyrg32: A 256-bit hash of argv[0], across eight lines of output
tinyrg32 '¡Es una niña linda!': 256-bit hash
tinyrg32 --binary-stream '¡La niña!': Binary stream
tinyrg32 --hex --numbers '¡Niña!': Many 32-bit hex numbers
tinyrg32 --lotsa _Fo0 3 'ñ': Many 60-bit alphanumeric strings 
tinyrg32 --many _Fo0 4 'ñ': Many 80-bit alphanumeric strings 
tinyrg32 --a --lot --of --256 --bit --hex --nums 'ñ': Many 256-bit hex strings
tinyrg32 --infinite --number --strings _Fo0 3 'ñ': Infinite 60-bit strings
tinyrg32 --infinite --number --strings _Fo0 4 'ñ': Infinite 80-bit strings
tinyrg32 --infinite --number --strings _Fo0 5 'ñ': Infinite 100-bit strings
tinyrg32 --infinite --number --strings _Fo0 9 'ñ': Infinite 180-bit strings
tinyrg32 - - - - - - - - - -: Infinite 32-bit hex strings
tinyrg32 - - - - - - - - - - - - - - - - - -: Infinite 256-bit hex strings
```

I understand that tinyrg32.c still has some bloat in it; which is
why I have made...

# nanorg32 RadioGatún[32] in 557 bytes

This program will be compiled in to the “bin/” directory.

nanorg32.c is a C-language RadioGatún[32] implementation in 551 bytes, 
using lines under 75 characters long.  There are no warnings when
compiling with `-Wall -Wpedantic` in gcc 7.3.0, with `-Wall -Wpedantic`
in clang 5.0.1, nor are there any warnings or errors when compiled 
with TCC 0.9.25.

nanorg32.c is public domain.

Since the code may be hard to read, I explain what some of the code does
in the document nanorg32.md (in the “C” directory).

nanorg32-vertical.c is the code for nanorg32.c reformatted to fit better
on a smartphone screen or a business card.

# pwgen.sh Generate passwords with tinyrg32

This script can be found in the “C/” directory.

pwgen.sh allows the one to generate secure passwords for websites. 
To use, do something like this:

```
echo 'Some secret string' > ~/.mash_prefix2 # Only do this once
pwgen.sh facebook.com
```

Replace “Some secret string” with a suitably hard to guess string, and 
facebook.com with the site you need to log in to.

pwgen.sh is smart enough to compile tinyrg32 should tinyrg32 not be
in one's path.

# tinyapi.c Tiny rg32 library

This program will be compiled in to the “bin/” directory.

tinyapi.c has both a tiny rg32 library for C program and an example of 
the library being used.  Useful when you want to give a C program a strong
random number generator in only 12 lines of code.

# rg32-55.c Creating RadioGatún[32] vectors for long inputs

This program will be compiled in to the “bin/” directory.

rg32-55.c is a program which, given about 20-30 minutes, generates
the file rg32-55-output.txt, which is a series of RadioGatún[32]
hashes for different lengths of strings of the byte 0x55 (ASCII “U”)
repeated.

To use the program, run it like this:

./rg32-55 4

This will tell us the RadioGatún[32] sum of 0x55 repeated four times;
in other words this will tell us the RadioGatun[32] sum of “UUUU” (without
the quotes).

# tiny32api.c

This version of RadioGatún takes a 32-bit unsigned int as its seed.  This
way, we can reduce the code size in applications where we simply use RG32
as a source of random numbers.  This is a compliant subset of
RadioGatún[32]: the generated numbers are the same as the ones the
reference RadioGatún[32] implementation generates when it’s given a
32-bit little endian number as the hash input.

# rg32.cpp RG32 in C++

rg32.cpp is a C++ version of RadioGatún[32]

# rg32.class.php RG32 in PHP

rg32.class.php is a class which performs RadioGatún[32] encryption.
Its api is pretty simple:

```php
<?php
require_once("rg32.class.php");
$rand = new rg32("OrpheanBeholderScryDoubt");
for($a=0;$a<32;$a++) {
	printf("%08x\n",$rand->rg());
}
?>
```

The rg32 constructor takes one argument: The seed for the rg32
random number generator.

The rg32::rg() method returns a 32-bit random integer.

rg.class.php is identical to rg32.class.php, but is written in a
manner which makes it a little easier to experiement with
RadioGatún variants.

# rg32.py RadioGatún[32] in Python3

`rg32.py` is a Python implementation I wrote of RadioGatún[32] back
in 2012; I updated it for Python3 in 2019.  `rg32test.py` (an example of the 
API in use) can be used with `sqa/do.test.sh` to ensure this implementation 
generates correct RadioGatún[32] hashes.

# rg32.py2 RadioGatún[32] in Python2

While `rg32.py` and `rg32test.py` will run in Python2, they are quite 
slow since they use `range()` instead of `xrange()`.  Python2 users
can use the `rg32.py2` class (which uses `xrange()`; and example of its
(nearly identical) API can be seen in `rg32test.py2`.

# rg32.js RadioGatún[32] in Javascript

rg32.js is a Node-compatible Javascript implementation I wrote of 
RadioGatún[32] back in 2010 (slightly updated in 2017 to be Node 
compatible).

rg32test.js is a script to convert the first command line
argument given to it in to a hex digest string compatible with
sqa/do.test.sh.  Note that Javascript treats a string of Unicode
characters as a string where each character is its own number between 0
and 1114111; since the RG32 code assumes that each code point is between
0 and 255, I use https://github.com/mathiasbynens/utf8.js in rg32test.js
to convert a Javascript string in to a form which is 
1) Unicode compatible (anything above 127 becomes multiple numbers) 
2) Standards compliant

If the Javascript string is not converted first, and if the characters
are all in the ISO 8859-1 subset of Unicode, we will get the rg32hash as
if the hash were given an ISO 8859-1 string.  Any Unicode characters with
a numeric value above 255 (i.e. not in ISO 8859-1) will be converted
in to an eight-bit number by rg32.js (to prevent the hash state from
being corrupted).

As an aside, it would be simpler for rg32.js to treat each character as
a 32-bit integer, but compatibility with official test vectors requires
that we give RagioGatún[32] a string of ASCII-encoded characters
converted in to 32-bit integers using the little-endian system.

# rg32.cs RadioGatún[32] in C# (.NET)

rg32.cs is a .NET (C sharp) implementation of the RadioGatún[32] 
algorithm.

