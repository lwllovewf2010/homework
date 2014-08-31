//
//  main.cpp
//  cpptest
//
//  Created by Merck Hung on 8/13/12.
//  Copyright (c) 2012 Merck Hung. All rights reserved.
//

#include <iostream>
#include <vector>
#include <bitset>

#include "DerivedClass.h"
#include "functor.h"
#include "UniquePtr.h"

using namespace std;




class BaseAst {
 public:
  virtual ~BaseAst() {}
};

class MyAst {
 public:
  MyAst(const std::vector<std::unique_ptr<BaseAst>*>* names,
                    const std::vector<std::unique_ptr<BaseAst>*>* signs,
                    const std::vector<std::unique_ptr<std::vector<std::unique_ptr<BaseAst>*>>*>* stats) {
    names_.reset(names);
    signs_.reset(signs);
    stats_.reset(stats);
  }

  const std::vector<std::unique_ptr<BaseAst>*>* GetNames() const {
    return names_.get();
  }
  const std::vector<std::unique_ptr<BaseAst>*>* GetPrereqs() const {
    return signs_.get();
  }
  const std::vector<std::unique_ptr<std::vector<std::unique_ptr<BaseAst>*>>*>* GetRules() const {
    return stats_.get();
  }

 private:
  std::unique_ptr<const std::vector<std::unique_ptr<BaseAst>*>> names_;
  std::unique_ptr<const std::vector<std::unique_ptr<BaseAst>*>> signs_;
  std::unique_ptr<const std::vector<std::unique_ptr<std::vector<std::unique_ptr<BaseAst>*>>*>> stats_;
};






class abc {
 public:
  explicit abc(const std::string* name)
    : name_(NULL) {
    name_ = name;
    std::cout << "abc() is called\n";
  }
  virtual ~abc() {}

  void test() {
    std::cout << "abc::test()\n";
  }

 private:
  const std::string* name_;
};

class ddd : public abc {
 public:
  explicit ddd(const std::string* name) : abc(name) {
    std::cout << "ddd() is called\n";
  }
};


typedef string* pstring;


void make_use( unique_ptr<DerivedClass>& p ) {
 
 p->PerformTest( 54321 );
}


void make_use( DerivedClass* p ) {

 p->PerformTest( 12345 );
}


int use_functor( add_x& fctor ) {

 return fctor( 56 );
}


static int UniquePtrCnt = 0;
class TestUniquePtr {
 public:
  TestUniquePtr() {
    UniquePtrCnt = 1;
  }
  virtual ~TestUniquePtr() {
    UniquePtrCnt = 0;
  }


  void setup(int* i) {
    uptr2.reset(i);
  }
  UniquePtr<int> uptr2;

  void setup1(int* i) {
    uptr3 = i;
  }
  int* uptr3;

  void setup2(abc* ooo) {
    myobj.reset(ooo);
  }
  UniquePtr<abc> myobj;
};


int main() {
    
 int cc = 1234;
 int &uiop = cc;
 bitset<32> val(0xFFFFFFFF);
 string str( "00100000" );
 bitset<32> mytest( str );
 int *fe = new int(1);
 int *ty = new int(1);
 std::string tmp = "Test";
 abc *m = new abc(&tmp);

  *fe = 987654;
  *ty = 345667;

  {
    UniquePtr<TestUniquePtr> uptr1(new TestUniquePtr());
    std::cout << "UniquePtrCnt = " << UniquePtrCnt << "\n";
    uptr1->setup(fe);
    uptr1->setup1(ty);
    uptr1->setup2(m);
    std::cout << "*fe = " << *fe << "\n";
    std::cout << "*ty = " << *ty << "\n";
    m->test();
  }
  std::cout << "UniquePtrCnt = " << UniquePtrCnt << "\n";
  std::cout << "*fe = " << *fe << "\n";
  std::cout << "*ty = " << *ty << "\n";
  m->test();

  TestUniquePtr* uptr10 = new TestUniquePtr();
  std::cout << "UniquePtrCnt = " << UniquePtrCnt << "\n";
  //uptr10->setup(fe);
  uptr10->setup1(ty);
  //uptr10->setup2(m);
  std::cout << "*fe = " << *fe << "\n";
  std::cout << "*ty = " << *ty << "\n";
  delete uptr10;
  std::cout << "*fe = " << *fe << "\n";
  std::cout << "*ty = " << *ty << "\n";
  std::cout << "UniquePtrCnt = " << UniquePtrCnt << "\n";
 
 // Test Functor Start
 add_x add_48( 48 );
 add_x add_182( 182 );
 int i = add_48( 192 );
 int y = use_functor( add_48 );
 int z = use_functor( add_182 );
 
	cout << "Functor i = " << i << "\n";
 cout << "Functor y = " << y << "\n";
 cout << "Functor z = " << z << "\n";
 // Test Functor End
 
 
 
 // Test Virtual
 TestClass *dc = new TestClass();
    
 dc->PerformTest( 456 );
 dc->CheckTest();
 dc->TestConst();
    
 delete dc;
 cout << "Next\n";
 
 dc = new DerivedClass( 45 );
 dc->PerformTest( 890 );
 dc->CheckTest();
 
 delete dc;
 
 
 // Test vector
 vector<string> v1( 8, "Test" );
 string text = "Merck";

 v1.push_back( text );
    
 vector<int> ivec;
    
 ivec.reserve( 50 );
    
 cout << "Cap  = " << ivec.capacity() << "\n";
 cout << "Size = " << ivec.size() << "\n";
 ivec.push_back( 123 );
 ivec.push_back( 456 );
 ivec.pop_back();
    
 cout << "Size = " << ivec.size() << "\n";
    
 cout << uiop << endl;
 cout << "v1.size = " << v1.size() << endl;
    
 for( vector<string>::size_type ix = 0 ; ix != v1.size() ; ++ix ) {
        
  cout << "v1[ " << ix << " ] = " << v1[ ix ] << endl;
 }
    
 for( vector<string>::iterator iter = v1.begin() ; iter != v1.end() ; ++iter ) {
        
  cout << *iter << endl;
 }
 
 
 // Test Unique Pointer
 cout << "Unique Pointer Test\n";
 unique_ptr<DerivedClass> foo(new DerivedClass( 99 ));
 make_use( foo );
 make_use( foo.get() );

 
 // Const pointer
 const double pi = 3.14159;
 const double *const pi_ptr = &pi;
 //*pi_ptr = 123.0;
 cout << "*pi_ptr = " << *pi_ptr << "\n";
 
 
 // Const pointer typedef
 string check( "ABC" );
 const pstring cstr = &check;
 //cstr = &str;
 cout << *cstr << "\n";


  std::string mystr = "MyStr";
  ddd *var = new ddd(&mystr);
 
 return 0;
}

