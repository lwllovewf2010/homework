CFLAGS=-g3

all:
	gcc -m32 $(CFLAGS) -o test_inline test_inline.c
	gcc -m32 $(CFLAGS) -o test_no_inline test_no_inline.c
	gcc -m32 $(CFLAGS) -o mytest mytest.c
	gcc -m32 $(CFLAGS) -o mytest_ni mytest_ni.c

clean:
	rm -f test_inline test_no_inline mytest mytest_ni
