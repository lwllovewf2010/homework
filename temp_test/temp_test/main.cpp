#include <iostream>
#include "TestTemp.h"

template <typename T>
T sq(const T& x) {
  return x + 1;
}

template <int N>
class SomeClass {
 public:
  SomeClass() {
    val = N;
  }
  int GetVal() {
    return val;
  }
  int val;
};

template
<
  typename T = int,
  T (*FUNC)(T) = nullptr
>
class X {
 public:
  X() {func_ = FUNC;}
  ~X() {}
  T Exec(T arg) {return func_(arg);}

 private:
  T (*func_)(T);
};

static const int A = 2;

static int exec_func(int argc) {
  std::cout << "exec_func(" << argc << ")\n";
  return 1;
}

int main(int argc, const char * argv[]) {
  double pi = 3.14;
  sq(pi);
  SomeClass<A> s;
  SomeClass<9> s1;
  X<int, exec_func> function;
  X<> function2;
  TestTemp<float> tt1(9.873f);

  std::cout << "Hello, " << s.GetVal() << ", " << s1.GetVal() << ", " << tt1.GetInner() << "\n";
  function.Exec(10009);

  return 0;
}
