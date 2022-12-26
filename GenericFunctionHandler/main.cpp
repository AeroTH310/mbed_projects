#include "mbed.h"
#include "TestClass.h"
#include "GenericFunctionHandler.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
extern "C" void testfunc1(void)
{
    while (1)
    {
        led1 = !led1;
        wait_ms(250);
        led2 = !led2;
        wait_ms(250);
        led3 = !led3;
        wait_ms(250);
        led4 = !led4;
        wait_ms(250);
    }
}

GenericFunctionHandler TestInst1(&testfunc1);
TestClass TestInst2;

int main()
{
        TestInst2.testMfunc2();
        TestInst1();
}