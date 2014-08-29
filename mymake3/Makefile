# LLVM
OS :=  $(shell uname -s)

ifeq ($(OS), Darwin)
SWDIR	:=  /Users/merck/Sw
else
SWDIR	:=  /home/merck/Sw
endif

LLVMBIN		:=  $(SWDIR)/llvm
LLVMBPATH	:=  $(LLVMBIN)/bin/
LLVMCFG		:=  $(LLVMBPATH)llvm-config

# Compiler
CROSS_COMPILE :=
AS       :=	$(CROSS_COMPILE)as
AR       :=	$(CROSS_COMPILE)ar
CC       :=	$(LLVMBPATH)clang
CPP      :=	$(LLVMBPATH)clang++
LD       :=	$(CROSS_COMPILE)ld
NM       :=	$(CROSS_COMPILE)nm
OBJCOPY  :=	$(CROSS_COMPILE)objcopy
OBJDUMP  :=	$(CROSS_COMPILE)objdump
RANLIB   :=	$(CROSS_COMPILE)ranlib
READELF  :=	$(CROSS_COMPILE)readelf
SIZE     :=	$(CROSS_COMPILE)size
STRINGS  :=	$(CROSS_COMPILE)strings
STRIP    :=	$(CROSS_COMPILE)strip

# Common flags
CPPFLAGS	:=	-Iinclude -Wall -fPIC -g3 -DDEBUG_MYMAKE
CPPFLAGS	+=	$(shell $(LLVMCFG) --cxxflags)
ifeq ($(OS), Darwin)
CPPFLAGS	+=	-I/usr/include/c++/4.2.1
endif

LDFLAGS		:=	$(shell $(LLVMCFG) --ldflags --libs core jit native)
ifeq ($(OS), Darwin)
LDFLAGS		+=	-stdlib=libstdc++ -lc++
endif


# libmymake.so
NAME_LIBMYMAKE		:=	libmymake.so
PATH_LIBMYMAKE		:=	src_libmymake
OBJS_LIBMYMAKE		:=	$(PATH_LIBMYMAKE)/lexer.o
OBJS_LIBMYMAKE   +=  $(PATH_LIBMYMAKE)/parser.o
OBJS_LIBMYMAKE		+=	$(PATH_LIBMYMAKE)/codegen.o
CPPFLAGS_LIBMYMAKE	:=	$(CPPFLAGS)
LDFLAGS_LIBMYMAKE	:=	$(LDFLAGS) -shared

# mymake
NAME_MYMAKE			:=	mymake
PATH_MYMAKE			:=	src_$(NAME_MYMAKE)
OBJS_MYMAKE			:=	$(PATH_MYMAKE)/main.o
CPPFLAGS_MYMAKE	:=	$(CPPFLAGS)
LDFLAGS_MYMAKE		:=	$(LDFLAGS) -lmymake -L./

# Modules
MODULES	:=	$(NAME_LIBMYMAKE) $(NAME_MYMAKE)

# Rules
all: $(MODULES)

$(NAME_MYMAKE): $(OBJS_MYMAKE)
	@$(CPP) -o $@ $(OBJS_MYMAKE) $(CPPFLAGS_MYMAKE) $(LDFLAGS_MYMAKE)

$(NAME_LIBMYMAKE): $(OBJS_LIBMYMAKE)
	@$(CPP) -o $@ $(OBJS_LIBMYMAKE) $(CPPFLAGS_LIBMYMAKE) $(LDFLAGS_LIBMYMAKE)

%.o: %.cpp
	@$(CPP) -o $@ $< $(CPPFLAGS) -c

clean:
	@$(RM) -rf $(MODULES) *.dSYM
	$(shell find ./ -name "*.o" -exec rm -f {} \;)

