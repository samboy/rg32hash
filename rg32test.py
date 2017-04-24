#!/usr/bin/env python

# This is a Python script which calls the code in rg32.py
# It outputs the 256-bit hex hash of the first argument given to the program
# Use this with sqa/do.test.sh

from rg32 import RadioGatun32
import sys
do_other_test = 0

try:
	i = sys.argv[1]
except:
	i = "12345678"
	do_other_test = 1
	print "Test #1 64-bit integers; should return"
	print"e69e29ba139c20846116d8ad406e6197f1701d8243cc53bb86f2b72c62320a39"

q = RadioGatun32(i)
o = ""
for a in xrange(4):
	o += "%016x" % (q.rng64())
print o

if do_other_test == 0:
	sys.exit(0)

# Let's do some more tests if not given an arg

print ""
print "Test #2 32-bit integers; should return"
print"e69e29ba139c20846116d8ad406e6197f1701d8243cc53bb86f2b72c62320a39"

q = RadioGatun32(i)
o = ""
for a in xrange(8):
	o += "%08x" % (q.rng32())
print o

print ""
print "Test #3 16-bit integers; should return"
print"e69e29ba139c20846116d8ad406e6197f1701d8243cc53bb86f2b72c62320a39"

q = RadioGatun32(i)
o = ""
for a in xrange(16):
	o += "%04x" % (q.rng16())
print o

print ""
print "Test #3 16-bit and 32-bit integers; should return"
print"e69e29ba139c20846116d8ad406e6197f1701d8243cc53bb86f2b72c62320a39"

q = RadioGatun32(i)
o = ""
for a in xrange(4):
	o += "%08x" % (q.rng32())
	o += "%04x%04x" % (q.rng16(),q.rng16())
print o
