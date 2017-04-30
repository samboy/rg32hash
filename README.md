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

This repo has 10 implementations of RadioGatún[32]:

* Seven C-language implementations
* A PHP implementation
* A Python implementation
* A Node-compatible Javascript implementation

# Compile

A Makefile is included to compile the programs here. To use:

```
make
```

# Using rg32hash

To use this program:

```
rg32hash . > RG32SUMS
```

This will recursively perform a RadioGatun hash on all files and folders
in the current working directory, putting the result in a file named 
"RG32SUMS"

# Using rg32-bin

rg32-bin.c will compile in to a program which outputs an arbitrary long
binary sequence of pseudo-random numbers.

# rg32-floats

rg32-floats.c will compile in to a program which outputs an arbitrary long
sequence of pseudo-random floating point numbers between 0 and 1 (it could
be 0 but never 1).  Note that each number has 32 bits of entropy.

I used this program to verify that RadioGatún[32] passes the “Small Crush”
version of TestU01.

# tinyrg32: Somewhat less bloat

For people that feel rg32-bin.c and rg32hash.c are too bloated, I have
also included tinyrg32.c, which is a somewhat unbloated program which
will output either the 256-bit hex hash of a string given to the program
as an argument, an arbitrary long binary string, an arbitrarily long list
of 32-bit random hex numbers, or even (undocumented) an arbitrarily long
continuous stream of hex numbers.

Documentation for tinyrg32.c is included in its source code.  To expand
on that:

The final argument given to tinyrg32.c is always the key used to 
choose the random numbers generated; the same key will always
generate the same numbers.

The current behavior is this:

```
./tinyrg32: A 32-bit hash of argv[0]
./tinyrg32 '¡Es una niña linda!': 256-bit hash
./tinyrg32 --binary-stream '¡La niña!': Binary stream
./tinyrg32 --hex --numbers '¡Niña!': Ten 32-bit hex numbers
./tinyrg32 --two --alphanum --strings 'ñ': Two alphanumeric strings
./tinyrg32 --infinite --number --of --alphanum -strings 'ñ': Self-explanatory
./tinyrg32 - - - - - - - - - -: Infinite 32-bit hex strings
./tinyrg32 - - - - - - - - - - - - - - - - - -: Infinite 256-bit hex strings
```

I understand that tinyrg32.c still has some bloat in it; which is
why I have made...

# nanorg32 RadioGatún[32] in 607 bytes

nanorg32.c is a C-language RadioGatún[32] implementation in 607 bytes, 
using lines under 75 characters long.  There are no warnings when
compiling with -Wall -Wpedantic in gcc 5.4.0, a recent version
of clang (LLVM), nor are there any warnings or errors when compiled
with TCC 0.9.25.

nanorg32.c is public domain.

# tinyapi.c Tiny rg32 library

tinyapi.c has both a tiny rg32 library for C program and an example of 
the library being used.  Useful when you want to give a C program a strong
random number generator in only 12 lines of code.

# rg32-55.c Creating RadioGatún[32] vectors for long inputs

rg32-55.c is a program which, given about 20-30 minutes, generates
the file rg32-55-output.txt, which is a series of RadioGatún[32]
hashes for different lengths of strings of the byte 0x55 (ASCII “U”)
repeated.

To use the program, run it like this:

./rg32-55 4

This will tell us the RadioGatún[32] sum of 0x55 repeated four times;
in other words this will tell us the RadioGatun[32] sum of “UUUU” (without
the quotes).

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

# rg32.py RadioGatún[32] in Python

rg32.py is a Python implementation I wrote of RadioGatún[32] back
in 2012.  rg32test.py (an example of the API in use) can be used
with sqa/do.test.sh to ensure this implementation generates correct
RadioGatún[32] hashes.

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
