#include "mbed.h"
#include "SoftInterrupt.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

void Int1();void Int2();void Int3();void Int4();
SoftInterrupt Obj1(Int1);SoftInterrupt Obj2(Int2);SoftInterrupt Obj3(Int3);SoftInterrupt* Obj4;
void Int1()
{   led4 = !led4; wait(0.25);
    Obj2.TriggerInterrupt(); delete Obj4;   }
void Int2()
{   led1 = !led1; wait(0.25);
    Obj3.TriggerInterrupt(); Obj4 = new SoftInterrupt(Int4);   }
void Int3()
{   led2 = !led2; wait(0.25);
    Obj4->TriggerInterrupt();   }
void Int4()
{   led3 = !led3; wait(0.25);
    Obj1.TriggerInterrupt();   }
      
/*void Int1()
{   led1 = !led1; wait(0.25);
    Obj2.TriggerInterrupt();
    delete Obj4; SoftInterrupt Obj5(Int4); Obj5.TriggerInterrupt();   }
void Int2()
{   led2 = !led2; wait(0.25);
    Obj3.TriggerInterrupt();
    Obj4 = new SoftInterrupt(Int4);   }
void Int3()
{   led3 = !led3; wait(0.25);
    Obj4->TriggerInterrupt();   }
void Int4()
{   led4 = !led4; wait(0.25);
    Obj1.TriggerInterrupt();   }*/
    
int main()
{
    Obj2.TriggerInterrupt();
}