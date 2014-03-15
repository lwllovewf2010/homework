all:
	@echo "===================================="
	@echo "Compile move73 & Foo.java"
	@echo "===================================="
	gcc -o move73 move73.c -lcrypto
	javac Foo.java
	@echo "===================================="
	@echo "Convert class into dex"
	@echo "===================================="
	./bin/dx --dex --output=foo.jar Foo.class
	@echo "===================================="
	@echo "Convert MOVE to MOVE73"
	@echo "===================================="
	unzip foo.jar
	rm -f foo.jar
	./bin/dexdump -d classes.dex 2> list.txt > /dev/null
	./move73 classes.dex list.txt
	jar cvf foo.jar classes.dex META-INF
	rm -rf META-INF list.txt classes.dex
	@echo "===================================="
	@echo "Disassemble the MOVE73 ver. foo.jar"
	@echo "===================================="
	unzip foo.jar
	rm -rf META-INF
	./bin/dexdump -d classes.dex 2> /dev/null
	rm -f classes.dex
	@echo "===================================="
	@echo "Execute the MOVE73 foo.jar"
	@echo "===================================="
	./dalvik foo.jar Foo

clean:
	rm -f foo.jar
	rm -f Foo.class
	rm -f move73
	rm -f list.txt classes.dex
	rm -rf META-INF

