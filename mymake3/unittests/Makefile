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
AS       := $(CROSS_COMPILE)as
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

CPPFLAGS  :=  -Wall -fPIC -I../include
ifeq ($(OS), Darwin)
CPPFLAGS  +=  -I/usr/include/c++/4.2.1
endif
LDFLAGS   :=  -L../ -lmymake
ifeq ($(OS), Darwin)
LDFLAGS   +=  -stdlib=libstdc++ -lc++
endif

SUBD			:=	files
MODS      :=  tstLexer

all: $(MODS)

tstLexer: tstLexer.cpp
	@$(CPP) $(CPPFLAGS) $(LDFLAGS) -o $@ $<
	@ln -s ../libmymake.so

test:
	@./tstLexer $(SUBD)/test1.mak $(SUBD)/test1.ans
	@./tstLexer $(SUBD)/test2.mak $(SUBD)/test2.ans
	@./tstLexer $(SUBD)/test3.mak $(SUBD)/test3.ans
	@./tstLexer $(SUBD)/test4.mak $(SUBD)/test4.ans
	@./tstLexer $(SUBD)/test5.mak $(SUBD)/test5.ans

update:
	@./tstLexer $(SUBD)/test1.mak $(SUBD)/test1.ans 0 > $(SUBD)/test1.ans
	@./tstLexer $(SUBD)/test2.mak $(SUBD)/test2.ans 0 > $(SUBD)/test2.ans
	@./tstLexer $(SUBD)/test3.mak $(SUBD)/test3.ans 0 > $(SUBD)/test3.ans
	@./tstLexer $(SUBD)/test4.mak $(SUBD)/test4.ans 0 > $(SUBD)/test4.ans
	@./tstLexer $(SUBD)/test5.mak $(SUBD)/test5.ans 0 > $(SUBD)/test5.ans

testbin:
	@dd if=/dev/zero of=$(SUBD)/bigfile.bin count=1024 bs=1024 > /dev/null 2>&1
	@./tstLexer $(SUBD)/bigfile.bin $(SUBD)/bigfile.bin

clean:
	@rm -f $(MODS) *.so $(SUBD)/*.bin

