#
# This is a test makefile
#
MOD	:=	main
TEST ?= abc
TEST1 += 1
TEST2 = 123

.PHONY: all
all: $(MOD)
	./$(MOD) ./Makefile

main:
	g++ -o $(@) $M $@.c

clean:
	rm -f $(MOD)

