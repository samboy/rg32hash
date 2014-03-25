all: rg32hash

clean:
	rm -f rg32hash *.exe

rg32hash: rg32hash.c
	gcc -O3 -o rg32hash rg32hash.c
