#include "TestSkia.h"
#include <iostream>

Test::Test() {
    
    std::cout << "Test class initialization\n";
}

Test::~Test() {
    
    std::cout << "Test class deinitialization\n";
}

void Test::coding( void ) {
    
    std::cout << "coding\n";
}

void SkEvent::SignalQueueTimer( SkMSec delay ) {}
void SkEvent::SignalNonEmptyQueue() {}

void application_init() {
    
    SkGraphics::Init();
    SkEvent::Init();
}

void application_term() {
    
    SkEvent::Term();
    SkGraphics::Term();
}

