all:
	mkdir -p MerckSort
	javac -classpath hadoop-core-1.2.1.jar -d MerckSort MerckSort.java
	jar -cvf MerckSort.jar -C MerckSort .
	mkdir -p WordCount
	javac -classpath hadoop-core-1.2.1.jar -d WordCount WordCount.java
	jar -cvf WordCount.jar -C WordCount .
	mkdir -p MerckPageRank
	javac -classpath hadoop-core-1.2.1.jar -d MerckPageRank MerckPageRank.java
	jar -cvf MerckPageRank.jar -C MerckPageRank .

clean:
	rm -rf MerckSort
	rm -f MerckSort.jar
	rm -rf WordCount
	rm -f WordCount.jar
	rm -rf MerckPageRank
	rm -f MerckPageRank.jar

test1:
	hadoop jar MerckSort.jar org.myorg.MerckSort /small.list /small.list.out

test2:
	hadoop jar MerckSort.jar org.myorg.MerckSort /big.list /big.list.out

test3:
	hadoop jar WordCount.jar org.myorg.WordCount /input /output

test4:
	hadoop jar MerckPageRank.jar org.myorg.MerckPageRank /small.graph /small.graph.out

view1:
	hadoop fs -cat /small.list.out/*
