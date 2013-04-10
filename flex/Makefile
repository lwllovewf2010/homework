MODS	:=		test1 pascal

all: $(MODS)

test1: test1.l
	flex -o $@.c $<
	gcc -o $@ $@.c

pascal: pascal.l
	flex -o $@.c $<
	gcc -o $@ $@.c

clean:
	rm -f $(MODS) *.c
