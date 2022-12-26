//mbed Microcontroller Library
//mbed Microcontroller Library
//Max Botix Ultrasonic Range Finder MB1210 Interface
//Copyright 2010
//Thomas Hamilton

#ifndef MB1210Library
#define MB1210Library

#include "mbed.h"
#include "PwmIn.h"

class MB1210
{
    private:
        PwmIn* PwmInput;
        AnalogIn* AnalogInput;
        DigitalOut* SerialOutput;
        Serial* SerialInput;

        char OperatingMode;
        float UnitFactor;
        float PwmScalingFactor;
        float AnalogScalingFactor;
        float Range;
        float* InterruptBuffer;

        void Interrupt();

    public:
        MB1210(PinName pw, PinName an, PinName tx, PinName rx);
            //pulse width modulation input, analog input, serial output, serial input;
            //specify NC if pin is not used;
            //if the pulse width pin is used, interrupts will perform a
            //few microseconds of calculations every time a reading is taken
        ~MB1210();
            //deallocates PwmInput, AnalogInput, SerialOutput, and SerailInput
        void SoundVelocity(float MetersPerSecond);
            //if, for some reason, you need to correct the speed of sound
            //for Pwm modes, enter the new value here in meters per second;
            //default is 340.29 m/s
            //Note: most accurate mode
        void Voltage(float Volts);
            //sets expected operating voltage for Analog modes;
            //user responsibility to ensure operating voltage between 3.3 V and 5 V;
            //default is 3.3 V
        void Unit(float UnitsPerMeter);
            //argument sets the putput units through multiplication;
            //default is cm
        void Mode(char Selection);
            //argument sets operating mode;
            //set flag 0x08 to 0 for polled modes, or 1 for interrupt modes;
            //set flag 0x04 to 0 for synchronous modes, or 1 for asynchronous modes;
            //set flags 0x03 to 0 for pwm, 1 for analog, or 2 for serial;
            //asynchronous modes generate pulse width interrupts, prepare an
            //analog reading, and send a 5 byte serial reading every 99 ms;
            //interrupt modes automatically read the range into the buffer provided
            //by AttachInterruptBuffer with the selected input method every 99 ms
            //if in an asynchronous mode, or 99 ms after RequestSyncRead is called;
            //the rx pin must be connected to use an interrupt mode;
            //the tx pin must be connected to use a synchronous mode;
            //default is 0
        void AttachInterruptBuffer(float* Buffer);
            //if interrupts are used, user must provide address to write result to
        void RequestSyncRead();
            //this tells the device to prepare a synchronous range reading;
            //must be called at least 99 ms before the reading is needed;
            //changes asynchronous mode to synchronous equivalent
        void DiscardSerialBuffer();
            //the serial port has a buffer and only the oldest data is read from it;
            //the buffer has limited space and, when full, the newest data is discarded;
            //this method allows the user to empty the buffer of old data so new data is used
        float Read();
            //get a reading from the device in the set mode;
            //RequestSyncRead() must be called at least 99 ms
            //before this method can be called in a synchronous mode;
            //may be called at any time during asynchronous mode;
        operator float();
            //shorthand for taking a range reading;
            //ex: "float reading = MB1210Object;"
};

#endif