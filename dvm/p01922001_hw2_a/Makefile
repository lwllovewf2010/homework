all:
	gcc -o move73 move73.c -lcrypto
	javac Foo.java
	./bin/dx --dex --output=foo.jar Foo.class
	unzip foo.jar
	rm -f foo.jar
	./bin/dexdump -d classes.dex 2> list.txt
	./move73 classes.dex list.txt
	jar cvf foo.jar classes.dex META-INF
	rm -rf META-INF list.txt classes.dex

test:
	./dalvik foo.jar Foo

clean:
	rm -f foo.jar
	rm -f Foo.class
	rm -f move73
	rm -f list.txt classes.dex
	rm -rf META-INF

