CROSS_COMPILE       =
AS                  =   $(CROSS_COMPILE)as
AR                  =   $(CROSS_COMPILE)ar
CC                  =   $(CROSS_COMPILE)gcc
CPP                 =   $(CROSS_COMPILE)g++
LD                  =   $(CROSS_COMPILE)ld
NM                  =   $(CROSS_COMPILE)nm
OBJCOPY             =   $(CROSS_COMPILE)objcopy
OBJDUMP             =   $(CROSS_COMPILE)objdump
RANLIB              =   $(CROSS_COMPILE)ranlib
READELF             =   $(CROSS_COMPILE)readelf
SIZE                =   $(CROSS_COMPILE)size
STRINGS             =   $(CROSS_COMPILE)strings
STRIP               =   $(CROSS_COMPILE)strip

CPPFLAGS			=	-Wall
CPPLDFLAGS			=

MODULES				=	adjacency_matrix

.PHONY: $(MODULES)

all: $(MODULES)

adjacency_matrix: adjacency_matrix.cpp
	@$(CPP) $(CPPFLAHS) $(CPPLDFLAGS) -o $@ $<

clean:
	@$(RM) -f *.o $(MODULES)

