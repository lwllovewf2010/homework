CODEDIR		:=	/Users/merck/References/compiler/
LLVMSRC		:=	$(CODEDIR)/llvm-3.1.src
LLVMPATH	:=	$(CODEDIR)/llvm31
LLVMBPATH	:=	$(LLVMPATH)/Release+Asserts/bin
LLVMCFG		:=	$(LLVMBPATH)/llvm-config

CC			:=	$(LLVMBPATH)/clang
CPP			:=	$(LLVMBPATH)/clang++
CFLAGS		:=	-O3 -Wall -shared $(shell $(LLVMCFG) --cxxflags)
LDFLAGS		:=	$(shell $(LLVMCFG) --ldflags --libs all)

CLANGINCS	:=	-O2 -Wall $(shell $(LLVMCFG) --cxxflags) \
				-I$(LLVMPATH)/tools/clang/include \
				-I$(LLVMSRC)/tools/clang/include

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

MODS		:=	Hello LocalRE GlobalRE PrintDomTree #ClangTest

all: $(MODS)

Hello:
	@$(CPP) $(CFLAGS) $(LDFLAGS) -o $(addsuffix .so, $@) $(addsuffix .cpp, $@)

LocalRE:
	@$(CPP) $(CFLAGS) $(LDFLAGS) -o $(addsuffix .so, $@) $(addsuffix .cpp, $@)

GlobalRE:
	@$(CPP) $(CFLAGS) $(LDFLAGS) -o $(addsuffix .so, $@) $(addsuffix .cpp, $@)

PrintDomTree:
	@$(CPP) $(CFLAGS) $(LDFLAGS) -o $(addsuffix .so, $@) $(addsuffix .cpp, $@)

ClangTest:
	@$(CPP) $(CLANGINCS) $(CLANGLIBS) -o $@ $(addsuffix .cpp, $@)

clean:
	@$(RM) -rf $(MODS) *.so *.dSYM

