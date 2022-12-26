//mbed Microcontroller Library
//Library designed as an interface to the mbed Pin Connect Block
//Copyright 2011
//Thomas Hamilton

#ifndef LPC1768PinConnectBlockLibrary
#define LPC1768PinConnectBlockLibrary

#include "cmsis.h"

#ifdef __cplusplus
extern "C"
{
#endif

namespace PinConnectBlock
{
    unsigned char ValidateInput(
        unsigned char Port, unsigned char Pin, unsigned char Mode);
        //This function accepts a Port number, Pin number, and Mode code for
        //a pin that is to be connected to microcontroller hardware.  It
        //validates the input for future use as input to connection setting
        //functions.  It will check for the validity of the Port and Pin and
        //the validity of the Function, Resistor, and Open Drain mode
        //selections for a valid Port and Pin.  The Mode codes are specified
        //as follows:
        //
        //Bits 7:5 - Reserved       ------------------------------------------
        //Bit   4  - OpenDrain Mode |   See documentation for the mapping of |
        //Bits 3:2 - Resistor Mode  | mode code values to connections.  The  |
        //Bits 1:0 - Function Mode  | reserved bits must be 0.               |
        //                          ------------------------------------------
        //The return value is a code with flags to alert the user of errors in
        //the input.  The errors are specified as follows:
        //
        //Bits 7:6 - Reserved
        //Bit   5  - OpenDrain Mode
        //Bit   4  - Resistor Mode  ------------------------------------------
        //Bit   3  - Function Mode  |   A value of 0 indicates there are no  |
        //Bit   2  - Mode           | errors.  A value of 1 indicates an     |
        //Bit   1  - Pin            | error in the input corresponding with  |
        //Bit   0  - Port           | that bit.  Reserved bits will be 0.    |
        //                          ------------------------------------------
    void SetPinConnection(
        unsigned char Port, unsigned char Pin, unsigned char Mode);
        //This function accepts a Port number, Pin number, and Mode code and
        //connects the selected Pin on the selected Port to the hardware
        //specified in the Mode code.  This function will not check for the
        //validity of the input and will write to a reserved or unrelated
        //sector if an invalid Port and Pin are specified.  It will reject any
        //portion of the Mode code that connects 1.0:1.17 to functions 10 or
        //11, 0.27:0.30 to resistor hardware, or 0.27:0.28 to open drain mode
        //hardware, and will complete any valid connections in the Mode code.
        //The Mode codes are specified as follows:
        //
        //Bits 7:5 - Reserved       ------------------------------------------
        //Bit   4  - OpenDrain Mode |   See documentation for the mapping of |
        //Bits 3:2 - Resistor Mode  | mode code values to connections.  The  |
        //Bits 1:0 - Function Mode  | reserved bits must be 0.               |
        //                          ------------------------------------------
    unsigned char GetPinConnection(
        unsigned char Port, unsigned char Pin);
        //This function accepts a Port number and Pin number and returns a
        //mode code that specifies the hardware connected to the selected Pin
        //on the selected Port.  This function will not check for the validity
        //of the input and will return junk bits from a reserved or unrelated
        //sector if an invalid Port and Pin are specified.  The Mode codes are
        //specified as follows:
        //
        //Bits 7:5 - Reserved       ------------------------------------------
        //Bit   4  - OpenDrain Mode |   See documentation for the mapping of |
        //Bits 3:2 - Resistor Mode  | mode code values to connections.  The  |
        //Bits 1:0 - Function Mode  | reserved bits must be 0.               |
        //                          ------------------------------------------
    void SetFunctionMode(
        unsigned char Port, unsigned char Pin, unsigned char Mode);
        //This function accepts a Port number, Pin number, and Mode code and
        //connects the selected Pin on the selected Port to the function
        //hardware specified in the Mode code.  This function will not check
        //for the validity of the input and will write to a reserved or
        //non-function mode sector if an invalid Port and Pin are specified.
        //Mode codes are specified as follows:
        //                          ------------------------------------------
        //                          |   See documentation for the mapping of |
        //Bits 7:2 - Reserved       | mode code values to connections.  The  |
        //Bits 1:0 - Function Mode  | reserved bits and modes must be 0.     |
        //                          ------------------------------------------
    unsigned char GetFunctionMode(unsigned char Port, unsigned char Pin);
        //This function accepts a Port number and Pin number and returns a
        //mode code that specifies the function hardware connected to the
        //selected Pin on the selected Port.  This function will not check for
        //the validity of the input and will return junk bits from a reserved
        //or non-function mode sector if an invalid Port and Pin are
        //specified.  The Mode codes are specified as follows:
        //                          ------------------------------------------
        //                          |   See documentation for the mapping of |
        //Bits 2:7 - Reserved       | mode code values to connections.  The  |
        //Bits 1:0 - Function Mode  | reserved bits will be 0.               |
        //                          ------------------------------------------
    void SetResistorMode(
        unsigned char Port, unsigned char Pin, unsigned char Mode);
        //This function accepts a Port number, Pin number, and Mode code and
        //connects the selected Pin on the selected Port to the resistor
        //hardware specified in the Mode code.  This function will not check
        //for the validity of the input and will write to a reserved or
        //non-resistor mode sector if an invalid Port and Pin are specified.
        //The Mode codes are       -------------------------------------------
        //specified as follows:    | Value - Connection                      |
        //                         |-----------------------------------------|
        //                         |   00  - Pull Up Resistor                |
        //                         |   01  - Repeater Mode                   |
        //Bits 7:2 - Reserved      |   10  - No Resistor                     |
        //Bits 1:0 - Resistor Mode |   11  - Pull Down Resistor              |
        //                         -------------------------------------------
    unsigned char GetResistorMode(unsigned char Port, unsigned char Pin);
        //This function accepts a Port number and Pin number and returns a
        //mode code that specifies the resistor hardware connected to the
        //selected Pin on the selected Port.  This function will not check for
        //the validity of the input and will return junk bits from a reserved
        //or non-resistor mode sector if an invalid Port and Pin are
        //specified.  The Mode     -------------------------------------------
        //codes are specified as   | Value - Connection                      |
        //follows:                 |-----------------------------------------|
        //                         |   00  - Pull Up Resistor                |
        //                         |   01  - Repeater Mode                   |
        //Bits 7:2 - Reserved      |   10  - No Resistor                     |
        //Bits 1:0 - Resistor Mode |   11  - Pull Down Resistor              |
        //                         -------------------------------------------
    void SetOpenDrainMode(
        unsigned char Port, unsigned char Pin, bool Mode);
        //This function accepts a Port number, Pin number, and Mode bit and
        //connects or disconnects the open drain hardware for the selected Pin
        //on the selected Port.  A 0 will disconnect the open drain hardware,
        //and a 1 will connect the open drain hardware.  This function will
        //not check for the validity of the input before making a connection,
        //and will write to a reserved or non-open drain mode sector if an
        //invalid Port and Pin are specified.
    bool GetOpenDrainMode(unsigned char Port, unsigned char Pin);
        //This function accepts a Port number and Pin number and returns a
        //mode bit that specifies the resistor hardware that is connected to
        //the selected Pin on the selected Port.  A value of 0 indicates that
        //the open drain hardware is disconnected, and a value of 1 indicates
        //that the open drain hardware is connected.  This function will not
        //check for the validity of the input and will return a junk bit from
        //a reserved or non-open drain sector if an invalid Port and Pin are
        //specified.
    void SetTraceMode(bool Mode);
        //This function accepts a Mode bit and connects or disconnects the
        //trace port hardware.  A value of 0 will disconnect the trace port
        //hardware, and a value of 1 will connect the trace port hardware.
        //The trace port uses pins 2.2:2.6, and will override any function,
        //resistor, or open drain hardware connected to these pins.
    bool GetTraceMode();
        //This function returns a mode bit that specifies the current
        //connection status of the trace port.  A value of 0 indicates that
        //the trace port is disconnected, and a value of 1 indicates that the
        //trace port is connected.
    void SetI2C0Mode(unsigned char Mode);
        //This function accepts a Mode code and connects or disconnects
        //specialized I2C hardware to the I2C0 bus on pins 0.27:0.28.  The
        //Mode codes are specified as follows:
        //                     -----------------------------------------------
        //Bit 1: I2C Fast Mode |   A value of 0 will disconnect the hard-    |
        //Bit 0: I2C Filter    | ware, and a value of 1 will connect it.     |
        //                     -----------------------------------------------
        //For non-I2C of these pins, the Filter and Fast Mode hardware should
        //be disconnected.  For normal I2C use of these pins, the Filter
        //hardware should be connected and the Fast Mode hardware should be
        //disconnected.  For I2C Fast Mode, the Filter hardware and Fast Mode
        //hardware should both be connected.
    unsigned char GetI2C0Mode();
        //This function returns a mode code that specifies the current
        //connection status of the I2C0 bus with its specialized I2C hardware.
        //The first two bits indicate the current hardware connection, and
        //the next four bits indicate errors in the connection.  The
        //connection status of SCL pin 0.28 must mach that of SDA pin 0.27.
        //If this is not the case, the bus will not function properly and the
        //function will return an error code.  The I2C Mode codes are
        //specified as follows:                   ----------------------------
        //                                        |   A value of o indicates |
        //Bit 5: Error - Only SCL Fast Mode is On | the hardware is discon-  |
        //Bit 4: Error - Only SDA Fast Mode is On | nected or the error is   |
        //Bit 3: Error - Only SCL Filter is On    | not present.  A value of |
        //Bit 2: Error - Only SDA Filter is On    | 1 indicates the hardware |
        //Bit 1: I2C Fast Mode                    | is connected or the      |
        //Bit 0: I2C Filter                       | error is present.        |
        //                                        ----------------------------
}

#ifdef __cplusplus
}
#endif

#endif