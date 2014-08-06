CFLAGS	:=	-Wall
LDFLAGS	:=	-static

all:
	aarch64-linux-gnu-gcc $(CFLAGS) $(LDFLAGS) -o hello_64 hello.c
	aarch64-linux-gnu-gcc -S -o hello_64.s hello.c
	arm-linux-gnueabi-gcc $(CFLAGS) $(LDFLAGS) -o hello_32 hello.c
	arm-linux-gnueabi-gcc -S -o hello_32.s hello.c
	arm-linux-androideabi-gcc $(CFLAGS) $(LDFLAGS) -o hello_32a hello.c
	arm-linux-androideabi-gcc -S -o hello_32a.s hello.c

clean:
	rm -f hello_64 hello_32 hello_32a *.s

dump:
	aarch64-linux-gnu-objdump -d hello_64 | less

dump32:
	arm-linux-gnueabi-objdump -d hello_32 | less

dump32a:
	arm-linux-androideabi-objdump -d hello_32a | less

run:
	qemu-aarch64 ./hello_64

run32:
	qemu-arm ./hello_32

run32a:
	qemu-arm ./hello_32a

