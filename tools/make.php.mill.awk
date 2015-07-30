#!/bin/awk -f

# Copyright (c) 2015 Sam Trenholme
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

# This generates PHP code for running RG32's mill with unrolled
# loops

BEGIN {

MILLSIZE=19
WORDSIZE=32
MASK="0xffffffff"
RMASK=0x7fffffff
GAMMA_OFFSET=7
TM="$this->mill["

rotate = 0
# Gamma, Pi
for(i=0; i<MILLSIZE; i++) {
	g1 = (i * GAMMA_OFFSET) % MILLSIZE
	g2 = (g1 + 1) % MILLSIZE
	g3 = (g1 + 2) % MILLSIZE
		
	rotate += i
	rotate %= WORDSIZE
	# Non-POSIX, works with Gawk and Busybox AWK
	r2 = rshift(RMASK,(rotate - 1))
	r3 = sprintf("0x%x",r2)
	r4 = WORDSIZE - rotate
	
	if(rotate == 0) {
		print "$A[" i "] = " TM g1 "]^(" TM g2 "]|~" TM g3 "]);"
	} else {
		print "$tmp = " TM g1 "]^(" TM g2 "]|~" TM g3 "]);"
		L = "$A[" i "]=((($tmp >> " rotate ") & "r3") |"
		print L " ($tmp << " r4 ")) & " MASK ";"
	}

}

# Theta
for(i = 0; i<MILLSIZE; i++) {
	t1 = (i + 1) % MILLSIZE
	t2 = (i + 4) % MILLSIZE
	print TM i "] = $A[" i "]^$A[" t1 "]^$A[" t2 "];"
}

# Iota
print TM "0] ^= 1;"

}
