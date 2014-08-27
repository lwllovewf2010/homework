# LLVM
OS :=  $(shell uname -s)

ifeq ($(OS), Darwin)
SWDIR :=  /Users/merck/Sw
else
SWDIR :=  /home/merck/Sw
endif

LLVMBIN   :=  $(SWDIR)/llvm
LLVMBPATH :=  $(LLVMBIN)/bin/
LLVMCFG   :=  $(LLVMBPATH)llvm-config

# Compiler
CROSS_COMPILE :=
AS       := $(LLVMBPATH)clang
AR       := $(CROSS_COMPILE)ar
CC       := $(LLVMBPATH)clang
CPP      := $(LLVMBPATH)clang++
LD       := $(CROSS_COMPILE)ld
NM       := $(CROSS_COMPILE)nm
OBJCOPY  := $(CROSS_COMPILE)objcopy
OBJDUMP  := $(CROSS_COMPILE)objdump
RANLIB   := $(CROSS_COMPILE)ranlib
READELF  := $(CROSS_COMPILE)readelf
SIZE     := $(CROSS_COMPILE)size
STRINGS  := $(CROSS_COMPILE)strings
STRIP    := $(CROSS_COMPILE)strip

CPPFLAGS	:=	-g3 -Wall $(shell $(LLVMCFG) --cxxflags)
LDFLAGS		:=	$(shell $(LLVMCFG) --ldflags --libs core jit native)
LDFLAGS		+=	-rdynamic

MODS		:=	kal
FILES		:=	main.cpp lexer.cpp parser.cpp codegen.cpp

all: $(MODS)

kal:
	@$(CPP) -o $@ $(FILES) $(CPPFLAGS) $(LDFLAGS)

clean:
	@$(RM) -rf $(MODS) *.dSYM

