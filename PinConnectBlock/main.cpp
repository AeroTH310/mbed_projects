#include "mbed.h"
#include "PinConnectBlock.h"

Serial myComputer(USBTX, USBRX);
void interruptHandler1(void)
{   myComputer.printf("Testing Pin 1 |"); wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-");
    wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-");
    wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-"); wait(1); myComputer.printf("|");     }
void interruptHandler2(void)
{   myComputer.printf("Testing Pin 2 |"); wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-");
    wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-");
    wait(1); myComputer.printf("-"); wait(1); myComputer.printf("-"); wait(1); myComputer.printf("|");     }
void checkFunctRegisters()
{
    for (int i = 0; i < 10; i++)
    {
        myComputer.printf("\r\nPinselReg %d:\r\n\t", i);
        for (int j = 0; j < 4; j++)
        {
            myComputer.printf("%X ", *((char*)((unsigned long*)&LPC_PINCON->PINSEL0 + i) + j));
        }
    }
}
void checkResistRegisters()
{
    for (int i = 0; i < 10; i++)
    {
        myComputer.printf("\r\nPinmodReg %d:\r\n\t", i);
        for (int j = 0; j < 4; j++)
        {
            myComputer.printf("%X ", *((char*)((unsigned long*)&LPC_PINCON->PINMODE0 + i) + j));
        }
    }
}
void checkODMRegisters()
{
    for (int i = 0; i < 5; i++)
    {
        myComputer.printf("\r\nPinmodODReg %d:\r\n\t", i);
        for (int j = 0; j < 4; j++)
        {
            myComputer.printf("%X ", *((char*)((unsigned long*)&LPC_PINCON->PINMODE_OD0 + i) + j));
        }
    }
}
void checkTMRegister()
{
    myComputer.printf("\r\nPinselTMReg:\r\n\t");
    for (int j = 0; j < 4; j++)
    {
        myComputer.printf("%X ", *((char*)&LPC_PINCON->PINSEL10 + j));
    }
}
void checkI2CRegister()
{
    myComputer.printf("\r\nI2CPADReg:\r\n\t");
    for (int j = 0; j < 4; j++)
    {
        myComputer.printf("%X ", *((char*)&LPC_PINCON->I2CPADCFG + j));
    }
}

int main ()
{
    PinConnectBlock::SetPinConnection(2, 11, 0x01);
    NVIC_SetVector(EINT1_IRQn, (uint32_t)interruptHandler1);
    NVIC_SetPriority(EINT1_IRQn, 1);
    NVIC_EnableIRQ(EINT1_IRQn);

    /*InterruptIn hardint(p30);
    hardint.rise(&interruptHandler2);
    NVIC_SetPriority(EINT3_IRQn, 0);*/

    /*unsigned char result;
    for (int i = 1; i < 7; i++)
    {
        myComputer.printf("Port %d:\r\n", i);
        for (int j = 0; j < 35; j++)
        {
            myComputer.printf("\tPin %d:\r\n", j);
            for (int k = 0; k < 4; k++)
            {
                result = PinConnectBlock::ValidateInput(i, j, k);
                myComputer.printf("\t\tMode %d: %d ", k, result);
                result = PinConnectBlock::ValidateInput(i, j, k | 0x0C);
                myComputer.printf("Mode %d: %d ", k | 0x0C, result);
                result = PinConnectBlock::ValidateInput(i, j, k | 0x10);
                myComputer.printf("Mode %d: %d \r\n", k | 0x10, result);
                result = PinConnectBlock::ValidateInput(i, j, k | 0x14);
                myComputer.printf("Mode %d: %d \r\n", k | 0x14, result);
            }
        }
    }*/

    /*PinConnectBlock::SetPinConnection(2, 11, 0x13);
    checkFunctRegisters();
    checkResistRegisters();
    checkODMRegisters();
    myComputer.printf("\r\nPin 2.11 Mode: %X\r\n\r\n", PinConnectBlock::GetPinConnection(2, 11));
    PinConnectBlock::SetPinConnection(2, 11, 0x0C);
    checkFunctRegisters();
    checkResistRegisters();
    checkODMRegisters();
    myComputer.printf("\r\nPin 2.11 Mode: %X\r\n\r\n", PinConnectBlock::GetPinConnection(2, 11));
    PinConnectBlock::SetPinConnection(2, 11, 0x1F);
    checkFunctRegisters();
    checkResistRegisters();
    checkODMRegisters();
    myComputer.printf("\r\nPin 2.11 Mode: %X\r\n\r\n", PinConnectBlock::GetPinConnection(2, 11));
    PinConnectBlock::SetPinConnection(2, 11, 0x00);
    checkFunctRegisters();
    checkResistRegisters();
    checkODMRegisters();
    myComputer.printf("\r\nPin 2.11 Mode: %X\r\n\r\n", PinConnectBlock::GetPinConnection(2, 11));
    PinConnectBlock::SetPinConnection(2, 11, 0x01);
    checkFunctRegisters();
    checkResistRegisters();
    checkODMRegisters();
    myComputer.printf("\r\nPin 2.11 Mode: %X\r\n\r\n", PinConnectBlock::GetPinConnection(2, 11));
    PinConnectBlock::SetPinConnection(1, 10, 0x02);
    PinConnectBlock::SetPinConnection(0, 28, 0x1F);
    PinConnectBlock::SetPinConnection(0, 29, 0x1F);
    checkFunctRegisters();
    checkResistRegisters();
    checkODMRegisters();*/

    /*PinConnectBlock::SetFunctionMode(1, 18, 0x02);
    PinConnectBlock::SetFunctionMode(1, 20, 0x03);
    checkFunctRegisters();
    myComputer.printf("\r\nPin 3.25 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(4, 29));
    myComputer.printf("\r\nPin 3.25 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(3, 25));
    myComputer.printf("\r\nPin 2.11 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(2, 11));
    myComputer.printf("\r\nPin 1.18 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(1, 18));
    myComputer.printf("\r\nPin 1.20 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(1, 20));
    myComputer.printf("\r\nPin 1.0 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(1, 0));
    myComputer.printf("\r\nPin 0.15 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(0, 15));
    myComputer.printf("\r\nPin 0.25 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(0, 25));
    PinConnectBlock::SetFunctionMode(4, 29, 0x01);
    PinConnectBlock::SetFunctionMode(3, 25, 0x02);
    PinConnectBlock::SetFunctionMode(2, 11, 0x01);
    PinConnectBlock::SetFunctionMode(1, 0, 0x01);
    PinConnectBlock::SetFunctionMode(0, 15, 0x02);
    PinConnectBlock::SetFunctionMode(0, 25, 0x03);
    PinConnectBlock::SetFunctionMode(1, 18, 0x00);
    PinConnectBlock::SetFunctionMode(1, 20, 0x00);
    checkFunctRegisters();
    myComputer.printf("\r\nPin 3.25 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(4, 29));
    myComputer.printf("\r\nPin 3.25 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(3, 25));
    myComputer.printf("\r\nPin 2.11 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(2, 11));
    myComputer.printf("\r\nPin 1.18 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(1, 18));
    myComputer.printf("\r\nPin 1.20 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(1, 20));
    myComputer.printf("\r\nPin 1.0 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(1, 0));
    myComputer.printf("\r\nPin 0.15 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(0, 15));
    myComputer.printf("\r\nPin 0.25 Fmode: %d\r\n\r\n", PinConnectBlock::GetFunctionMode(0, 25));*/

    /*PinConnectBlock::SetResistorMode(1, 18, 0x00);
    PinConnectBlock::SetResistorMode(1, 20, 0x03);
    checkResistRegisters();
    myComputer.printf("\r\nPin 0.0 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(0, 0));
    myComputer.printf("\r\nPin 0.15 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(0, 15));
    myComputer.printf("\r\nPin 1.18 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(1, 18));
    myComputer.printf("\r\nPin 1.20 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(1, 20));
    myComputer.printf("\r\nPin 1.31 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(1, 31));
    myComputer.printf("\r\nPin 2.0 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(2, 0));
    myComputer.printf("\r\nPin 3.26 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(3, 26));
    myComputer.printf("\r\nPin 4.28 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(4, 28));
    PinConnectBlock::SetResistorMode(0, 0, 0x01);
    PinConnectBlock::SetResistorMode(0, 15, 0x02);
    PinConnectBlock::SetResistorMode(1, 18, 0x03);
    PinConnectBlock::SetResistorMode(1, 20, 0x00);
    PinConnectBlock::SetResistorMode(1, 31, 0x01);
    PinConnectBlock::SetResistorMode(2, 0, 0x02);
    PinConnectBlock::SetResistorMode(3, 26, 0x03);
    PinConnectBlock::SetResistorMode(4, 28, 0x01);
    checkResistRegisters();
    myComputer.printf("\r\nPin 0.0 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(0, 0));
    myComputer.printf("\r\nPin 0.15 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(0, 15));
    myComputer.printf("\r\nPin 1.18 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(1, 18));
    myComputer.printf("\r\nPin 1.20 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(1, 20));
    myComputer.printf("\r\nPin 1.31 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(1, 31));
    myComputer.printf("\r\nPin 2.0 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(2, 0));
    myComputer.printf("\r\nPin 3.26 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(3, 26));
    myComputer.printf("\r\nPin 4.28 Rmode: %d\r\n\r\n", PinConnectBlock::GetResistorMode(4, 28));*/

    /*PinConnectBlock::SetOpenDrainMode(3, 26, 1);
    PinConnectBlock::SetOpenDrainMode(3, 25, 1);
    checkODMRegisters();
    myComputer.printf("\r\nPin 0.0 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(0, 0));
    myComputer.printf("\r\nPin 0.11 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(0, 11));
    myComputer.printf("\r\nPin 0.15 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(0, 15));
    myComputer.printf("\r\nPin 0.26 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(0, 26));
    myComputer.printf("\r\nPin 0.29 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(0, 29));
    myComputer.printf("\r\nPin 0.30 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(0, 30));
    myComputer.printf("\r\nPin 1.0 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(1, 0));
    myComputer.printf("\r\nPin 1.31 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(1, 31));
    myComputer.printf("\r\nPin 2.0 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(2, 0));
    myComputer.printf("\r\nPin 2.13 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(2, 13));
    myComputer.printf("\r\nPin 3.25 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(3, 25));
    myComputer.printf("\r\nPin 3.26 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(3, 26));
    myComputer.printf("\r\nPin 4.28 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(4, 28));
    myComputer.printf("\r\nPin 4.29 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(4, 29));
    PinConnectBlock::SetOpenDrainMode(0, 0, 1);
    PinConnectBlock::SetOpenDrainMode(0, 11, 1);
    PinConnectBlock::SetOpenDrainMode(0, 15, 1);
    PinConnectBlock::SetOpenDrainMode(0, 26, 1);
    PinConnectBlock::SetOpenDrainMode(0, 29, 1);
    PinConnectBlock::SetOpenDrainMode(0, 30, 1);
    PinConnectBlock::SetOpenDrainMode(1, 0, 1);
    PinConnectBlock::SetOpenDrainMode(1, 31, 1);
    PinConnectBlock::SetOpenDrainMode(2, 0, 1);
    PinConnectBlock::SetOpenDrainMode(2, 13, 1);
    PinConnectBlock::SetOpenDrainMode(3, 25, 0);
    PinConnectBlock::SetOpenDrainMode(3, 26, 0);
    PinConnectBlock::SetOpenDrainMode(4, 28, 1);
    PinConnectBlock::SetOpenDrainMode(4, 29, 1);
    checkODMRegisters();
    myComputer.printf("\r\nPin 0.0 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(0, 0));
    myComputer.printf("\r\nPin 0.11 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(0, 11));
    myComputer.printf("\r\nPin 0.15 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(0, 15));
    myComputer.printf("\r\nPin 0.26 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(0, 26));
    myComputer.printf("\r\nPin 0.29 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(0, 29));
    myComputer.printf("\r\nPin 0.30 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(0, 30));
    myComputer.printf("\r\nPin 1.0 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(1, 0));
    myComputer.printf("\r\nPin 1.31 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(1, 31));
    myComputer.printf("\r\nPin 2.0 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(2, 0));
    myComputer.printf("\r\nPin 2.13 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(2, 13));
    myComputer.printf("\r\nPin 3.25 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(3, 25));
    myComputer.printf("\r\nPin 3.26 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(3, 26));
    myComputer.printf("\r\nPin 4.28 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(4, 28));
    myComputer.printf("\r\nPin 4.29 ODmode: %d\r\n\r\n", PinConnectBlock::GetOpenDrainMode(4, 29));*/

    /*checkTMRegister();
    myComputer.printf("\r\nTrace mode: %d\r\n\r\n", PinConnectBlock::GetTraceMode());
    PinConnectBlock::SetTraceMode(1);
    checkTMRegister();
    myComputer.printf("\r\nTrace mode: %d\r\n\r\n", PinConnectBlock::GetTraceMode());
    PinConnectBlock::SetTraceMode(0);
    checkTMRegister();
    myComputer.printf("\r\nTrace mode: %d\r\n\r\n", PinConnectBlock::GetTraceMode());*/

    /*checkI2CRegister();
    myComputer.printf("\r\nI2C0mode: %d\r\n\r\n", PinConnectBlock::GetI2C0Mode());
    PinConnectBlock::SetI2C0Mode(0x03);
    checkI2CRegister();
    myComputer.printf("\r\nI2C0mode: %d\r\n\r\n", PinConnectBlock::GetI2C0Mode());
    PinConnectBlock::SetI2C0Mode(0x01);
    checkI2CRegister();
    myComputer.printf("\r\nI2C0mode: %d\r\n\r\n", PinConnectBlock::GetI2C0Mode());
    PinConnectBlock::SetI2C0Mode(0x02);
    checkI2CRegister();
    myComputer.printf("\r\nI2C0mode: %d\r\n\r\n", PinConnectBlock::GetI2C0Mode());
    PinConnectBlock::SetI2C0Mode(0x00);
    checkI2CRegister();
    myComputer.printf("\r\nI2C0mode: %d\r\n\r\n", PinConnectBlock::GetI2C0Mode());
    LPC_PINCON->I2CPADCFG |= 0x00000001;
    checkI2CRegister();
    myComputer.printf("\r\nI2C0mode: %d\r\n\r\n", PinConnectBlock::GetI2C0Mode());
    LPC_PINCON->I2CPADCFG |= 0x00000003;
    checkI2CRegister();
    myComputer.printf("\r\nI2C0mode: %d\r\n\r\n", PinConnectBlock::GetI2C0Mode());
    LPC_PINCON->I2CPADCFG |= 0x00000007;
    checkI2CRegister();
    myComputer.printf("\r\nI2C0mode: %d\r\n\r\n", PinConnectBlock::GetI2C0Mode());
    LPC_PINCON->I2CPADCFG &= ~0x0000000F;
    LPC_PINCON->I2CPADCFG |= 0x00000008;
    checkI2CRegister();
    myComputer.printf("\r\nI2C0mode: %d\r\n\r\n", PinConnectBlock::GetI2C0Mode());
    LPC_PINCON->I2CPADCFG |= 0x0000000C;
    checkI2CRegister();
    myComputer.printf("\r\nI2C0mode: %d\r\n\r\n", PinConnectBlock::GetI2C0Mode());
    LPC_PINCON->I2CPADCFG |= 0x0000000E;
    checkI2CRegister();
    myComputer.printf("\r\nI2C0mode: %d\r\n\r\n", PinConnectBlock::GetI2C0Mode());*/

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

        NVIC_SetPendingIRQ(EINT1_IRQn);
        wait(10);
    }
}