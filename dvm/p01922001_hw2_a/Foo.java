class Foo {

	public static void main(String[] args) {

    final int iterationCount1 = 1000;
    final int iterationCount2 = 1000;
    final int iterationCount3 = 100;
    int counter = 0;

    long startTime = java.lang.System.currentTimeMillis();

    for (int index1 = 0; index1 < iterationCount1; index1++)
      for (int index2 = 0; index2 < iterationCount2; index2++)
        for (int index3 = 0; index3 < iterationCount3; index3++)
          counter++;

    long endTime = java.lang.System.currentTimeMillis();

    int runTime = (int) (endTime - startTime);
    System.out.println("execution time = " + runTime + "ms. count = " + counter);
	}
}
