BPATH		:=	/Users/merck/Desktop/llvm/Debug+Asserts/bin
LLVMCFG		:=	$(BPATH)/llvm-config

CC			:=	gcc
CPP			:=	g++
CFLAGS		:=	-O3 -Wall -shared $(shell $(LLVMCFG) --cxxflags)
LDFLAGS		:=	$(shell $(LLVMCFG) --ldflags --libs all)

CLANGINCS	:=	-O3 -Wall $(shell $(LLVMCFG) --cxxflags) \
				-I/Users/merck/Desktop/llvm/tools/clang/include

CLANGLIBS	:= 	$(shell $(LLVMCFG) --ldflags --libs all) \
				-lclangLex \
				-lclangFrontend \
				-lclangParse \
				-lclangSema \
				-lclangEdit \
				-lclangAnalysis \
				-lclangAST \
				-lclangBasic \
				-lclangDriver \
				-lclangSerialization \
				-lLLVMMC \
				-lLLVMSupport

MODS		:=	Hello LocalRE GlobalRE ClangTest

all: $(MODS)

Hello:
	@$(CPP) $(CFLAGS) $(LDFLAGS) -o $(addsuffix .so, $@) $(addsuffix .cpp, $@)

LocalRE:
	@$(CPP) $(CFLAGS) $(LDFLAGS) -o $(addsuffix .so, $@) $(addsuffix .cpp, $@)

GlobalRE:
	@$(CPP) $(CFLAGS) $(LDFLAGS) -o $(addsuffix .so, $@) $(addsuffix .cpp, $@)

ClangTest:
	$(CPP) $(CLANGINCS) $(CLANGLIBS) -o $@ $(addsuffix .cpp, $@)

clean:
	@$(RM) -rf $(MODS) *.so *.dSYM

