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

This repo has six implementations of RadioGatún[32]:

* Three C-language implementations
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
in 2012.  rg32test.py can be used with sqa/do.test.sh to ensure
this implementation generates correct RadioGatún[32] hashes.

# rg32.js RadioGatún[32] in Javascript

rg32.js is a Node-compatible Javascript implementation I wrote of 
RadioGatún[32] back in 2010 (slightly updated in 2017 to be Node 
compatible).

Note that node converts UTF-8 strings in to ISO 8859-1 strings in
my environment; I use the utf8 Node module to undo this conversion.

