#
# This is a test makefile
#
ABC = 123 
$(ABC) = 986
A=Hello

all: $(ABC)
	gcc -o test test.c

123:
	echo "123" $ABC

986:
	echo "986"

clean:
	rm -f test

