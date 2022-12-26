#include "mbed.h"
#include "PinConnectBlock.h"
//#include "TestClass.h"
//#include "GenericFunctionHandler.h"

Serial myComputer(USBTX,USBRX);
extern "C" void interruptHandler1(void)
{   myComputer.printf("Testing Pin 1 |");
    wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-");
    wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-");
    NVIC_ClearPendingIRQ(EINT1_IRQn);
    wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-");
    wait(1); myComputer.printf("-"); wait(1); myComputer.printf("|");
}
extern "C" void interruptHandler2(void)
{
    myComputer.printf("Testing Pin 2 |");
    wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-");
    wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-");
    wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-");
    wait(1); myComputer.printf("-"); wait(1); myComputer.printf("|");
}

//TestClass TestInst;
//GenericFunctionHandler TestInst2(&interruptHandler2);

int main()
{
    PinConnectBlock::SetPinConnection(2, 11, 0x01);
    NVIC_SetVector(EINT1_IRQn, (unsigned long)&interruptHandler1);
    NVIC_SetPriority(EINT1_IRQn, 1);
    NVIC_EnableIRQ(EINT1_IRQn);

    NVIC_SetPendingIRQ(EINT1_IRQn);

    DigitalOut led1(LED1);
    DigitalOut led2(LED2);
    DigitalOut led3(LED3);
    DigitalOut led4(LED4);
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
        //TestInst.testfunc2();
        //TestInst2();
    }
}