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

print ""
print "Test #4 randint"

q = RadioGatun32(i)
for a in xrange(32):
	print "This number should be between 1 and %d: %d" % (
		2 ** a, q.randint(1,2 ** a))
print ""
for a in xrange(48):
	print ("This number should be between 101 and 121: " + 
			str(q.randint(101,121)))

print ""
print "Test #5 random"
q = RadioGatun32(i)
print "The numbers should be 0.9008508757866456 0.37925485830094763 "
print "0.9431169932393442 and 0.5271410449369754"
print ""
for a in xrange(4):
	print ("This number should be between 0 and 1: " + str(q.random()))
