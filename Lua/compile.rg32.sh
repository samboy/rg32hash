#!/bin/sh
# This is for Linux and Cygwin.
gcc -O2 -shared -o rg32.so rg32.c -llua

# On CentOS 8, run this instead:
# gcc -O2 -fPIC -shared -o rg32.so rg32.c -llua
# Also, go here for lua-devel: 
# http://mirror.centos.org/centos/8.2.2004/PowerTools/x86_64/os/Packages/
# For discussion why lua-devel is no longer part of RedHat proper:
# https://bugzilla.redhat.com/show_bug.cgi?id=1734218

if uname -a | grep CYGWIN > /dev/null ; then
	ln -s rg32.so rg32.dll
fi

# Mac users may or may not get things to work with 
# gcc -bundle -undefined dynamic_lookup -o rg32.so rg32.c -llua
