all:
	gcc -o elfgen main.c

test:
	dd if=/dev/zero of=./a.bin bs=1024 count=1
	./elfgen < a.bin > a.elf32
	chmod 755 a.elf32

clean:
	rm -f elfgen a.elf32 a.bin

