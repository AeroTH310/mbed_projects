#include "GenericFunctionHandler.h"
#include "mbed.h"

class TestClass
{
private:
    GenericFunctionHandler instance;

public:
    TestClass(void) : instance(this, &TestClass::testMfunc1), myComputer(USBTX,USBRX)
    {
    }
    void testMfunc1(void)
    {
        myComputer.printf("it worked!!! ");
    }
    void testMfunc2(void)
    {
        instance();
    }
    Serial myComputer;
};