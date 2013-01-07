#include "SkGraphics.h"
#include "SkTypes.h"
#include "SkEvent.h"
#include "SkView.h"

class Test : SkView {
public:
    Test();
    virtual ~Test();
    void coding( void );
    
protected:
    int MyVar1;
    
private:
    int MyVar;
    int MyVar2;
};

