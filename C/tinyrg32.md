# A tiny password generator

tinyrg32.c is a tiny password generator.  It uses cryptography to generate
passwords with a user-specified level of strength, allowing the user
to choose their level of security vs. convenience.

## How to compile

While tinyrg32.c is quite compact, it compiles without any warnings (even
with `-Wall -Wpedantic` flags turned on) in GCC 7.3.0, Clang 5.0.1, and
TCC 0.9.25.  To compile:

```bash
cc -O3 -o tinyrg32 tinyrg32.c
```

Once it is compiled, place it somewhere in one’s `$PATH` (e.g. 
`/usr/local/bin`)

## How to use tinyrg32

There are four modes of operation:

* We have a version to convert strings in to 256-bit hex digests
* We can output arbitrarily long random noise to standard output
* We can generate a long list of 32-bit pseudo-random hexadecimal numbers
* We can generate passwords with a user-supplied level of entropy

## Generating hash digests

While tinyrg32.c can not hash strings with ASCII nulls in them, it is
able to generate the 256-bit RadioGatún[32] digest for a given string.
Since none of the official RadioGatún[32] test vectors have ASCII nulls,
this mode of operation is useful for verifying that tinyrg32.c generates
correct hashes.

To generate the 256-bit hash for the string "1234":

```
tinyrg32 '1234'
```

Note that, if no argument is supplied to tinyrg32, it will hash the
name of the program; e.g. this will give us the 256-bit RadioGatún[32]
digest for the string "tinyrg32".

```
tinyrg32
```

## Generating a random binary stream

To have tinyrg32 output a random binary stream on standard output, 
invoke it thusly:

```
tinyrg32 --binary '1234'
```

`1234` here is the seed for the binary stream (the same seed will always
generate the same pseudo-random binary numbers).  Note that the seed is
not a number; it is a string.

Note that, unless redirected or piped, the random string will probably
place one’s terminal in an undefined state.

## Generate a list of 32-bit hex numbers

To have tinyrg32 output a list of 32-bit hex numbers on standard output,
invoke it thusly:

```
tinyrg32 --hex --numbers '1234'
```

This will generate a somewhat long (but finite) list of hexadecimal 
numbers.  1234 is the seed, which is a string, not a number:

```
tinyrg32 --hex --numbers 'Look, ma, a string'
```

We can also have tinyrg32 generate an infinite list of 32-bit hex numbers: 

```
tinyrg32 --we --can --make --an --infinite --list --of --hex --numbers '1234'
```

## Generating a secure password

These days, it is a bad idea to use the same password on multiple sites.
Tinyrg32 has a mode of operation where it helps generate strong passwords:

```
tinyrg32 --password _Suff1x 4 'secret:website.example.com' | head -1 
```

