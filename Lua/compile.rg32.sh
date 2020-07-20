#!/bin/sh
# This is for Linux and Cygwin.
gcc -shared -o rg32.so rg32.c -llua
if uname -a | grep CYGWIN > /dev/null ; then
	ln -s rg32.so rg32.dll
fi

# Mac users may or may not get things to work with 
# gcc -bundle -undefined dynamic_lookup -o rg32.so rg32.c -llua
