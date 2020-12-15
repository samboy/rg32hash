#!/usr/bin/env python3

# This script takes hex numbers on the standard input and outputs
# their tinyrg32 base32 form on the standard output

import sys

k = "23456789abcdefghijklmnopqrstuvwx"

for line in sys.stdin:
        while len(line) > 0:
                h = line[0:2]
                line = line[2:]
                if(len(h) > 0):
                    try:
                        a = int(h,16)
                    except:
                        a = -1
                    if a >= 0:
                        a &= 31
                        sys.stdout.write(k[a:a+1])
        print("")
