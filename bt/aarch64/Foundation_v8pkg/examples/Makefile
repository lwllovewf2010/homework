all: hello.axf

clean:
	rm -f hello.axf hello.o

hello.o: hello.c
	armcc --cpu=8-A.64.no_neon -g -O2 -c -o $@ $<

hello.axf: hello.o
	armlink --cpu=8-A.64.no_neon --ro-base 0x80000000 -o $@ $^
