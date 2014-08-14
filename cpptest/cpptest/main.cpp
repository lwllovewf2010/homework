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

using namespace std;


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


int main() {
    
 int cc = 1234;
 int &abc = cc;
 bitset<32> val(0xFFFFFFFF);
 string str( "00100000" );
 bitset<32> mytest( str );
 
 
 
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
    
 cout << abc << endl;
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
 
 
 return 0;
}

