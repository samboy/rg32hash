#!/usr/bin/env python
# -*- coding: utf-8 -*-

# rv200.py 

test_rv200 = 1 # Make this 0 if using this as a library

# Copyright (c) 2012-2015 Sam Trenholme
# 
# TERMS
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# This software is provided 'as is' with no guarantees of correctness or
# fitness for purpose.

# This is a Python implementation of RV200, a RadioGatun variant

"""
I am proposing a modest RadioGatún variant, which I call RV200
(Radiogatún Variant 200 bytes).

==Definition==

RV200 is identical to RadioGatún, except:

*    The belt rows are 11 words wide, and the mill is 17 words wide.

*    Only ten (not 12) words are exclusive ored in the mill-to-belt
     feedforward stage

*    When mapping the input, it’s the final three words of the mill that
     are altered. In RadioGatún, those are words 17, 18, and 19 of the mill
     (16-18 if using zero-indexed arrays); the indexes are two less in RV200:
     14, 15, and 16, or 13-15 when using zero-indexed arrays.

*    The inner Gamma operation, instead of being (a[(y+1)%m]|~a[(y+2)%m]), 
     is (a[(y+1)%m]-a[(y+2)%m]) (bitwise or/not is replaced with subtraction).

*    The only allowed word sizes are 16 bits, 32 bits, and 64 bits. The
     16-bit version is called “RV100”, the 32-bit version is “RV200”,
     and the 64-bit version is “RV400”. The primary version, and the only
     version I provide test vectors for, is the 32-bit one: RV200.

*    32-bit RV200 is suitable for 256-bit hashes and as a stream cipher
     with 256 bits of key security. RV100 is suitable as a stream cipher with
     128 bits of security. RV400 may be suitable for 512-bit hashes or as a
     stream cipher with 512 bits of security.

*    RV200’s behavior is undefined when the input is not an even number
     of octets in size. In other words, there is no specification for, say,
     its output if given a 3-bit input. Only multiples of eight bits are
     allowed for the input given to RV200.

"""

class RV200:
	def __init__(self, m):
		self.wordsize = 32
		self.millsize = 17
		self.beltrows = 3
		self.beltcol = 11
		self.beltfeed = 10
		self.mask = 0xffffffff # 32-bit
		self.index = 0
		(self.a, self.b) = self.seed(m)
	def mill(self,a):
		aa = []
		for g in xrange(self.millsize):
			aa.append(0)
		x = 0
		i = 0
		y = 0
		r = 0
		z = 0
		for i in xrange(self.millsize):
			y = (i * 7) % self.millsize
			r = ((i * (i + 1)) / 2) % self.wordsize
			
			x = (a[ ((y + 1) % self.millsize) ] - 
			     a[ ((y + 2) % self.millsize) ] )
			if(x < 0):
				x = self.mask + x + 1
			x ^= a[y]
			aa[i] = ((x >> r) | (x << (self.wordsize - r))
			        & self.mask)
		for i in xrange(self.millsize):
			y = i
			r = (i + 1) % self.millsize
			z = (i + 4) % self.millsize
			a[i] = aa[y] ^ aa[r] ^ aa[z]
		a[0] ^= 1
		return a
	def belt(self,a,b):
		q = []
		for g in xrange(self.beltrows):
			q.append(0)
		s = 0	
		i = 0
		v = 0
		for s in xrange(self.beltrows):
			q[s] = b[((s * self.beltcol) + self.beltcol - 1)]
		i = self.beltcol - 1
		while i > 0:
			for s in xrange(self.beltrows):
				v = i - 1
				if v < 0:
					v = self.beltcol - 1
				b[((s * self.beltcol) + i)] = (
					b[((s * self.beltcol) + v)])
			i -= 1
		for s in xrange(self.beltrows):
			b[(s * self.beltcol)] = q[s]
		for i in xrange(self.beltfeed):
			s = (i + 1) + ((i % self.beltrows) * self.beltcol)
			b[s] ^= a[(i + 1)]
		a = self.mill(a)
		for i in xrange(self.beltrows):
			a[(i + self.beltcol)] ^= q[i]
		return (a, b)
	def seed(self,m):
		p = []
		for g in xrange(3):
			p.append(0)
		q = 0
		c = 0
		r = 0
		done = 0
		index = 0
		counter = 0
		a = []
		b = []
		for g in xrange(self.millsize):
			a.append(0)
		for g in xrange(self.beltrows * self.beltcol):
			b.append(0)
		for counter in xrange(16777218): # Infinite loop protection
			p[0] = p[1] = p[2] = 0
			for r in xrange(3):
				q = 0
				while q < self.wordsize:
					x = 0
					try:
						x = ord(m[index])
					except:
						x = 1
					index += 1
					if(index > len(m)):
						done = 1
						x = 1
					p[r] |= x << q
					if done == 1:
						for c in xrange(3):
							b[c * 11] ^= p[c]
							a[14 + c] ^= p[c]
						(a,b) = self.belt(a,b)
						for c in xrange(16):	
							(a,b) = self.belt(a,b)
						return (a,b)
					q += 8
			for c in xrange(3):
				b[c * 11] ^= p[c]
				a[14 + c] ^= p[c]
			(a,b) = self.belt(a,b)
		return (a,b) # We should never get here
	def rng(self):
		if (self.index % 4) == 0:
			(self.a, self.b) = self.belt(self.a, self.b)
			self.index += 1
			return (((self.a[1] & 0xff) << 8) | 
				 ((self.a[1] & 0xff00) >> 8))
		self.index += 1
		if (self.index % 4) == 2:
			return(((self.a[1] & 0xff0000) >> 8) |
				((self.a[1] & 0xff000000) >> 24))
		elif (self.index % 4) == 3:
			return(((self.a[2] & 0xff) << 8) |
				((self.a[2] & 0xff00) >> 8))
		elif (self.index % 4) == 0:
			return(((self.a[2] & 0xff0000) >> 8) |
				((self.a[2] & 0xff000000) >> 24))
		else: # Should never get here
			return -1

if test_rv200 == 1:
	for z in ["","1","12","1234","12345678","123456789",
		"12345678901234567890" ]:
		q = RV200(z)
		print "Input: " + z
		for a in xrange(4):
		   print "%04x%04x%04x%04x" % (q.rng(),q.rng(),q.rng(),q.rng())
		print ""
