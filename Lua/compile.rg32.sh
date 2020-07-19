#!/bin/sh
gcc -shared -o rg32.so rg32.c -llua
if uname -a | grep CYGWIN > /dev/null ; then
	ln -s rg32.so rg32.dll
fi
