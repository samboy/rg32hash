# A thought experiment: Infinite Radio Gatun

For all practical purposes, in the universe that exists, RadioGatun may
as well be infinite.  With 1856 bits in the 32-bit variant of RG, that
gives us more numbers than the number of particles in the observable
universe (296 [10 ** 89] bits) multiplied by the age of the universe in Planck
units (202 bits) (a Planck unit is the shortest measurable unit of time
according to standard physics; there are 10 ** 44 or so Planck time units
in a single second).  However, standard RadioGatun, while having more 
states than can be represented in the universe as we understand it,
is still finite.

So, can we make an infinite RadioGatun variant?  In a word, yes.

## Sexy primes

The belt and mill width are defined in the RadioGatun spec as being 19 and
13 -- a "sexy prime" pair.  While there is yet a rigorous mathematical
proof that there are an infinite number of "sexy prime" pairs (sex here
is simply Latin for six; a sexy prime pair are two prime numbers six
apart from each other), it is conjectured that the number is infinite,
and we have found arbitrarily large sexy prime pairs.

## Infinite Radio Gatun

So, let us make a simple variant of RadioGatun where the belt and
mill are variable width.  The belt width is the lower sexy prime; 
the mill width is the upper sexy prime.  During the mill-to-belt
feedforward, we exclusive or (belt width - 1) numbers from the
mill with elements in the belt.  The final three numbers from 
the mill (as well as the first column of the belts) is exclusive
ored during the input mapping stage. 

The number of blank rounds is determined as follows:

* We have a minimum of 16 blank rounds
* The number of blank rounds is the mill width minus three (so, for
  standard RadioGatun with a mill width of 19, we have 16 blank rounds)
* If the mill width is greater than 40, we divide the mill width by 40,
  rounding down.  The resulting number is added to the number of blank   
  rounds we have.

## Computation cost

The computation needed to hash a short string is the square of the size of
our RadioGatún variant (order _n_ calculations per Beltmill calculation,
multiplied by order _n_ blank rounds).  As the string we hash increases in
length to infinity, however, the time needed to hash it is linear to the
size of our mill (but, remember, for the blank rounds to stay a small
part of the total calculation of a hash, the size of the string to hash
has to get bigger and bigger for bigger mill size variants of RG).

## Some test vectors

Here is the string '1' (i.e. the one-byte string with the byte 0x31)
hashed with varying mill widths of RadioGatun[32]:

* Mill width 11 (16 blank rounds)
  `1d41f7ea542c434f69cf78f16f638a9d6970d109818a16433f4f4c66d9f4e5e3`
* Mill width 13 (16 blank rounds)
  `e961db4a8c331ac55eb47cba4e92b00b388028cf9579acf3c54a69b3eabde592`
* Mill width 17 (16 blank rounds)
  `fc96516ef47f9061a904cdaa0e4f2671b540b0f00bbdb84ff811163b95f2eae8`
* Mill width 19 (16 blank rounds; standard RadioGatun[32])
  `896c580ef77a68c36b040dcd5a650c09b8b5d915ece298ad0210a21ed5811c95`
* Mill width 23 (20 blank rounds)
  `33df6ddb204b4c7740b7a5409eab5922694e373a60e46007d22c68d062fd0a02`
* Mill width 29 (26 blank rounds)
  `bd395bebf0edec3b45453d6b78a6786a5b7cde13549af96f32324290251fc631`
* Mill width 37 (34 blank rounds)
  `de7ae2ae60a9b628a0a64c7d0af77cccce28dbb8253d85598647b6f60555b3e7`
* Mill width 43 (41 blank rounds; we add one because 43/40 rounded down
  is one)
  `ef4aaad91b4b6e44543b18f21aa23e64ea813f0c2984e9b22b2242b109dbb3bc`

## Using rg-variants.c to get these test vectors

```
cc -O3 -o rg-variants rg-variants.c
./rg-variants '1' 32 11 0 16
./rg-variants '1' 32 13 0 16
./rg-variants '1' 32 17 0 16
./rg-variants '1' 32 19 0 16
./rg-variants '1' 32 23 0 20
./rg-variants '1' 32 29 0 26
./rg-variants '1' 32 37 0 34
./rg-variants '1' 32 43 0 41
```

The arguments are:

* The string to hash
* The word width, in bits.  Can be 8, 16, 32, or 64 (any multiple of 
  8 between 8 and 64 actually)
* The mill width
* The operation to use in part of the mill; for standard RadioGatun,
  we make this 0
