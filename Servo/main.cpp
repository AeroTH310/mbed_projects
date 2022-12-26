#include "mbed.h"
#include "Servo.h"

DigitalOut myled(LED1);

int main()
{
Servo servo(p21);

    while(1)
    {
    wait(0.5);
        servo.position(-1);
        myled = 1;
        wait(0.2);
        myled = 0;
        wait(0.2);
        wait(0.5);
        servo.position(1);
    }
}