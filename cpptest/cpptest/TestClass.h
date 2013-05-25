//
//  TestClass.h
//  cpptest
//
//  Created by Merck Hung on 1/21/13.
//  Copyright (c) 2013 Merck Hung. All rights reserved.
//

#ifndef __cpptest__TestClass__
#define __cpptest__TestClass__

#include <iostream>


class TestClass {

public:
    TestClass();
    TestClass( int zyb ) {
    
        std::cout << "TestClass::TestClass( " << zyb << " )\n";
    };
    virtual ~TestClass();
    virtual void PerformTest( int abc );
    virtual int CheckTest();
    
    void TestConst() const;
    
    int pub_var;
    
protected:
    int pro_var;

private:
    int pri_var;
};


#endif /* defined(__cpptest__TestClass__) */

