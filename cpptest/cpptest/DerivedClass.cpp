//
//  DerivedClass.cpp
//  cpptest
//
//  Created by Merck Hung on 1/21/13.
//  Copyright (c) 2013 Merck Hung. All rights reserved.
//

#include "DerivedClass.h"


DerivedClass::DerivedClass() {

    std::cout << "DerivedClass()\n";
}


DerivedClass::~DerivedClass() {

    std::cout << "~DerivedClass()\n";
}


int DerivedClass::CheckTest() {

    std::cout << "DerivedClass::CheckTest()\n";
    return 1;
}


void DerivedClass::PerformTest( int abc ) {

    std::cout << "DerivedClass::PerformTest( " << abc << " )\n";
}

