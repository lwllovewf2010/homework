#include <iostream>

class Foo {
 public:
  /*explicit*/ Foo(int x)
    : num(x),
      str('\0') {
  } 
  Foo(char p)
    : num(0),
      str(p) {
  }

  int GetFoo() {
    return num;
  }

  char GetStr() {
    return str;
  }

 private:
  int num;
  char str;
};

#if 0
int main(int argc, char** argv) {
  Foo myfoo = 'x';

  std::cout << "Return1 = " << myfoo.GetFoo() << "\n";
  std::cout << "Return2 = " << myfoo.GetStr() << "\n";
  return 0;
}
#endif
