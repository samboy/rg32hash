#!/bin/sh

# This massages microrg32 so that it acts like a "standard" rg32 
# implementation, giving the lower case rg32 hash of argv[1]

P="$1"
export P
echo $( ./microrg32 @ 0 | head -8 | tr -d '\015' | tr -d '\012' | tr -d ' ' )
