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
./microrg32 1 4 | head -1
```

The first argument is how much effort we should expend to get this
password; higher numbers mean more effort (this makes brute force
attacks harder).  The second argument is how many bits of entropy the
password should have, divided by 20 (1 gives a 20-bit password, 2 gives
an 40-bit password, 3 60 bits, and so on).

## Comparison with tinyrg32

```
tinyrg32 --password - 4 "4:Some secret:example.com" | head -1
```

Acts the same as:

```
LEN=4
export P="$LEN:Some secret:example.com"
./microrg32 @ 4 | head -1
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

The work parameter acts like the “cost” parameter in bcrypt, where 
every increment doubles the amount of work we perform to make the
passwords.

## Multiple passwords generated

microrg32 generates multiple passwords when run on the command line.  
This allows one to change their password on a given site by simply
changing which line of microrg32’s output we look at.  For example,
let’s make an initial password for example.com:

```
LEN=4
SECRET="Some long passphrase with random text, like qhohxks5mx9el9v6ujg3t."
export P="$LEN:Some secret:example.com"
./microrg32 3 4 | head -1
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
export P="$LEN:Some secret:example.com"
./microrg32 3 4 | head -2 | tail -1
```

Should example.com now require us to change our password every 90 days,
we can do this 90 days later to generate a new password:

```
LEN=4
SECRET="Some long passphrase with random text, like qhohxks5mx9el9v6ujg3t."
export P="$LEN:Some secret:example.com"
./microrg32 3 4 | head -3 | tail -1
```

