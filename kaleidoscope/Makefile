CODEDIR		:=	/home/merck/Sw
SWDIR		:=	/home/merck/Sw

LLVMSRC		:=	$(CODEDIR)/llvm-3.2.src
LLVMBIN		:=	$(SWDIR)/llvm

LLVMBPATH	:=	$(LLVMBIN)/Release+Asserts/bin
LLVMCFG		:=	$(LLVMBPATH)/llvm-config

CC			:=	$(LLVMBPATH)/clang
CPP			:=	$(LLVMBPATH)/clang++
CPPFLAGS	:=	-O3 -Wall $(shell $(LLVMCFG) --cxxflags)
LDFLAGS		:=	$(shell $(LLVMCFG) --ldflags --libs engine)

MODS		:=	kal

all: $(MODS)

kal:
	$(CPP) -o $@ main.cpp lexer.cpp parser.cpp codegen.cpp $(CPPFLAGS) $(LDFLAGS)

clean:
	$(RM) -rf $(MODS) *.dSYM

