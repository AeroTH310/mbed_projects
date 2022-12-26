//mbed Microcontroller Library
//Class designed to store and invoke function pointers and method pointers
//Copyright 2011
//Thomas Hamilton

#ifndef GenericFunctionHandlerLibrary
#define GenericFunctionHandlerLibrary

class GenericFunctionHandler
{
    private:
        bool FunctionPointerType;
        void (*FunctionPointer)();
        unsigned char* ObjectPointer;
        unsigned char* MethodPointer;
        void (GenericFunctionHandler::*InvokePointer)();

        template<class Class>
            void InvokeMethod()
        {
            (*(Class**)ObjectPointer->**(void (Class::**)())MethodPointer)();
        }

    public:
        GenericFunctionHandler(void (*FunctionAddress)())
            : FunctionPointerType(0), FunctionPointer(FunctionAddress)
        {
        }
            //This constructor accepts a function address and constructs an
            //instance of this class for handling a static function.  The
            //constructor prepares the function to be called when the the Call
            //method of this class object is called.
        template<class Class>
            GenericFunctionHandler(
                Class* ObjectAddress, void (Class::*MethodAddress)()) :
                FunctionPointerType(1),
                ObjectPointer(new unsigned char(sizeof(ObjectAddress))),
                MethodPointer(new unsigned char(sizeof(MethodAddress))),
                InvokePointer(&GenericFunctionHandler::InvokeMethod<Class>)
            {
                memcpy(ObjectPointer, (unsigned char*)&ObjectAddress,
                    sizeof(ObjectAddress));
                memcpy(MethodPointer, (unsigned char*)&MethodAddress,
                    sizeof(MethodAddress));
            }
            //This constructor template accepts a pointer to a class object
            //and a pointer to a method of that class and constructs an
            //instance of this class for handling a method of that class.  The
            //constructor constructor prepares the method to be called on the
            //class object when the Call method of this class object is
            //called.

        ~GenericFunctionHandler()
        {
            if (FunctionPointerType)
            {
                delete[] ObjectPointer;
                delete[] MethodPointer;
            }
        }

        void Call()
        {
            if (!FunctionPointerType)
            {
                (*FunctionPointer)();
            }
            else
            {
                (this->*InvokePointer)();
            }
        }
            //This method invokes the function or method pointer that was
            //passed into the constructor of this class.
        
        void operator ()()
        {
            if (!FunctionPointerType)
            {
                (*FunctionPointer)();
            }
            else
            {
                (this->*InvokePointer)();
            }
        }
            //This overload of the () operator allows shorthand invokation of
            //a GenericFunctionHandler object's function pointer such that it
            //appears as a standard function call.  For example:
            //GenericFunctionHandlerObject(); does the same thing as
            //GenericFunctionHandlerObject.Call();
};

#endif