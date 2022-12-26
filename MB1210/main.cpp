#include "mbed.h"
#include "MB1210.h"

DigitalOut debugled(LED1);
Serial Computer(USBTX, USBRX);

MB1210 RangeFinder(p12, p15, p13, p14);
float Range;
int main()
{
    Computer.baud(9600);
    debugled = 0;
    while(1)
    {
        debugled = !debugled;
        wait_ms(100);//wait for reading to be prepared
        //RangeFinder.Mode(0);//switch to PWM mode
        //Computer.printf("PWM reading: %f in | ", Range);
        //RangeFinder.Mode(1);//switch to Analog mode
        //Computer.printf("Analog reading: %f in | ", Range);
        //RangeFinder.Mode(2);//switch to serial mode
        Computer.printf("Serial reading: %f in | ", Range);
        wait(0.9);
    }
}