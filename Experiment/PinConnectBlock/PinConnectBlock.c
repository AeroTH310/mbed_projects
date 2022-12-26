#include "PinConnectBlock.h"

unsigned char PinConnectBlock::ValidateInput(
    unsigned char Port, unsigned char Pin, unsigned char Mode)
{
    switch (Port)
    {
        case 0:
            if (Pin < 2)
            {
                return 0x00;
            }
            if ((Pin > 3) && (Pin < 12))
            {
                return 0x00;
            }
            if ((Pin > 14) && (Pin < 19))
            {
                return 0x00;
            }
            if ((Pin > 22) && (Pin < 27))
            {
                return 0x00;
            }
            if ((Mode & 0x03) != 0x03)
            {
                if (Pin < 4)
                {
                    return 0x00;
                }
                if ((Pin > 26) && (Pin < 29) && !(Mode & 0x1C))
                {
                    return 0x00;
                }
            }
            if ((Pin > 18) && (Pin < 23) && ((Mode & 0x03) != 0x02))
            {
                return 0x00;
            }
            if ((Pin > 28) && (Pin < 31) && !(Mode & 0x0E))
            {
                return 0x00;
            }
            if (Pin < 4)
            {
                return 0x0C;
            }
            if (Pin < 15)
            {
                return 0x02 | ((Mode & 0x03) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00)
                    | ((Mode & 0x10) ? 0x24 : 0x00);
            }
            if (Pin < 23)
            {
                return 0x0C;
            }
            if (Pin < 29)
            {
                return (((Mode & 0x03) == 0x03) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00)
                    | ((Mode & 0x10) ? 0x24 : 0x00);
            }
            if (Pin < 31)
            {
                return ((Mode & 0x02) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00);
            }
            if (Pin > 30)
            {
                return 0x02 | ((Mode & 0x03) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00)
                    | ((Mode & 0x10) ? 0x24 : 0x00);
            }
            break;
        case 1:
            if ((Pin > 17) && (Pin < 25))
            {
                return 0x00;
            }
            if ((Pin > 25) && (Pin < 30))
            {
                return 0x00;
            }
            if (!(Mode & 0x02))
            {
                if (Pin < 2)
                {
                    return 0x00;
                }
                if (Pin == 4)
                {
                    return 0x00;
                }
                if ((Pin > 7) && (Pin < 11))
                {
                    return 0x00;
                }
                if ((Pin > 13) && (Pin < 18))
                {
                    return 0x00;
                }
            }
            if ((Pin == 25) && ((Mode & 0x03) != 0x02))
            {
                return 0x00;
            }
            if ((Pin > 29) && (Pin < 32) && ((Mode & 0x03) != 0x01))
            {
                return 0x00;
            }
            if (Pin < 2)
            {
                return 0x0C;
            }
            if (Pin < 4)
            {
                return 0x02 | ((Mode & 0x03) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00)
                    | ((Mode & 0x10) ? 0x24 : 0x00);
            }
            if (Pin == 4)
            {
                return 0x0C;
            }
            if (Pin < 8)
            {
                return 0x02 | ((Mode & 0x03) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00)
                    | ((Mode & 0x10) ? 0x24 : 0x00);
            }
            if (Pin < 11)
            {
                return 0x0C;
            }
            if (Pin < 14)
            {
                return 0x02 | ((Mode & 0x03) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00)
                    | ((Mode & 0x10) ? 0x24 : 0x00);
            }
            if (Pin < 18)
            {
                return 0x0C;
            }
            if ((Pin == 25) && ((Mode & 0x03) == 0x02))
            {
                return 0x0C;
            }
            if ((Pin > 29) && (Pin < 32) && ((Mode & 0x03) == 0x01))
            {
                return 0x0C;
            }
            if (Pin > 31)
            {
                return 0x02 | ((Mode & 0x03) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00)
                    | ((Mode & 0x10) ? 0x24 : 0x00);
            }
            break;
        case 2:
            if ((Pin > 7) && (Pin < 10))
            {
                return 0x00;
            }
            if ((Pin < 11) && ((Mode & 0x03) != 0x03))
            {
                return 0x00;
            }
            if ((Pin > 10) && (Pin < 14) && ((Mode & 0x03) != 0x02))
            {
                return 0x00;
            }
            if (Pin < 14)
            {
                return 0x0C;
            }
            if (Pin > 13)
            {
                return 0x02 | ((Mode & 0x03) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00)
                    | ((Mode & 0x10) ? 0x24 : 0x00);
            }
            break;
        case 3:
            if (Pin == 26)
            {
                return 0x00;
            }
            if ((Pin == 25) && ((Mode & 0x03) != 0x01))
            {
                return 0x00;
            }
            if (Pin < 25)
            {
                return 0x02 | ((Mode & 0x03) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00)
                    | ((Mode & 0x10) ? 0x24 : 0x00);
            }
            if (Pin == 25)
            {
                return 0x0C;
            }
            if (Pin > 26)
            {
                return 0x02 | ((Mode & 0x03) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00)
                    | ((Mode & 0x10) ? 0x24 : 0x00);
            }
            break;
        case 4:
            if ((Pin > 27) && (Pin < 30))
            {
                return 0x00;
            }
            if (Pin < 28)
            {
                return 0x02 | ((Mode & 0x03) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00)
                    | ((Mode & 0x10) ? 0x24 : 0x00);
            }
            if (Pin > 29)
            {
                return 0x02 | ((Mode & 0x03) ? 0x0C : 0x00)
                    | ((Mode & 0x0C) ? 0x14 : 0x00)
                    | ((Mode & 0x10) ? 0x24 : 0x00);
            }
            break;
        default:
            return 0x01 | ((Pin > 31) ? 0x02 : 0x00)
                | ((Mode & 0x03) ? 0x0C : 0x00)
                | ((Mode & 0x0C) ? 0x14 : 0x00)
                | ((Mode & 0x10) ? 0x24 : 0x00);
    }
    return 0xFF;
}

