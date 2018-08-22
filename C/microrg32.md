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
export P="Some secret:example.com"
./microrg32 1 4 | head -1
```

The first argument is how much effort we should expend to get this
password; higher numbers mean more effort (this makes brute force
attacks harder).  The second argument is how many bits of entropy the
password should have, divided by 20 (1 gives a 20-bit password, 2 gives
an 40-bit password, 3 60 bits, and so on).

## Comparison with tinyrg32

```
tinyrg32 --password - 4 "Some secret:example.com" | head -1
```

Acts the same as:

```
export P="Some secret:example.com"
./microrg32 @ 4 | head -1
```
