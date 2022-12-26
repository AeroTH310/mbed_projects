//mbed Microcontroller Library
//PHOTOSWITCH light-sensing switch 42KC library
//Copyright 2010
//Thomas Hamilton

//this is the code file, it defines the functionality of the class members.  the double colons
//indicate scope, the class to the left of "::" is the owner of the member to the right of "::"

#include "B42KC.h"
    //we need to include the header file to connect the declarations to their definitions

B42KC::B42KC(PinName op, PinName st) : Signal(op)
{
    if (st != NC)   //check if the user connected the stability pin
    {
        Stability = new DigitalIn(st);  //if the user did, dynamically allocate a DigitalIn object
    }
}
    //this is the constructors code block, the list after the single colon is called the
    //initialization list and sets the initial values of all variables and constructs all
    //(non-dynamically allocated) objects that will be used by the class (ex: Signal is set
    //to pin "op"); the code inside the code block is executed after the initialization list,
    //it is used for dynamically allocating memory and miscellaneous tasks; the constructor
    //is called when an object of this class is declared, and it "sets up" the class object

bool B42KC::Read()
{
    if (Stability)                  //if the stability object has been allocated
    {
        if (!Stability->read())     //and if the signal is stable (the datasheet
                                    //indicates the line is off when the signal is
                                    //stable, so we use the opposite "!" operator)
        {
            return Signal.read();   //then return the signal reading
        }
        else                        //otherwise
        {
            return 0;               //return: "signal is off"
        }
    }
    else                            //and if the stability line was not used
    {
        return Signal.read();       //just return the un-stabilized signal reading
    }
}
    //this is the definition of the Read method.  a method is a member that performs a function.
    //the Signal object has its own "read" method, and we call it with the "." operator, as
    //in: "Signal.read();"; which will return a 1 if the digital pin is on, or 0 if it is off.
    //This "Read" method will return a 1 if the signal is on and stable, or a 0 if the signal is
    //off or unstable.  dynamically allocated objects have no original identifier, so they must
    //be accessed through their address.  to access a member of such an object we use the "->"
    //operator in place of where we would normally use the "." operator.  ex: "Stability->read()"
    //means call the "read" member of the object allocated to the address stored in Stability.