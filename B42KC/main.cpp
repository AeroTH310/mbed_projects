#include "mbed.h"
#include "B42KC.h"
    //we need to include the header file of the class we want to use, "mbed.h"
    //includes all of the headers of all of mbed's special hardware classes

DigitalOut Indicator(LED1);
    //this sets up the microcontroller's first led to
    //interface with a DigitalOut object named "Indicator"

B42KC Photosensor(p5, p6);
    //connect pin 5 to the signal output of the sensor, and pin 6 to the stability
    //output. WARNING: THE DATASHEET FOR THIS DEVICE INDICATES THE OUTPUT VOLTAGE
    //IS AROUND 24 V, THE mbed PIN LOGIC IS ONLY 3.3 V TOLERANT, YOU WILL FRY YOUR
    //MICROCONTROLLER IF YOU CONNECT THESE LINES DIRECTLY TO THE PINS.  to fix this
    //issue, use voltage regulators to step the voltage down from 24 V to 3.3 V

int main()  //this is the main function, it is the entry point of the
            //program, all activities start here and proceed downward
{
    while(1)    //this is an infinite loop, it will cycle through the following code forever
    {
        if (Photosensor.Read()) //if the photosensor signal returns "on"
        {
            Indicator.write(1); //then turn the microcontroller's indicator led on
        }
        else                    //otherwise
        {
            Indicator.write(0); //turn the indicator led off
        }
    }
}
    //the mbed API determines the functionality of its calsses like
    //"DigitalOut". documentation of this API can be found on the mbed website