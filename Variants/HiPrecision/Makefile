all: libHiPrecisionTest

clean:
	rm -f libHiPrecisionTest *.exe

libHiPrecisionTest: libHiPrecision.c
	${CC} ${CFLAGS} -Wall -DTEST -o libHiPrecisionTest libHiPrecision.c
