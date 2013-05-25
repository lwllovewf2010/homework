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

using namespace std;

int main() {
    
    int cc = 1234;
    int &abc = cc;
    bitset<32> val(0xFFFFFFFF);
    string str( "00100000" );
    bitset<32> mytest( str );
    
    vector<string> v1( 8, "Test" );
    string text = "Merck";
    
    TestClass *dc = new TestClass();
    
    dc->PerformTest( 456 );
    dc->CheckTest();
    dc->TestConst();
    
    delete dc;
    
    dc = new DerivedClass( 45 );
    dc->PerformTest( 890 );
    dc->CheckTest();
    
    delete dc;
    
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
    

    
    return 0;
}

