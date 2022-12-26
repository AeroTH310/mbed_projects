#include "GenericFunctionHandler.h"
#include "mbed.h"

class TestClass
{
private:
    GenericFunctionHandler instance;

public:
    TestClass(void) : instance(this, &TestClass::testfunc1), myComputer(USBTX,USBRX)
    {
    }
    void testfunc1(void)
    {
        myComputer.printf("it worked!!! ");
    }
    void testfunc2(void)
    {
        instance();
    }
    Serial myComputer;
};