#!/bin/sh

# Since there was a bug which only showed up with "clang -O2" and 
# "clang -O3" (it passed with all gcc and tcc options), we now have
# a test to make tinyrg32.c works with all compilers and all flags

PROGRAM="$1"
if [ -z "$PROGRAM" ] ; then
	PROGRAM="../C/tinyrg32.c"
fi

for cc in gcc clang ; do
    for opt in '-g' '' '-O1' '-O2' '-O3' ; do
        $cc $opt -o test.exe $PROGRAM
        echo Program $PROGRAM with compiler $cc and flags \'$opt\'
        ./do.test.sh ./test.exe
    done
done
