all: rg32hash rg32-bin tinyrg32 tinyapi rg32-55

clean:
	rm -f rg32hash rg32-bin tinyrg32 tinyapi rg32-55 *.exe

rg32hash: rg32hash.c
	cc -O3 -o rg32hash rg32hash.c

rg32-bin: rg32-bin.c
	cc -O3 -o rg32-bin rg32-bin.c

tinyrg32: tinyrg32.c
	cc -O3 -o tinyrg32 tinyrg32.c

tinyapi: tinyapi.c
	cc -O3 -o tinyapi tinyapi.c

rg32-55: rg32-55.c
	cc -O3 -o rg32-55 rg32-55.c

