#!/usr/bin/env python

# This is a Python script which calls the code in rg32.py
# It outputs the 256-bit hex hash of the first argument given to the program
# Use this with sqa/do.test.sh

from rg32 import RadioGatun32
import sys

try:
	i = sys.argv[1]
except:
	i = "12345678"
q = RadioGatun32(i)
o = ""
for a in xrange(4):
	o += "%04x%04x" % (q.rng16(),q.rng16())
	o += "%08x" % (q.rng32())
print o

