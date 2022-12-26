#include "SoftInterrupt.h"

CircularQueue<SoftInterrupt*>* SoftInterrupt::InterruptQueue = 0;

SoftInterrupt::SoftInterrupt(void (*HandlerAddress)())
    : PendingStatus(0), InterruptHandler(HandlerAddress)
{
    if (!InterruptQueue)
    {
        PinConnectBlock::SetPinConnection(2, 11, 0x01);
        NVIC_SetVector(EINT1_IRQn, (uint32_t)&CallHandler);
        NVIC_EnableIRQ(EINT1_IRQn);
        InterruptQueue = new CircularQueue<SoftInterrupt*>(1);
    }
    else
    {
        InterruptQueue->IncrementSize();
    }
}

SoftInterrupt::~SoftInterrupt()
{
    if(InterruptQueue->GetSize() == 1)
    {
        PinConnectBlock::SetPinConnection(2, 11, 0x00);
        NVIC_SetVector(EINT1_IRQn, 0x00000000);
        NVIC_DisableIRQ(EINT1_IRQn);
        delete InterruptQueue;
        InterruptQueue = 0;
    }
    else
    {
        if (PendingStatus)
        {
            InterruptQueue->Remove(this);
        }
        InterruptQueue->DecrementSize();
    }
}

void SoftInterrupt::TriggerInterrupt()
{
    if (!PendingStatus)
    {
		InterruptQueue->Add(this);
        PendingStatus = 1;
    }
    NVIC_SetPendingIRQ(EINT1_IRQn);
}

void SoftInterrupt::CallHandler()
{
    SoftInterrupt* CurrentPointer;

    while (InterruptQueue->GetCount())
    {
		CurrentPointer = InterruptQueue->GetNext();
        CurrentPointer->PendingStatus = 0;
        CurrentPointer->InterruptHandler.Call();
    }
}