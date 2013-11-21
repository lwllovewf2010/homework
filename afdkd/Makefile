CROSS_COMPILE       =
AS                  =   $(CROSS_COMPILE)as
AR                  =   $(CROSS_COMPILE)ar
CC                  =   $(CROSS_COMPILE)gcc
CPP                 =   $(CC) -E
LD                  =   $(CROSS_COMPILE)ld
NM                  =   $(CROSS_COMPILE)nm
OBJCOPY             =   $(CROSS_COMPILE)objcopy
OBJDUMP             =   $(CROSS_COMPILE)objdump
RANLIB              =   $(CROSS_COMPILE)ranlib
READELF             =   $(CROSS_COMPILE)readelf
SIZE                =   $(CROSS_COMPILE)size
STRINGS             =   $(CROSS_COMPILE)strings
STRIP               =   $(CROSS_COMPILE)strip

CFLAGS				=	-Iinclude -Wall -g3
LDFLAGS				=	-lpthread
MODULES				=	afdkd

OBJS_AFDKD			=	main.o
OBJS_TEST			=	airplaysvr.o socket.o utils.o


all: $(MODULES)

afdkd: $(OBJS_AFDKD)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS_AFDKD)

test: $(OBJS_TEST)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS_TEST)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@$(RM) -f *.o $(MODULES)


