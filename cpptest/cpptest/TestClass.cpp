//
//  TestClass.cpp
//  cpptest
//
//  Created by Merck Hung on 1/21/13.
//  Copyright (c) 2013 Merck Hung. All rights reserved.
//

#include "TestClass.h"


TestClass::TestClass() {

    std::cout << "TestClass()\n";
    this->pri_var = 1;
    this->pro_var = 2;
    this->pub_var = 3;
}


TestClass::~TestClass() {

    std::cout << "~TestClass()\n";
}


void TestClass::PerformTest( int abc ) {

    std::cout << "TestClass::PerformTest( " << abc << " )\n";
}


int TestClass::CheckTest() {
    
    std::cout << "TestClass::CheckTest()\n";
    return 1;
}


void TestClass::TestConst() const {

    std::cout << "TestClass::TestConst()\n";
}

