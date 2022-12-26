#include "Servo.h"

Servo::Servo(PinName sgnl) : SignalLine(sgnl), Period(0.02),
    UpperRange(0.045), Center(0.075), LowerRange(-0.045), Position(0)
{
    SignalLine.period(0.02);
        //set period to most common value;
    SignalLine.write(0.075);
        //set position to most common center
}

void Servo::position(float ratio) 
{
    Position = ratio;
    if (ratio > 1)
    {
        ratio = 1;
    }
    else if (ratio < -1)
    {
        ratio = -1;
    }
        //saturate position to the range boundaries
    if (ratio >= 0)
    {
        SignalLine.write((ratio * UpperRange) + Center);
    }
    else 
    {
        SignalLine.write((ratio * LowerRange) + Center);
    }
        //set normalized position (-1 to 1)
}

float Servo::position()
{
    float ratio = SignalLine.read() - Center;
    if (ratio > UpperRange)
    {
        ratio = UpperRange;
    }
    else if (ratio < LowerRange)
    {
        ratio = LowerRange;
    }
        //saturate position to the range boundaries
    if(ratio >= 0)
    {
        return ratio / UpperRange;
    }
    else 
    {
        return ratio / LowerRange;
    }
        //return normalized position
}

void Servo::maximum(float seconds) 
{
    UpperRange = (seconds / Period) - Center;
    position(Position);
}

void Servo::maximum_ms(int milliseconds) 
{
    UpperRange = ((milliseconds / 1000) / Period) - Center;
    position(Position);
}

void Servo::maximum_us(int microseconds) 
{
    UpperRange = ((microseconds / 1000000) / Period) - Center;
    position(Position);
}

void Servo::center(float seconds)
{
    Center = seconds / Period;
    position(Position);
}

void Servo::center_ms(int milliseconds)
{
    Center = (milliseconds / 1000) / Period;
    position(Position);
}

void Servo::center_us(int microseconds)
{
    Center = (microseconds / 1000000) / Period;
    position(Position);
}

void Servo::minimum(float seconds)
{
    LowerRange = (seconds / Period) - Center;
    position(Position);
}

void Servo::minimum_ms(int milliseconds)
{
    LowerRange = ((milliseconds / 1000) / Period) - Center;
    position(Position);
}

void Servo::minimum_us(int microseconds)
{
    LowerRange = ((microseconds / 1000000) / Period) - Center;
    position(Position);
}

void Servo::period(float seconds)
{
    Period = seconds;
    SignalLine.period(seconds);
}

void Servo::period_ms(int milliseconds)
{
    Period = milliseconds / 1000;
    SignalLine.period_ms(milliseconds);
}

void Servo::period_us(int microseconds)
{
    Period = microseconds / 1000000;
    SignalLine.period_us(microseconds);
}

void Servo::pulsewidth(float seconds)
{
    Position = (seconds / Period) - Center;
    if (Position >=0)
    {
        Position /= UpperRange;
    }
    else
    {
        Position /= LowerRange * -1;
    }
    SignalLine.pulsewidth(seconds);
}

void Servo::pulsewidth_ms(int milliseconds)
{
    Position = ((milliseconds / 1000) / Period) - Center;
    if (Position >=0)
    {
        Position /= UpperRange;
    }
    else
    {
        Position /= LowerRange * -1;
    }
    SignalLine.pulsewidth_ms(milliseconds);
}

void Servo::pulsewidth_us(int microseconds)
{
    Position = ((microseconds / 1000000) / Period) - Center;
    if (Position >=0)
    {
        Position /= UpperRange;
    }
    else
    {
        Position /= LowerRange * -1;
    }
    SignalLine.pulsewidth_us(microseconds);
}

float Servo::operator =(float assignment)
{
    position(assignment);
    return assignment;
}

Servo::operator float()
{
    return position();
}