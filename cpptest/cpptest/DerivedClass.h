//
//  DerivedClass.h
//  cpptest
//
//  Created by Merck Hung on 1/21/13.
//  Copyright (c) 2013 Merck Hung. All rights reserved.
//

#ifndef __cpptest__DerivedClass__
#define __cpptest__DerivedClass__

#include <iostream>
#include "TestClass.h"


class DerivedClass : public TestClass {

public:
    DerivedClass();

    DerivedClass( int x ): TestClass( 556 ) {
        
        std::cout << "DerivedClass::DerivedClass( " << x << " )\n";
    }

    virtual ~DerivedClass();
    virtual int CheckTest();
    void PerformTest( int abc );
};


#endif /* defined(__cpptest__DerivedClass__) */

