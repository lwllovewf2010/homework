//
//  TestTemp.h
//  temp_test
//
//  Created by Merck Hung on 2/26/15.
//  Copyright (c) 2015 Merck Hung. All rights reserved.
//

#ifndef __temp_test__TestTemp__
#define __temp_test__TestTemp__

#include <iostream>

template
<typename T>
class TestTemp {
 public:
  TestTemp(T arg)
    : inner_(arg) {
  }

  ~TestTemp() {
    inner_ = 98;
  }

  T GetInner() const {
    return inner_;
  }

 private:
  T inner_;
};

#endif /* defined(__temp_test__TestTemp__) */
