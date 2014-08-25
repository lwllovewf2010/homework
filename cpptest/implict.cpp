#include <iostream>

class Foo {
 public:
  /*explicit*/ Foo(int x)
    : num(x) {
  } 

  int GetFoo() {
    return num;
  }

 private:
  int num;
};

int DoBar(Foo foo) {
  return foo.GetFoo();
}

int main(int argc, char** argv) {

  std::cout << "Return = " << DoBar(42) << "\n";
  return 0;
}

