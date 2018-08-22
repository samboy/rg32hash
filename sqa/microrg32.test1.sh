#!/bin/sh

# Since there was a bug which only showed up with "clang -O2" and 
# "clang -O3" (it passed with all gcc and tcc options), we now have
# a test to make microrg32.c works with all compilers and all flags

PROGRAM="$1"
if [ -z "$PROGRAM" ] ; then
	PROGRAM="../C/microrg32.c"
fi

for cc in gcc clang ; do
    for opt in '-g' '' '-Os' '-O1' '-O2' '-O3' ; do
        $cc $opt -o test.exe $PROGRAM
        echo Program $PROGRAM with compiler $cc and flags \'$opt\'
        ./microrg32.test.sh ./test.exe
    done
done

rm -f test.exe test