* The number of blank rounds

# RV200

I am proposing a modest RadioGatún variant, which I call RV200
(Radiogatún Variant 200 bytes).

##Definition
RV200 is identical to RadioGatún, except:

*   The belt rows are 11 words wide, and the mill is 17 words wide.

*   Only ten (not 12) words are exclusive ored in the mill-to-belt
    feedforward stage

*   When mapping the input, it’s the final three words of the mill that
    are altered. In RadioGatún, those are words 17, 18, and 19 of the mill
    (16-18 if using zero-indexed arrays); the indexes are two less in RV200:
    14, 15, and 16, or 13-15 when using zero-indexed arrays.

*   The inner Gamma operation, instead of being (a[(y+1)%m]|~a[(y+2)%m]),
    is (a[(y+1)%m]-a[(y+2)%m]) (bitwise or/not is replaced with i
    subtraction).

*   The only allowed word sizes are 16 bits, 32 bits, and 64 bits. The
    16-bit version is called “RV100”, the 32-bit version is “RV200”,
    and the 64-bit version is “RV400”. The primary version, and the only
    version I provide test vectors for, is the 32-bit one: RV200.

*   32-bit RV200 is suitable for 256-bit hashes and as a stream cipher
    with 256 bits of key security. RV100 is suitable as a stream cipher
    with 128 bits of security. RV400 may be suitable for 512-bit hashes
    or as a stream cipher with 512 bits of security.

*   RV200’s behavior is undefined when the input is not an even number
    of octets in size. In other words, there is no specification for,
    say, its output if given a 3-bit input. Only multiples of eight bits
    are allowed for the input given to RV200.

## Rationale
The reason I am making a RadioGatún (RG) variant is because RG is one
of the fastest hashes out there, faster than MD5. It also is a very
small, efficient stream cipher. However, the original RG design has
some “hairline crack” issues which might become a realistic attack
in the future; as Schneier has said: “Attacks always gets better,
never worse.”

The reason for the changes in the design:

*   The “Gamma” step has bitwise or/not replaced by
    subtraction. Pretty much all of the attacks against RG out there start
    off with “since RadioGatún’s Gamma function has a low algebraic
    degree...” By using a different operation with very different
    mathematical properties, we thwart those kinds of attacks.

*   The disadvantage of using subtraction is that, while it is just as
    fast when implemented as software (indeed, faster, since we only have 
    to do one, not two operations), at the hardware level, subtraction
    needs five times (or six times, depending on how it is done) as many
    gates as or/not.

    However, software implementations are far more common than hardware
    implementations; for people who are designing hardware and something
    which is very fast when implemented in silicon, RG’s direct
    successor, SHA-3 (Keccak), is probably a better choice.

*   Subtraction is done instead of addition because it more closely
    resembles bitwise or/not, and because, like RG’s or/not, 
    a-b != b-a.

*   The belt and mill are made smaller to give RV200 a slight performance
    boost compared to RG, and to make the algorithm fit better on
    embedded chipsets. RG32 (RG with 32-bit words) needs 232 bytes to
    store its state; that leaves too little room on embedded chips with
    256 bytes of ram, since we need seven temporary words in addition to
    the memory the state uses to calculate the mill. By using only 200
    bytes, 56 bytes can be used for temporary variables or the stack,
    which is much more breathing room.

*   The mill function for RV200 can be calculated using only 228 bytes,
    including the 200 bytes used to store its static state. This allows
    RV200 to fit inside, as just one example, a 68 cent ATTINY45-20MU
    with 256 bytes of RAM, instead of having to use a ATTINY85-20MU with
    522 bytes of RAM which costs 77 cents — a 12% cost savings.

*   Since RG has a sponge security claim based on the size of the mill,
    RV200 gives us 272 bits of claimed security instead of the 304 bits
    of security 32-bit RadioGatún claims to have. That’s still over
    256 bits of security.

*   The smallest version of RG/RV with usable security is the version
    using 16-bit words, which gives us 128 bits of security. The only
    reason to have smaller RV variants is to perform cryptographic
    research on them. There is no real-world reason to have 21-bit,
    22-bit, and so on word sizes; indeed Keccak (RG’s direct successor)
    forces the word size to be a power of two — All modern non-embedded
    CPUs work with 8-bit, 16-bit, and 32-bit words, with a large number
    of CPUs also able to work with 64-bit words. Unless you still have
    a PDP-10 using 36-bit words in your closet, other word sizes just
    don’t make sense.

    Mathematically, the triangular numbers RV uses to determine the
    rotation amounts work best when the word size is a power of 2.