Here, _Suff1x (which _must_ start with `_`) is a string we place at
the end of the generated password.  We have the suffix to compensate
for clueless site admins who thing forcing mixed case and making users 
have a number and punctuation mark in the password makes passwords more 
secure (see https://xkcd.com/936/ for why that doesn’t help); it is
not intended to increase security.

The number 4 above is the security level of the password; in other words
the number of bits of entropy in the password, divided by 20.  A
security level of "1" gives us a password with 20 bits of entropy, "2"
40 bits, "3" 60 bits, "4" 80 bits, and so on up until "9" which gives
us 180 bits of entropy.  Keep in mind that a password like this is only
as secure as its secret.

If more than 180 bits of entropy is desired in the password, it’s possible
to use letters to specify more entropy: "j" for 200 bits, "k" for 220 bits,
up to "o" for 300 bits of entropy.  Note that a chain is only as strong
as its weakest link: In order for us to _really_ have a large number
of bits of security, we will need a passphrase which is quite long, as
well as making sure the computer generating the bits is secure (ideally,
offline).  Also, the best public attacks out there, as of 2018, can crack
about 64 bits of entropy (the famous “Shattered” attack breaking
SHA-1 from 2017 took about 2 ** 64 work).

tinyrg32 will generate a somewhat long, but finite, list of passwords;
it is designed to be used with the standard `head` and `tail` tools
included with *NIX to pick the password we can use.  These tools allow
us to print only a single line from a list given to us; for example:

```
tinyrg32 - _B1ah 'secret:example.com' | head -1 | tail -1
```

This will print only the first line.  Should the example.com password
database be hacked, forcing us to change the password, we can change
the password:

```
tinyrg32 - _B1ah 'secret:example.com' | head -2 | tail -1
```

Should they require us to change the password again in a few months:

```
tinyrg32 - _B1ah 'secret:example.com' | head -3 | tail -1
```

When used with a script, it would probably be invoked like this:

```
SECRET="Some long secret which only we know"
WEBSITE="example.com"
LEN=4
tinyrg32 --password _Aa1 $LEN "${LEN}:${SECRET}:${WEBSITE}" | head -2 | tail -1
```

I have already written a script to generate good passwords, called pwgen.sh.
This script allows BASH/Shell script programmers to update the script to
compensate for clueless password rules from site admins (e.g. As I type 
this, southwest.com does not allow a `_` in a password, so we make it a `:` 
instead for that one site).

The reason why the password list is finite is because some setups, such
as the msys toolkit for Windows, will not terminate if a given program
sends an infinite amount of data piped in to another program (such as
the `head` tool), even when the second program is done receiving data
and closes the pipe.

We can, if desired, make an infinitely long list of passwords:

```
tinyrg32 --infinite --password --list _Suff1x 4 'secret:website'
```

Note that password strings never have the characters “0” and “1” in
them; they use a custom base32 alphabet designed to minimize code size:

|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
|2|3|4|5|6|7|8|9|a|b|c|d|e|f|g|h|

|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|

Here is a Python program which reads hex numbers on the standard input
(e.g. "1234abcd321a") and converts them in to the base32 format used
for passwords:

```python
#!/usr/bin/env python

# This script takes hex numbers on the standard input and outputs
# their tinyrg32 base32 form on the standard output

import sys

k = "23456789abcdefghijklmnopqrstuvwx"

for line in sys.stdin:
        while len(line) > 0:
                h = line[0:2]
                line = line[2:]
                if(len(h) > 0):
                    try:
                        a = int(h,16)
                    except:
                        a = -1
                    if a >= 0:
                        a &= 31
                        sys.stdout.write(k[a:a+1])
        print ""
```

## Source code

This source code (as well as this document) is donated to the public 
domain:

```c
#include <stdio.h> // cc -o tinyrg32 tinyrg32.c /////// Public domain code
#include <stdint.h> // ./tinyrg32 --hex --numbers 'A tiny list of numbers'
#define b(z) for(c=0;c<(z);c++) // ./tinyrg32 --binary-stream 'Es la niña'
uint32_t c,e[40],f[40],g=19,h=13,r,s,t,n[40],i,k,y,z;void m(){int c,j=0;b(
12)f[c+c%3*h]^=e[c+1];b(g){j=(c+j)&31;i=c*7%g;k=e[i++];k^=e[i%g]|~e[(i+1)%
g];n[c]=k>>j|k<<(32-j);}for(c=39;c--;f[c+1]=f[c])e[c]=n[c]^n[(c+1)%g]^n[(c
+4)%g];*e^=1;b(3)e[c+h]^=f[c*h]=f[c*h+h];}int main(int p,char**v){char *q=
v[--p],*x=0;for(;;m()){b(3){for(r=0;r<4;){f[c*h]^=k=(*q?*q&255:1)<<8*r++;e
[16+c]^=k;if(!*q++){b(17)m();b(p<3?8:89*p){if(~t&1)m();s=e[(t&1)+1];r=(p&3
)-2?c:1;b(4){i=s;if(p&4){x=v[p-2];y=z=z?z:*v[p-1]%16;i&=31;i+=i<8?50:89;}s
>>=8;printf(p==2||p&4?"%c":"%02x",255&i);}if((++t%8==0||(p&22)==2)&&p-2&&!
y){puts("");}c=r;if(y&&!--z)puts(*x==95?x:"");}if(x)puts(x);return 0;}}}}}
```

