#include "stdint.h"
#include "mbed.h"

class Servo
{
    private:
        PwmOut SignalLine;

        float Period;
        float UpperRange;
        float Center;
        float LowerRange;
        float Position;

    public:
        Servo(PinName sgnl);
            //constructor sets common servo defaults
        void position(float ratio);
            //set normalized servo position (-1 to 1), default = 0
        float position();
            //get normalized servo position (-1 to 1)
        void maximum(float seconds);
        void maximum_ms(int milliseconds);
        void maximum_us(int microseconds);
            //set maximum deflection pulse width, default = 2400 us
        void center(float seconds);
        void center_ms(int milliseconds);
        void center_us(int microseconds);
            //set center deflection pulse width, default = 1500 us
        void minimum(float seconds);
        void minimum_ms(int milliseconds);
        void minimum_us(int microseconds);
            //set minimum deflection pulse width, default = 600 us
        void period(float seconds);
        void period_ms(int milliseconds);
        void period_us(int microseconds);
            //set the period, default = 20000 us
        void pulsewidth(float seconds);
        void pulsewidth_ms(int milliseconds);
        void pulsewidth_us(int microseconds);
            //set the pulse width, default = 1500 us
        float operator =(float assignment);
            //shorthand for position setting;
            //ex: "ServoObj = 0.5;" will set servo deflection to +0.5
        operator float();
            //shorthand for position reading;
            // ex: "float check = ServoObj;" will get the current servo deflection
};