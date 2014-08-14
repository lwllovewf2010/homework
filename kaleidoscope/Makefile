OS			:=	$(shell uname -s)

ifeq ($(OS), Darwin)
SWDIR		:=	/Users/merck/Sw
else
SWDIR		:=	/home/merck/Sw
endif

LLVMBIN		:=	$(SWDIR)/llvm
LLVMBPATH	:=	$(LLVMBIN)/bin
LLVMCFG		:=	$(LLVMBPATH)/llvm-config

CC			:=	clang
CPP			:=	clang++

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