void PinConnectBlock::SetPinConnection(
    unsigned char Port, unsigned char Pin, unsigned char Mode)
{
    if (!((Port == 1) && (Pin < 18) && (Mode & 0x02)))
    {
        SetFunctionMode(Port, Pin, Mode & 0x03);
    }
    if (!((Port == 0) && (Pin > 26)))
    {
        SetResistorMode(Port, Pin, (Mode & 0x0C) >> 2);
    }
    if (!((Port == 0) && (Pin > 26) && (Pin < 29)))
    {
        SetOpenDrainMode(Port, Pin, Mode & 0x10);
    }
    return;
}

unsigned char PinConnectBlock::GetPinConnection(
    unsigned char Port, unsigned char Pin)
{
    return GetFunctionMode(Port, Pin)
        | (GetResistorMode(Port, Pin) << 2)
        | (GetOpenDrainMode(Port, Pin) ? 0x10 : 0x00);
}

void PinConnectBlock::SetFunctionMode(
    unsigned char Port, unsigned char Pin, unsigned char Mode)
{
    Port *= 2;
    if (Pin >= 16)
    {
        Port++;
        Pin -= 16;
    }
    Pin *= 2;
    *((unsigned long*)&LPC_PINCON->PINSEL0 + Port)
        &= ~(0x00000003 << Pin);
    *((unsigned long*)&LPC_PINCON->PINSEL0 + Port)
        |= (unsigned long)Mode << Pin;
}

unsigned char PinConnectBlock::GetFunctionMode(
    unsigned char Port, unsigned char Pin)
{
    Port *= 2;
    if (Pin >= 16)
    {
        Port++;
        Pin -= 16;
    }
    Pin *= 2;
    return (*((unsigned long*)&LPC_PINCON->PINSEL0 + Port)
        & (0x00000003 << Pin)) >> Pin;
}

void PinConnectBlock::SetResistorMode(
    unsigned char Port, unsigned char Pin, unsigned char Mode)
{
    Port *= 2;
    if (Pin >= 16)
    {
        Port++;
        Pin -= 16;
    }
    Pin *= 2;
    *((unsigned long*)&LPC_PINCON->PINMODE0 + Port)
        &= ~(0x00000003 << Pin);
    *((unsigned long*)&LPC_PINCON->PINMODE0 + Port)
        |= (unsigned long)Mode << Pin;
    return;
}

unsigned char PinConnectBlock::GetResistorMode(
    unsigned char Port, unsigned char Pin)
{
    Port *= 2;
    if (Pin >= 16)
    {
        Port++;
        Pin -= 16;
    }
    Pin *= 2;
    return (*((unsigned long*)&LPC_PINCON->PINMODE0 + Port)
        & (0x00000003 << Pin)) >> Pin;
}

void PinConnectBlock::SetOpenDrainMode(
    unsigned char Port, unsigned char Pin, bool Mode)
{
    if (Mode)
    {
        *((unsigned long*)&LPC_PINCON->PINMODE_OD0 + Port)
            |= 0x00000001 << Pin;
    }
    else
    {
        *((unsigned long*)&LPC_PINCON->PINMODE_OD0 + Port)
            &= ~(0x00000001 << Pin);
    }
    return;
}

bool PinConnectBlock::GetOpenDrainMode(unsigned char Port, unsigned char Pin)
{
    return (*((unsigned long*)&LPC_PINCON->PINMODE_OD0 + Port)
        & (0x00000001 << Pin)) >> Pin;
}

void PinConnectBlock::SetTraceMode(bool Mode)
{
    if (Mode)
    {
        LPC_PINCON->PINSEL10 |= 0x00000008;
    }
    else
    {
        LPC_PINCON->PINSEL10 &= ~0x00000008;
    }
    return;
}

bool PinConnectBlock::GetTraceMode()
{
    return (LPC_PINCON->PINSEL10 & 0x00000008) != 0x00000000;
}

void PinConnectBlock::SetI2C0Mode(unsigned char Mode)
{

    if (!(Mode & 0x01))
    {
        LPC_PINCON->I2CPADCFG |= 0x0000000A;
    }
    else
    {
        LPC_PINCON->I2CPADCFG &= ~0x0000000A;
    }
    if (Mode & 0x02)
    {
        LPC_PINCON->I2CPADCFG |= 0x00000005;
    }
    else
    {
        LPC_PINCON->I2CPADCFG &= ~0x00000005;
    }
    return;
}

unsigned char PinConnectBlock::GetI2C0Mode()
{
    unsigned char Result = 0x00;

    if (!(LPC_PINCON->I2CPADCFG & 0x0000000A))
    {
        Result |= 0x01;
    }
    else if (!(LPC_PINCON->I2CPADCFG & 0x00000002))
    {
        Result |= 0x04;
    }
    else if (!(LPC_PINCON->I2CPADCFG & 0x00000008))
    {
        Result |= 0x08;
    }
    if ((LPC_PINCON->I2CPADCFG & 0x00000005) == 0x00000005)
    {
        Result |= 0x02;
    }
    else if (LPC_PINCON->I2CPADCFG & 0x00000001)
    {
        Result |= 0x10;
    }
    else if (LPC_PINCON->I2CPADCFG & 0x00000004)
    {
        Result |= 0x20;
    }
    return Result;
}