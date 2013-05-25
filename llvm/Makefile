OS          :=  $(shell uname -s)

ifeq ($(OS), Darwin)
CODEDIR     :=  /Users/merck/References/compiler
SWDIR       :=  /Users/merck/Sw
else
CODEDIR     :=  /home/merck/Sw
SWDIR       :=  /home/merck/Sw
endif

LLVMSRC     :=  $(CODEDIR)/llvm-3.2.src
LLVMBIN     :=  $(SWDIR)/llvm

LLVMBPATH   :=  $(LLVMBIN)/Release+Asserts/bin
LLVMCFG     :=  $(LLVMBPATH)/llvm-config

CC          :=  $(LLVMBPATH)/clang
CPP         :=  $(LLVMBPATH)/clang++
AS			:=	$(LLVMBPATH)/llvm-as
DIS			:=	$(LLVMBPATH)/llvm-dis
LD			:=	$(LLVMBPATH)/llc

CFLAGS      :=  -Wall $(shell $(LLVMCFG) --cxxflags)
LDFLAGS     :=  $(shell $(LLVMCFG) --ldflags --libs all)

C_MODS		:= hello.bc
AS_MODS		:= mytest.bin

all: $(C_MODS) $(AS_MODS)

%.bc: %.c
	@$(CC) -emit-llvm $< -c -o $@
	@$(DIS) $@ -o $(@:%.bc=%.ll)

%.bin: %.ll
	$(AS) $< -o $(<:%.ll=%.bc)
	$(LD) $(<:%.ll=%.bc) -o $(<:%.ll=%.s)
	$(CC) $(<:%.ll=%.s) -o $@

clean:
	@$(RM) -f *.bc $(C_MODS:%.bc=%.ll) *.bin

