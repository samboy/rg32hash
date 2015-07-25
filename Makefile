all: rg32hash rg32-bin tinyrg32 

clean:
	rm -f rg32hash rg32-bin tinyrg32 *.exe

rg32hash: rg32hash.c
	cc -O3 -o rg32hash rg32hash.c

rg32-bin: rg32-bin.c
	cc -O3 -o rg32-bin rg32-bin.c

tinyrg32: tinyrg32.c
	cc -O3 -o tinyrg32 tinyrg32.c
