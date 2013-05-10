CFLAGS=-fno-exceptions -Wno-multichar -march=atom -fno-omit-frame-pointer -O2 -Ulinux -Wa,--noexecstack -Werror=format-security -Wstrict-aliasing=2 -fPIC -fPIE -ffunction-sections -finline-functions -finline-limit=300 -fno-inline-functions-called-once -fno-short-enums -fstrict-aliasing -funswitch-loops -funwind-tables -fstack-protector -mstackrealign -msse3 -mfpmath=sse -m32 -DUSE_SSE2 -DUSE_SSSE3 -D__ANDROID__ -DANDROID -fmessage-length=0 -W -Wall -Wno-unused -Winit-self -Wpointer-arith -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point -O2 -g -fno-strict-aliasing -DNDEBUG -UDEBUG

all:
	gcc -m32 $(CFLAGS) -o test_inline test_inline.c
	gcc -m32 $(CFLAGS) -o test_no_inline test_no_inline.c
	gcc -m32 -o mytest mytest.c
	gcc -m32 $(CFLAGS) -o mytest_ni mytest_ni.c

clean:
	rm -f test_inline test_no_inline mytest mytest_ni
