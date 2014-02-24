CROSS_COMPILE		=	arm-none-linux-gnueabi-
AS					=	$(CROSS_COMPILE)as
AR					=	$(CROSS_COMPILE)ar
CC					=	$(CROSS_COMPILE)gcc
CPP					=	$(CC) -E
LD					=	$(CROSS_COMPILE)ld
NM					=	$(CROSS_COMPILE)nm
OBJCOPY				=	$(CROSS_COMPILE)objcopy
OBJDUMP				=	$(CROSS_COMPILE)objdump
RANLIB				=	$(CROSS_COMPILE)ranlib
READELF				=	$(CROSS_COMPILE)readelf
SIZE				=	$(CROSS_COMPILE)size
STRINGS				=	$(CROSS_COMPILE)strings
STRIP				=	$(CROSS_COMPILE)strip

CFLAGS				=	-Iinclude
CFLAGS				+=
#CFLAGS				+=	-g3

LDFLAGS				=

TGTS				=	hello


all: $(TGTS)


.o:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<


clean:
	$(RM) *.o $(TGTS)


sym:
	$(OBJDUMP) -t hello


dis:
	$(OBJDUMP) -d hello
	


