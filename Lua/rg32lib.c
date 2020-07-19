/* This is a Lua implementation of RadioGatun[32]
 * MIT license (same as Lua)
 * Copyright for Lua bits at end of file
 * Copyright 2020 Sam Trenholme
 */

#include <stdlib.h>
#include <math.h>

// If using C++ uncomment and comment lines as needed
#include <stdint.h> // C99
// #include <cstdint> // C++
// U32 needs to be an unsigned integer precisely 32 bits in size
#define U32 uint32_t

#define lrg32lib_c
#define LUA_LIB

#include <lua.h>

#include <lauxlib.h>
#include <lualib.h>


U32 rg_belt[40], rg_mill[19], rg_phase = 0, rg_place = 0, rg_num = 0;
#define rg U32
#define rgp(a) for(c=0;c<a;c++)
#define rgn w[c*13]^=s;u[16+c]^=s;
void rgf(rg*a,rg*b){rg m=19,A[45],x,o=13,c,y,r=0;rgp(12)b[c+c%3*o]^=a
[c+1];rgp(m){r=(c+r)&31;y=c*7;x=a[y++%m];x^=a[y%m]|~a[(y+1)%m];A[c]=A
[c+m]=x>>r|x<<(32-r)%32;}for(y=39;y--;b[y+1]=b[y])a[y%m]=A[y]^A[y+1]^
A[y+4];*a^=1;rgp(3)a[c+o]^=b[c*o]=b[c*o+o];}void rgl(rg*u,rg*w,char*v
){rg s,q,c,x;rgp(40)w[c]=u[c%19]=0;for(;;rgf(u,w)){rgp(3){for(s=q=0;q
<4;){x=*v++;s|=(x?255&x:1)<<8*q++;if(!x){rgn;rgp(17)rgf(u,w);return;}
}rgn;}}}rg rgi(rg*m,rg*b,rg*a){if(*a&2)rgf(m,b);return m[*a^=3];}

// This is used so we can get ints directly from RadioGatun.  They are 16
// bits in size because 32-bit floats have 24 bits of percision, and it's
// a lot simpler to get 16 bits from rg32 instead of 24 bits
static int rg32_rand16 (lua_State *L) {
  if(rg_phase == 0) {
    rgl(rg_mill, rg_belt, "1234");
    rg_phase = 2;
  }
  if(rg_place == 0) {
    rg_num = rgi(rg_mill, rg_belt, &rg_phase);
    // Fix endian; swap 16-bit parts
    rg_num = ((rg_num & 0xff) << 8 | (rg_num & 0xff00) >> 8 | 
              (rg_num & 0xff0000) << 8 | (rg_num & 0xff000000) >> 8); 
    rg_place = 1;
  } else {
    rg_num >>= 16;
    rg_place = 0;
  }
  lua_Number r = (lua_Number)(rg_num & 0xffff);
  lua_pushnumber(L, r);  /* Number between 0 and 0xffff */
  return 1;
}

// Since Lua tends to use 64-bit numbers for everything (unless we're
// using LUA_32BITS and a newer version of Lua), we can safely get 32
// bits from the RadioGatun[32] state
static int rg32_rand32 (lua_State *L) {
  U32 getnum;
  if(rg_phase == 0) {
    rgl(rg_mill, rg_belt, "1234");
    rg_phase = 2;
  }
  getnum = rgi(rg_mill, rg_belt, &rg_phase);
  // Fix endian to get correct RadioGatun[32] test vectors
  getnum = ((getnum & 0xff) << 24 | (getnum & 0xff00) << 8 | 
            (getnum & 0xff0000) >> 8 | (getnum & 0xff000000) >> 24); 
  lua_Number r = (lua_Number)(getnum & 0xffffffff);
  lua_pushnumber(L, r);  /* Number between 0 and 0xffffffff */
  return 1;
}
   
/* This code is taken from Lua's own math.random, only adapted to use
 * RadioGatun[32] instead of rand() 
 */  
