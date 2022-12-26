//mbed Microcontroller Library
//PHOTOSWITCH light-sensing switch 42KC library
//Copyright 2010
//Thomas Hamilton

//this is the header file of the library, it contains all of the libraries necessary
//to execute the class code, as well as the declarations of all members of the class

#ifndef P42KCLibrary
#define P42KCLibrary
    //this tells the compiler to check if there is already a copy of this
    //library in the program, and if there is not, then it makes one

#include "mbed.h"
    //this will include the mbed library file, which will allow us to use
    //its special hardware classes such as "InterruptIn" or "DigitalIn"

class B42KC //this tells the compiler you are defining a class named "P42KC"
{
    private:    //this is a scope specifier indicating the following members of
                //your class cannot be accessed by code from outside of your class

        DigitalIn Signal;   //this is a DigitalIn pin connected to the
                                //mbed, it is already allocated to memory
                                //and able to perform all DigitalIn functions

        DigitalIn* Stability;   //this is an address holder for a DigitalIn object that
                                //may have memory allocated to it later; it cannot perform
                                //any DigitalIn functions until its memory has been allocated,
                                //but maybe it won't be used and we can save the memory

    public:     //this is a scope specifier indicating the following members of
                //your class can be called and used by the code using this class

        B42KC(PinName op, PinName st);      //this member is the constructor, it has the same 
                                            //name as the class and must be public; it will
                                            //"set up" the class object so that it can be used
        
        bool Read();                        //this member will allow the user to read device output
};
    //this ends the class declaraion

#endif
    //this ends the compiler's library-inclusion "if" statement