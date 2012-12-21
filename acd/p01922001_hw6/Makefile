BPATH		:=	/Users/merck/Desktop/llvm/Debug+Asserts/bin
LLVMCFG		:=	$(BPATH)/llvm-config

CC			:=	gcc
CPP			:=	g++
CFLAGS		:=	-O3 -Wall -shared $(shell $(LLVMCFG) --cxxflags)
LDFLAGS		:=	$(shell $(LLVMCFG) --ldflags --libs all)
MODS		:=	Hello LocalRE GlobalRE

all: $(MODS)

Hello:
	@$(CPP) $(CFLAGS) $(LDFLAGS) -o $(addsuffix .so, $@) $(addsuffix .cpp, $@)

LocalRE:
	@$(CPP) $(CFLAGS) $(LDFLAGS) -o $(addsuffix .so, $@) $(addsuffix .cpp, $@)

GlobalRE:
	@$(CPP) $(CFLAGS) $(LDFLAGS) -o $(addsuffix .so, $@) $(addsuffix .cpp, $@)

clean:
	@$(RM) -rf $(MODS) *.so *.dSYM
