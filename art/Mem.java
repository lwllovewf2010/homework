class Mem {

  static Test[] abc;
  static Test[] ccc;
  static Test[] ddd;

  public static void main(String args[]) {
    while (true) {

      abc = new Test[4096];
      ccc = new Test[8192];
      ddd = new Test[10240];
      //System.out.println("Hello World");
    }
  }
};

