# Path and settings
TARGET_PRODUCT	= generic
ANDROID_ROOT		= /Users/merck/Projects/Daily/android/ntu-android
BIONIC_LIBC			= $(ANDROID_ROOT)/bionic/libc
PRODUCT_OUT			= $(ANDROID_ROOT)/out/target/product/$(TARGET_PRODUCT)
CROSS_COMPILE		= $(ANDROID_ROOT)/prebuilts/gcc/darwin-x86/arm/arm-linux-androideabi-4.7/bin/arm-linux-androideabi-

# Tool names
AS							=	$(CROSS_COMPILE)as
AR							=	$(CROSS_COMPILE)ar
CC							=	$(CROSS_COMPILE)gcc
CPP							=	$(CC) -E
LD							=	$(CROSS_COMPILE)ld
NM							=	$(CROSS_COMPILE)nm
OBJCOPY					=	$(CROSS_COMPILE)objcopy
OBJDUMP					=	$(CROSS_COMPILE)objdump
RANLIB					=	$(CROSS_COMPILE)ranlib
READELF					=	$(CROSS_COMPILE)readelf
SIZE						=	$(CROSS_COMPILE)size
STRINGS					=	$(CROSS_COMPILE)strings
STRIP						=	$(CROSS_COMPILE)strip

export AS AR CC CPP LD NM OBJCOPY OBJDUMP RANLIB READELF SIZE STRINGS STRIP

# Build settings
CFLAGS					=	-O2 -Wall -fno-short-enums
HEADER_OPS			= -I$(BIONIC_LIBC)/arch-arm/include \
									-I$(BIONIC_LIBC)/kernel/common \
									-I$(BIONIC_LIBC)/kernel/arch-arm
LDFLAGS					=	-nostdlib -Wl,-dynamic-linker,/system/bin/linker \
									$(PRODUCT_OUT)/obj/lib/crtbegin_dynamic.o \
									$(PRODUCT_OUT)/obj/lib/crtend_android.o \
									-L$(PRODUCT_OUT)/obj/lib -lc -ldl

# Installation variables
EXEC_NAME				=	example-app
INSTALL					=	install
INSTALL_DIR			=	$(PRODUCT_OUT)/system/bin

# Files needed for the build
OBJS						=	example-app.o

# Make rules
all: example-app

.c.o:
	$(CC) $(CFLAGS) $(HEADER_OPS) -c $<

example-app: $(OBJS)
	$(CC) -o $(EXEC_NAME) $(OBJS) $(LDFLAGS)

install: example-app
	test -d $(INSTALL_DIR) || $(INSTALL) -d -m 755 $(INSTALL_DIR)
	$(INSTALL) -m 755 $(EXEC_NAME) $(INSTALL_DIR)

clean:
	rm -f *o $(EXEC_NAME) core

distclean:
	rm -f *~
	rm -f *.o $(EXEC_NAME) core

