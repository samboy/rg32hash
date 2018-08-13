all: rg32hash rg32-bin tinyrg32 tinyapi rg32-55 rg32-floats nanorg32 \
	tiny32api nanorg32-vertical

clean:
	rm -f bin/t* bin/r* bin/n* bin/*.exe

rg32hash: C/rg32hash.c
	cc -O1 -o bin/rg32hash C/rg32hash.c

rg32-bin: C/rg32-bin.c
	cc -O1 -o bin/rg32-bin C/rg32-bin.c

rg32-floats: C/rg32-floats.c
	cc -O1 -o bin/rg32-floats C/rg32-floats.c

tinyrg32: C/tinyrg32.c
	cc -Wall -O1 -o bin/tinyrg32 C/tinyrg32.c

nanorg32: C/nanorg32.c
	cc -Wall -O1 -o bin/nanorg32 C/nanorg32.c

nanorg32-endian: C/nanorg32-endian.c
	cc -Wall -O1 -o bin/nanorg32-endian C/nanorg32-endian.c

nanorg32-vertical: C/nanorg32-vertical.c
	cc -Wall -O1 -o bin/nanorg32-vertical C/nanorg32-vertical.c

tinyapi: C/tinyapi.c
	cc -Wall -O1 -o bin/tinyapi C/tinyapi.c

tiny32api: C/tiny32api.c
	cc -Wall -O1 -o bin/tiny32api C/tiny32api.c

rg32-55: C/rg32-55.c
	cc -O1 -o bin/rg32-55 C/rg32-55.c

