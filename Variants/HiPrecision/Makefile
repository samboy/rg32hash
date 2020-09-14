all: libHiPrecisionTest

clean:
	rm -f libHiPrecisionTest *.exe

libHiPrecisionTest: libHiPrecision.c
	${CC} ${CFLAGS} -DTEST -o libHiPrecisionTest libHiPrecision.c
