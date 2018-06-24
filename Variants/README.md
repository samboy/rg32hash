# A thought experiment: Infinite Radio Gatun

For all practical purposes, in the universe that exists, RadioGatun may
as well be infinite.  With 1856 bits in the 32-bit variant of RG, that
gives us more numbers than the number of particles in the observable
universe (266 bits) multiplied by the age of the universe in Planck
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

