OS			:=	$(shell uname -s)

ifeq ($(OS), Darwin)
CODEDIR		:=	/Users/merck/References/compiler
SWDIR		:=	/Users/merck/Sw
else
CODEDIR		:=	/home/merck/Sw
SWDIR		:=	/home/merck/Sw
endif

LLVMSRC		:=	$(CODEDIR)/llvm-3.2.src
LLVMBIN		:=	$(SWDIR)/llvm

LLVMBPATH	:=	$(LLVMBIN)/Release+Asserts/bin
LLVMCFG		:=	$(LLVMBPATH)/llvm-config

CC			:=	$(LLVMBPATH)/clang
CPP			:=	$(LLVMBPATH)/clang++
CPPFLAGS	:=	-O3 -Wall $(shell $(LLVMCFG) --cxxflags)
LDFLAGS		:=	$(shell $(LLVMCFG) --ldflags --libs core jit native)
LDFLAGS		+=	-rdynamic

MODS		:=	kal
FILES		:=	main.cpp lexer.cpp parser.cpp codegen.cpp

all: $(MODS)

kal:
	@$(CPP) -o $@ $(FILES) $(CPPFLAGS) $(LDFLAGS)

clean:
	@$(RM) -rf $(MODS) *.dSYM

