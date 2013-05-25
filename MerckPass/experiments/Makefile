LLVMPATH	:= /Users/merck/Desktop/llvm/
LLVMBIN		:= $(LLVMPATH)Debug+Asserts/bin/
LLVMLIB		:= $(LLVMPATH)Debug+Asserts/lib/
LLVMOPTS	:= -time-passes

MODULE		:= mytest
PASSNAME	:= MerckPass
PASSOPTS	:= -MerckPass -MerckPass2
SHLIBEXT	:= .dylib

POLLYPATH	:= $(LLVMPATH)tools/polly/Debug+Asserts/lib/
POLLYLIB	:= LLVMPolly
POLLYOPTS	:= -S -mem2reg -loop-simplify -polly-indvars

.PHONY: all polly

all: $(MODULE).bc
	$(LLVMBIN)opt -load $(LLVMLIB)$(PASSNAME)$(SHLIBEXT) $(PASSOPTS) $(LLVMOPTS) < $< > /dev/null

polly: $(MODULE).bc
	$(LLVMBIN)opt -load $(POLLYPATH)$(POLLYLIB)$(SHLIBEXT) $(POLLYOPTS) $(LLVMOPTS) < $< > $(MODULE).ll

$(MODULE).bc: $(MODULE).c
	$(LLVMBIN)clang -emit-llvm $< -c -o $@

clean:
	$(RM) $(MODULE).bc

