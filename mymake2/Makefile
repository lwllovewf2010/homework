#
# This is a test makefile
#
MOD	:=	token
TEST ?= abc
TEST1 += 1
TEST2 = 123

.PHONY: all
all: $(MOD)
	./$(MOD) Makefile

token:
	flex++ -o $@.cpp $@.l
	g++ -o $@ $@.cpp

clean:
	rm -f $(MOD).cpp $(MOD)

