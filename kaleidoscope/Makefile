CODEDIR		:=	/Users/merck/References/compiler
LLVMSRC		:=	$(CODEDIR)/llvm-3.2.src
LLVMPATH	:=	$(CODEDIR)/llvm32
LLVMBPATH	:=	$(LLVMPATH)/bin/Debug
LLVMCFG		:=	$(LLVMBPATH)/llvm-config

CC			:=	$(LLVMBPATH)/clang
CPP			:=	$(LLVMBPATH)/clang++
CFLAGS		:=	-O3 -Wall -shared $(shell $(LLVMCFG) --cxxflags)
LDFLAGS		:=	$(shell $(LLVMCFG) --ldflags --libs all)

MODS		:=	kal

all: $(MODS)

kal:
	$(CPP) $(CFLAGS) $(LDFLAGS) -o $(addsuffix .so, $@) $(addsuffix .cpp, $@)

clean:
	@$(RM) -rf $(MODS) *.so *.dSYM

