# Lua and RadioGatún[32]

Lua is a very powerful scripting language in a very small package
(120,832 bytes for my size-optimized Windows 32-bit compile, complete
with lfs and bit32 support).

The way Lua keeps things so small is by being a “batteries not included”
language.  Lua has, in every version, kept the language as small as possible.
This means that, for Lua 5.1, there is no direct support for bit operations
such as and/or/xor.  Since both xor and a “or-not” operation are used
by RadioGatún, my Lua 5.1 version of RadioGatún emulates those options
by using 4x4 sboxes (take 4 bits of two numbers, perform a table lookup
to see what the desired value is, rotate the numbers to perform things
on 4 more bits, repeat eight times to perform a 32-bit or/not or xor).

Since the `bit32` library, included with Lua 5.2, the Roblox port of 
Lua 5.1, and my own `lunacy` version of Lua 5.1, can perform bitwise
operations, there is a version using the bit32 library.

In both cases, the libraries should only be used in situations where
it is not practical to add a C library to the Lua scripting engine
being used.

There is also a .c file which compiles in to a rg32 shared object which
can be loaded and used by Lua.

# rg32.c

`rg32.c` is the file one should use if possible.  The shell script
`compile.rg32.sh` can compile it for Linux and Cygwin; Mac users can 
look at the script to figure out how to compile things on a Mac.  The
resulting `.so` (`.dll` on Windows) file can be loaded in Lua:

```lua
require("rg32")
rg32.randomseed("1234")
string.format("%04x",rg32.rand16())
print(rg32.random())
rg32.randomseed(101.1) -- Converted to "101.1"
print(rg32.random(1,5))
print(rg32.random())
```

The API is a superset of Lua’s `math.random`, with the call `rand16` 
(random number from 0 to 65535) added to the usual `randomseed` and 
`random` calls (both of which have an API compatible with Lua’s native 
`math.random`).

Actually, `rg32.randomseed` takes a *string* as its argument.  However,
if a number is given, Lua automatically performs “coercion” and
makes the number a string.

# rg32bit32.lua

This is an implementation of RadioGatún[32] using Lua’s `bit32` library.
Example usage:

```lua
require("rg32bit32")
rs = RG32init("1234")
print(RG32rand32(rs))
```

This version of RadioGatún[32] is a pure Lua interpretation which works
in Lua 5.2 or higher, or Lua 5.1 with the bit32 library back ported to
it (e.g. Roblox or my own Lunacy fork of Lua 5.1).

Note that the numbers are endian swapped compared to RadioGatún[32]’s
reference test vectors; but there are routines which make strings of
hex numbers in the correct byte order, such as:

```lua
require("rg32bit32")
rs = RG32init("1234")
print(makeLittleEndianHex(RG32rand32(rs)))
```

# rg32PureLua.lua

This is an implementation of RadioGatún[32] using pure Lua 5.1 calls,
emulating bit operations with table look ups.

This verion is slower than other versions; only use this in Lua 5.1
environments without `bit32` where it is not possible to add any 
C-language libraries.

Its api is the same as `rg32bit32`:

```lua
require("rg32PureLua")
rs = RG32init("1234")
print(RG32rand32(rs))
```

