#!/bin/sh

PROG="$1"

if [ -z "$PROG" ] ; then
	echo Usage: $0 {program_name}
	echo Where the program is microrg32 compiled
	echo Or, better, call this script from microrg32.test1.sh
	exit 255
fi

cat Input | awk '{print "export P='\''" $0 "'\''"; 
		  print "'${PROG}' @ 8 | head -1"}' | bash | \
	tr -d '\015' > output.test

cat output.success | ./base32.convert.py > output.base32

if ! cmp output.base32 output.test > /dev/null 2>&1 ; then
	echo Test failed
	cat output.8859-1 | ./base32.convert.py > output.base32
	if cmp output.base32 output.test > /dev/null 2>&1 ; then
		echo The script is converting UTF-8 strings in to
		echo ISO 8859-1 strings.  It is otherwise generating
		echo correct base32 RadioGatun[32] hashes.
	fi
	exit 255
fi

echo Success: Good base32 hashes
rm output.test output.base32
