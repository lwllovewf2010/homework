CFLAGS=-I/usr/include/libxml2
LDFLAGS=-lxml2

all:
	gcc $(CFLAGS) -o tree2 tree2.c $(LDFLAGS)
	gcc $(CFLAGS) -o parse1 parse1.c $(LDFLAGS)
	gcc $(CFLAGS) -o reader1 reader1.c $(LDFLAGS)

test: clean all
	./tree2 > test.xml
	./parse1 ./test.xml
	./reader1 ./test.xml

clean:
	rm -f tree2 parse1 reader1 *.xml