static int rg32_random (lua_State *L) {
  if(rg_phase == 0) {
    rgl(rg_mill, rg_belt, "1234");
    rg_phase = 2;
  }
  /* the `%' avoids the (rare) case of r==1, and is needed also because on
     some systems (SunOS!) `rand()' may return a value larger than RAND_MAX */
  lua_Number r = (lua_Number)(rgi(rg_mill, rg_belt, &rg_phase)%0x7fffffff) 
      / (lua_Number)2147483648.0;
  switch (lua_gettop(L)) {  /* check number of arguments */
    case 0: {  /* no arguments */
      lua_pushnumber(L, r);  /* Number between 0 and 1 */
      break;
    }
    case 1: {  /* only upper limit */
      int u = luaL_checkint(L, 1);
      luaL_argcheck(L, 1<=u, 1, "interval is empty");
      lua_pushnumber(L, floor(r*u)+1);  /* int between 1 and `u' */
      break;
    }
    case 2: {  /* lower and upper limits */
      int l = luaL_checkint(L, 1);
      int u = luaL_checkint(L, 2);
      luaL_argcheck(L, l<=u, 2, "interval is empty");
      lua_pushnumber(L, floor(r*(u-l+1))+l);  /* int between `l' and `u' */
      break;
    }
    default: return luaL_error(L, "wrong number of arguments");
  }
  return 1;
}

// Since RadioGatun[32] takes strings, not numbers, as seeds, we will
// add a method to accept NULL-terminated strings as seed
// Note: The string can not have NULL in it
static int rg32_randomStringseed (lua_State *L) {
  const char *the_seed = luaL_checkstring(L,1);
  rgl(rg_mill, rg_belt, the_seed);
  rg_phase = 2;
  rg_place = 0;
  rg_num = 0;
  return 0;
}

// This acts like math.randomseed; it converts the number in to a
// string then uses that string as a seed for RadioGatun[32]
static int rg32_randomSeed (lua_State *L) {
  lua_Number base_seed = luaL_checknumber(L,1);
  int a, b;
  char c[80], *d, e;
  // Very crude number-to-string conversion
  // We start at 'A' and go up to 'P'.  On an x86 system, this is
  // the 16 nybbles of a 64-bit IEEE-754 floating point number in reverse, so
  // It's MMMMMMMMMMMMMXXS, where S is both sign and three bits of exponent,
  // X is exponent, and M is mantissa.
  // 1 is AAAAAAAAAAAAAPPD (exponent is 0x3ff) , 2 is AAAAAAAAAAAAAAAE 
  // (exponent is 0x400), 4 is AAAAAAAAAAAAABAE (exponent 0x401), 
  // -4 is AAAAAAAAAAAAABAM (sign bit 1, exponent 0x401, so 0xd01...)
  // 1.0001 is BHMKLILNIGAAAPPD, 2.0002 is BHMKLILNIGAAAAAE, 
  // 0 is AAAAAAAAAAAAAAAA, and 1/3 is FFFFFFFFFFFFFNPD
  // This is on an x86 system.  If you use a big-endian system or have
  // lua_Number be something besides a 64-bit binary IEEE-754 floating point
  // number (maybe revive MBF floats, or use 80-bit floats, or whatever),
  // this will generate different random numbers.
  for(a=0;a<sizeof(lua_Number);a++) {
    if(a > 32) break; // Just in case 512-bit floats ever becomes a thing
    d = (char *)&base_seed + a;
    e = *d;
    for(b=0;b<2;b++) { 
      c[(2 * a) + b] = (e & 0x0f) + 'A';
      e >>= 4;
    }
  }
  c[2 * a] = 0;
  rgl(rg_mill, rg_belt, c);
  rg_phase = 2;
  rg_place = 0;
  rg_num = 0;
  return 0;
}

static const luaL_Reg rg32lib[] = {
  {"rand16",     math_rand16},
  {"random",     math_random},
  {"randomstrseed", math_randomStringseed},
  {"randomseed", math_randomSeed},
  {NULL, NULL}
};


/*
** Open math library
*/
LUALIB_API int luaopen_rg32 (lua_State *L) {
  luaL_register(L, LUA_MATHLIBNAME, mathlib);
  return 1;
}

/******************************************************************************
* Copyright (C) 1994-2012 Lua.org, PUC-Rio.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

