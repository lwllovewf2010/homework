CC		:=	gcc
CFLAGS	:=	-O2 -Wall
LDFLAGS	:=
MODS	:=	hw3

all: $(MODS)

hw3:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ hw3.c adjlist.c

clean:
	@$(RM) -f $(MODS)
