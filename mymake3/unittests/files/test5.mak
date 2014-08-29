# LLVM
OS											:=  $(shell uname -s)
ifeq ($(OS), Darwin)
CODEDIR									:=  /Users/merck/References/compiler
SWDIR										:=  /Users/merck/Sw
else
CODEDIR									:=  /home/merck/Sw
SWDIR										:=  /home/merck/Sw
endif
LLVMSRC									:=  $(CODEDIR)/llvm-3.3.src
LLVMBIN									:=  $(SWDIR)/llvm

LLVMBPATH								:=  $(LLVMBIN)/Release+Asserts/bin
LLVMCFG									:=  $(LLVMBPATH)/llvm-config

# Compiler
CROSS_COMPILE       		:=
AS                  		:=	$(LLVMBPATH)/clang
AR                  		:=	$(CROSS_COMPILE)ar
CC                  		:=	$(CROSS_COMPILE)gcc
CPP                 		:=	$(LLVMBPATH)/clang++
LD                  		:=	$(CROSS_COMPILE)ld
NM                  		:=	$(CROSS_COMPILE)nm
OBJCOPY             		:=	$(CROSS_COMPILE)objcopy
OBJDUMP             		:=	$(CROSS_COMPILE)objdump
RANLIB              		:=	$(CROSS_COMPILE)ranlib
READELF             		:=	$(CROSS_COMPILE)readelf
SIZE                		:=	$(CROSS_COMPILE)size
STRINGS             		:=	$(CROSS_COMPILE)strings
STRIP               		:=	$(CROSS_COMPILE)strip

# Common flags
CPPFLAGS					:=	-Iinclude -Wall -fPIC -g3 -DDEBUG_GORMAKE
CPPFLAGS					+=	$(shell $(LLVMCFG) --cxxflags)
LDFLAGS						:=
LDFLAGS						+=	$(shell $(LLVMCFG) --ldflags --libs core jit native)

# libgormake.so
NAME_LIBGORMAKE				:=	libgormake.so
PATH_LIBGORMAKE				:=	src_libgormake
OBJS_LIBGORMAKE				:=	$(PATH_LIBGORMAKE)/lexer.o
OBJS_LIBGORMAKE				+=	$(PATH_LIBGORMAKE)/parser.o
OBJS_LIBGORMAKE				+=	$(PATH_LIBGORMAKE)/codegen.o
CPPFLAGS_LIBGORMAKE			:=	$(CPPFLAGS)
LDFLAGS_LIBGORMAKE			:=	$(LDFLAGS) -shared

# gormake
NAME_GORMAKE				:=	gormake
PATH_GORMAKE				:=	src_$(NAME_GORMAKE)
OBJS_GORMAKE				:=	$(PATH_GORMAKE)/main.o
CPPFLAGS_GORMAKE			:=	$(CPPFLAGS)
LDFLAGS_GORMAKE				:=	$(LDFLAGS) -lgormake -L./

# GorgeousMake
NAME_GORGEOUSMAKE			:=	GorgeousMake
PATH_GORGEOUSMAKE			:=	src_$(NAME_GORGEOUSMAKE)
OBJS_GORGEOUSMAKE			:=	$(PATH_GORGEOUSMAKE)/main.cpp
CPPFLAGS_GORGEOUSMAKE		:=	$(CPPFLAGS)
LDFLAGS_GORGEOUSMAKE		:=	$(LDFLAGS) -lgormake -L./

# Modules
MODULES						:=	$(NAME_LIBGORMAKE) $(NAME_GORMAKE) $(NAME_GORGEOUSMAKE)


# Rules
all: $(MODULES)

$(NAME_GORGEOUSMAKE): $(OBJS_GORGEOUSMAKE)
	@$(CPP) -o $@ $(OBJS_GORGEOUSMAKE) $(CPPFLAGS_GORGEOUSMAKE) $(LDFLAGS_GORGEOUSMAKE)

$(NAME_GORMAKE): $(OBJS_GORMAKE)
	@$(CPP) -o $@ $(OBJS_GORMAKE) $(CPPFLAGS_GORMAKE) $(LDFLAGS_GORMAKE)

$(NAME_LIBGORMAKE): $(OBJS_LIBGORMAKE)
	@$(CPP) -o $@ $(OBJS_LIBGORMAKE) $(CPPFLAGS_LIBGORMAKE) $(LDFLAGS_LIBGORMAKE)

%.o: %.cpp
	@$(CPP) -o $@ $< $(CPPFLAGS) -c

clean:
	@$(RM) -rf $(MODULES) *.dSYM
	$(shell find ./ -name "*.o" -exec rm -f {} \;)

