#!/bin/sh

PROG="$1"

if [ -z "$PROG" ] ; then
	echo Usage: $0 {program_name}
	echo Where the program is one that performs RG32 hashes to its
	echo first argument
	exit 255
fi

cat Input | awk '{print "'${PROG}\ \''" $0 "'\''"}' | sh > output.test

if ! cmp output.success output.test > /dev/null 2>&1 ; then
	echo Test failed
	exit 255
fi

echo Success
rm output.test
