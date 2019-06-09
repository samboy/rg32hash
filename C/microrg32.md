# microrg32.c

microrg32.c is a security hardened version of tinyrg32.c:

* It generates only base-32 passwords, which are more secure than using
  hex numbers for passwords
* It grabs the secret key from the environment instead of the command
  line; this helps keep things secure on systems where one’s command 
  line is visible to untrusted accounts.
* It allows one to define how much work is needed to generate a password;
  this makes attacks where someone tries to determine a master password
  based on a generated password more difficult.

## Compile

```
cc -O3 -o microrg32 microrg32.c
```

## Run

```
LEN=4
export P="$LEN:Some secret:example.com"
./microrg32 1 $LEN | head -1
```

Replace `Some secret` with a suitably hard to guess secret string.

The first argument is how much effort we should expend to get this
password; higher numbers mean more effort (this makes brute force
attacks harder).  The second argument is how many bits of entropy the
password should have, divided by 20 (1 gives a 20-bit password, 2 gives
an 40-bit password, 3 60 bits, and so on).  If the second argument has
the value of 0, the passwords will be hex numbers with 32 bits of 
entropy.

## Comparison with tinyrg32

```
LEN=4
tinyrg32 --password - $LEN "$LEN:Some secret:example.com" | head -1
```

Acts the same as:

```
LEN=4
export P="$LEN:Some secret:example.com"
./microrg32 @ $LEN | head -1
```

## Determining how much work to make a password

The work parameter changes the number of blank rounds of RadioGatún we
run.  The RadioGatún specification calls for 16 blank rounds; microrg32
specifies the number of blank rounds with the first argument given
to it, as follows:

First arg | Number of blank rounds
----------|-----------------------
@         | 16 (stock RadioGatún)
1         | 131087
2         | 262159
3         | 524303
4         | 1048591
5         | 2097167
6         | 4194319
7         | 8388623

This parameter acts like the “cost” parameter in bcrypt, where every
increment doubles the amount of work we perform to make the passwords.

## Multiple passwords generated

microrg32 generates multiple passwords when run on the command line.  
This allows one to change their password on a given site by simply
changing which line of microrg32’s output we look at.  For example,
let’s make an initial password for example.com:

```
LEN=4
SECRET="Some long passphrase with random text, like qhohxks5mx9el9v6ujg3t."
export P="$LEN:$SECRET:example.com"
./microrg32 3 $LEN | head -1
```

(Be sure to have SECRET be different than the above example!)

Now, let’s suppose some cybercriminals obtain the password database
at example.com.  While they perform brute force searches to get passwords
(which will, as of 2018, take some time with the above password, which has 
some 80 bits of entropy), we can update our password at example.com with
a new one:

```
LEN=4
SECRET="Some long passphrase with random text, like qhohxks5mx9el9v6ujg3t."
export P="$LEN:$SECRET:example.com"
./microrg32 3 $LEN | head -2 | tail -1
```

Should example.com now require us to change our password every 90 days,
we can do this 90 days later to generate a new password:

```
LEN=4
SECRET="Some long passphrase with random text, like qhohxks5mx9el9v6ujg3t."
export P="$LEN:$SECRET:example.com"
./microrg32 3 $LEN | head -3 | tail -1
```

## The characters used in generated passwords

Note that password strings never have the characters “0” and “1” in
them; they use a custom base32 alphabet designed to minimize code size:

|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
|2|3|4|5|6|7|8|9|a|b|c|d|e|f|g|h|

|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|

Also, to minimize code size, to get five-bit numbers, we simply discard
the upper three bits of each byte, since getting more bytes is so trivial
with the RadioGatún algorithm.

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

## The program

microrg32.c, which generates the passwords, is as follows:

```c
#include <stdio.h> // cc -o microrg32 microrg32.c ; WORK=3 ; LEN=4 #######
#include <stdint.h> // SECRET="Something random like qhohxks5mx9el9v6ujg3"
#include <stdlib.h> // export P="$LEN:$SECRET:x.org" ## Public domain code
#define b(z) for(c=0;c<(z);c++) // ./microrg32 $WORK $LEN | head -1 | tail
uint32_t c,e[42],f[42],g=19,h=13,r,s,n[45],i,k;char*q,y;void m(){int c,j=0
;b(12)f[c+c%3*h]^=e[c+1];b(g){j+=c;i=c*7%g;k=e[i++];k^=e[i%g]|~e[(i+1)%g];
n[c]=n[c+g]=k>>j|k<<-j%32;}for(c=39;c--;f[c+1]=f[c])e[c]=n[c]^n[c+1]^n[c+4
];*e^=1;b(3)e[c+h]^=f[c*h]=f[c*h+h];}int main(int p,char**v){q=getenv("P")
;if(q&&p>2){for(;;m()){b(3){for(r=0;r<4;){f[c*h]^=k=(*q?*q&255:1)<<8*r++;e
[16+c]^=k;if(!*q++){b(17+(1<<*v[1]%32))m();b(983){s=e[1+c%2];if(c%2)m();r=
c;b(4){q=v[p-1];y=y>0?y:*v[2]%16;i=s;s>>=8;if(y){i&=31;i+=i<8?50:89;}//\//
printf(y?"%c":"%02x",i&255);}c=r;if(--y<1||c>981)puts("");}return 0;}}}}}}
```

## Copyright

The above code and this document have both been donated to the public
domain by Sam Trenholme (CC0 license).

