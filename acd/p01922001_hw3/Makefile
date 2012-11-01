CC		:=	gcc
CFLAGS	:=	-O2 -Wall
LDFLAGS	:=
MODS	:=	hw3

all: $(MODS)

hw3: hw3.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	@$(RM) -f $(MODS)
