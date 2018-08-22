#!/bin/sh

# The purpose of this test is to ensure that microrg32 creates
# correct test vectors for high levels of work; we test up to work
# level 7 (higher work levels are very time consuming on this i7-7600U)

# Make sure computing levels 0-9 work
gcc -O3 -o variants ../Variants/rg-variants.c
awk 'BEGIN{b=131072;for(a=1;a<8;a++){
	print "./variants 1234 32 19 0 " b + 15 " | ./base32.convert.py"
	b *= 2
     }}' | sh > output.work.reference

gcc -O3 -o microrg32 ../C/microrg32.c
export P="1234"
rm -f output.test
touch output.test
for a in 1 2 3 4 5 6 7 ; do
	./microrg32 $a 8 | head -1 >> output.test
done

if ! cmp output.work.reference output.test > /dev/null 2>&1 ; then
	echo Test fail
	exit 1
fi

echo Test pass
rm -f output.test microrg32 microrg32.exe variants variants.exe
