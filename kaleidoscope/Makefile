CODEDIR		:=	/Users/merck/References/compiler
SWDIR		:=	/Users/merck/Sw

LLVMSRC		:=	$(CODEDIR)/llvm-3.2.src
LLVMBIN		:=	$(SWDIR)/llvm

LLVMBPATH	:=	$(LLVMBIN)/Release+Asserts/bin
LLVMCFG		:=	$(LLVMBPATH)/llvm-config

CC			:=	$(LLVMBPATH)/clang
CPP			:=	$(LLVMBPATH)/clang++
CPPFLAGS	:=	-O3 -Wall $(shell $(LLVMCFG) --cxxflags)
LDFLAGS		:=	$(shell $(LLVMCFG) --ldflags --libs all)

MODS		:=	kal

all: $(MODS)

kal:
	$(CPP) $(CPPFLAGS) $(LDFLAGS) -o $@ main.cpp lexer.cpp ast.cpp

clean:
	$(RM) -rf $(MODS) *.dSYM

