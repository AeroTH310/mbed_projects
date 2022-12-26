//mbed Microcontroller Library
//Software Interrupt Class for EINT1 Pin 2.11 designed to attach interrupt
//    handlers and call them in software-generated interrupts.
//Copyright 2010
//Thomas Hamilton

#ifndef SoftInterruptLibrary
#define SoftInterruptLibrary

#include "GenericFunctionHandler.h"
#include "CircularQueue.h"
#include "PinConnectBlock.h"

class SoftInterrupt
{
    private:
		static CircularQueue<SoftInterrupt*>* InterruptQueue;

        static void CallHandler();

        bool PendingStatus;

        GenericFunctionHandler InterruptHandler;

    public:
        SoftInterrupt(void (*HandlerAddress)());
        template<class Class>
            SoftInterrupt(Class* ObjectAddress, void (Class::*HandlerAddress)());

        ~SoftInterrupt();

        void TriggerInterrupt();
};

template<class Class>
    SoftInterrupt::SoftInterrupt(
        Class* ObjectAddress, void (Class::*HandlerAddress)())
        : PendingStatus(0), InterruptHandler(ObjectAddress, HandlerAddress)
{
    if (!QueueSize)
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

#endif