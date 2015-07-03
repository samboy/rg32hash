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

This repo has three third party implementations of RadioGatún[32].

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

# tinyrg32: Less bloat

For people that feel rg32-bin.c and rg32hash.c are too bloated, I have
also included tinyrg32.c, which is a somewhat unbloated program which
will output either the 256-bit hex hash of a string given to the program
as an argument, an arbitrary long binary string, an arbitrarily long list
of 32-bit random hex numbers, or even (undocumented) an arbitrarily long
continuous stream of hex numbers.

Documentation for tinyrg32.c is included in its source code.

I understand that tinyrg32.c may still have some bloat in it; if anyone 
can help me reduce its size without giving up any of its functionality,
please open up a github ticket with your smaller version of tinyrg32.c