*   The reason why the primary word size is 32-bit is because there are
    still a lot of places where 64-bit words are inconvenient:
    Javascript’s bitwise operations only work with 32-bit words; and PHP
    compiled on 32-bit systems can not work with 64-bit words. Even here
    in the mid-2010s, the majority of smartphones still use 32-bit words.

*   By leaving non-octet-length inputs undefined, we could, in the future,
    propose RV200 variants by simply changing the final “padding”
    byte, which is currently hard-coded to one.

*   Looking at the number of cycles in the mill of a reduced word size
    RV200 variant (2-bit words), the RV200 mill function looks like a
    random permutation (the number of cycles a N-bit random permutation
    will normally have is log(2) * N; therefore, we would expect a 34-bit
    random permutation to have 23 or 24 cycles, and the mill in 2-bit
    word size RV200 has 23 cycles)

## Test vectors
Some test vectors for RV200:

```
RV200("") = b8d32785ded8f828b3ba74293dcdf433 a7b6abf40848248c3037c06712a3494b
RV200("1") = 3c619a527f6bcc4f1050b8bcbed7c6e1 c351feacf5379af7644ac51f3bcbf223
RV200("12") = df251c4cb0ec7273af6c072354fa92e2 d0ccbe22f050ad8bbcb90d58db47555a
RV200("1234") = ea5e5b6bbe714b3cede6dbb4ba0835c2 c2fb13945d41f633e0844ce7faa56865
RV200("12345678") = 73b99ad6b76960e868ef99a5f76b4eb2 e0f1c12bd71ab5cf0afb926e76832cab
RV200("123456789") = 230f97664a358286232d35e2953e05a7 9f02a689b4b7196b3891940c20e16393
RV200("12345678901234567890") = 022601dc5cfa0efda12bc0e273456c5d aea35671c49bcf716799cef9d5ca747e 
```

## Infinite RV200

We can vary the mill size with RV200.

Here is the ASCII string "1" (0x31) with different mill widths

* Mill width 11, 16 blank rounds: `455a1cd0928432c330bee92e921577a139c66f2af4860ec5ff85886344331900`
* M13/R16 `8e6bb4b27013a3f505d76c38c8a5e02f18ad9027107bb31141010545ccb88cbd`
* M17/R16 `3c619a527f6bcc4f1050b8bcbed7c6e1c351feacf5379af7644ac51f3bcbf223`
* M19/R16 `86352c01534f80964af39288f88c33ccc57c3168f22672844e27bd8c3c4d78dc`
* M23/R20 `34f3545c9b02196a9a2edcaa8f4741586205562fd264192a1110d1417aaf530a`
* M29/R26 `447207da61a13ea977dba3166c5466b88a2dbe39352ec97fa8e88725ee88b1fa`
* M37/R34 `166e663c763dd43fad03e6e3766589457679dd89728e90d645efa04c550c4e4c`
* M43/R41 `ffe5642e4d2880921e48a044004eb01e30f70784a408e5c54ec3fbb507a184d8`

To generate these test vectors:

```
./rg-variants '1' 32 11 1 16
./rg-variants '1' 32 13 1 16
./rg-variants '1' 32 17 1 16
./rg-variants '1' 32 19 1 16
./rg-variants '1' 32 23 1 20
./rg-variants '1' 32 29 1 26
./rg-variants '1' 32 37 1 34
./rg-variants '1' 32 43 1 41
```

## A thought on the Gamma step

The mill function for RadioGatún includes this operation:

```c
x = 1;
y = 2;
mill[i] = mill[i] ^ (mill[(i + x) % millsize] | (~mill[(i + y) % millsize)]);
```

This is the “gamma” step in RadioGatún.

Observe the constants `x` and `y` above.  As it turns out, for this gamma
operation (|~), we need it so `y = x * 2`, i.e. y is x multiplied by two 
(so, if x is one, y is two; if x is two, y is four; and so on).  If we
do not do this, the mill function is no longer invertible, which is
not desired.

However, in RV200, the gamma step looks like this:

```c
x = 1;
y = 2;
mill[i] = mill[i] ^ (mill[(i + x) % millsize] - mill[(i + y) % millsize]);
```

Here, instead of "or not" (`|~`) in the core of the gamma step, we
instead perform subtraction.  When we do this, the requirement that
y be x multiplied by two no longer applies.  We can have, for example,
x be 2 and y be three, and still have an invertible mill function.

