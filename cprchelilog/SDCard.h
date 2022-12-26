//mbed Microcontroller Library
//SDCard Interface
//Copyright 2010
//Thomas Hamilton

#ifndef SDCardLibrary
#define SDCardLibrary

#include "stdint.h"
#include "mbed.h"
#include "FATFileSystem.h"

class SDCard : private FATFileSystem
{
    private:
        SPI DataLines;
        DigitalOut ChipSelect;
            //physical chip interface
        unsigned char CommandCRCTable[256];
            //CRC7 lookup table
        unsigned char DataCRCTable[512];
            //CRC CCITT lookup table
        unsigned char OCR[4];
            //operating conditions register
        unsigned char CSD[16];
            //card-specific data register
        unsigned char FSR[64];
            //function status register
        bool Version;
            //card version, low for 1, high for 2
        bool Capacity;
            //low for low-capacity, high for high-capacity
        bool CRCMode;
            //low to disable CRCs, high to enable CRCs
        unsigned int Timeout;
            //timeout limit
        unsigned int t;
            //timeout counter
        unsigned char Workspace[5];
            //all-purpose work area

        unsigned char Initialize();
            //complete all initialization operations
        void Command(unsigned char Index, unsigned int Argument, unsigned char* Response);
            //sends command to the SD card
        void CommandCRC(unsigned char* IndexPtr, unsigned int* ArgumentPtr, unsigned char* Result);
            //calculates the command CRC7 result with a lookup table
        void DataCRC(unsigned short Length, unsigned char* Data, unsigned char* Result);
            //calculates the CRC16 result of the input with a lookup table
        void GenerateCRCTable(
            unsigned char Size, unsigned long long Generator, unsigned char* Table);
            //pre-calculates CRC results for efficient checking

        virtual unsigned char disk_initialize();
        virtual unsigned char disk_status();
        virtual unsigned char disk_read(
            unsigned char* buff, unsigned long sector, unsigned char count);
        virtual unsigned char disk_write(
            const unsigned char* buff, unsigned long sector, unsigned char count);
        virtual unsigned char disk_sync();
        virtual unsigned long disk_sector_count();
        virtual unsigned short disk_sector_size();
        virtual unsigned long disk_block_size();
            //FAT system virtual function definitions, called by FAT module

    public:
        SDCard(PinName mosi, PinName miso, PinName sck, PinName cs, const char* DiskName);
            //constructor requires SPI and DigitalOut pins, and a directory name
        virtual ~SDCard();
            //destructor deallocates tables and workspace
        unsigned char Log(unsigned char Control, unsigned char Data);
            //multipurpose raw data-logging method with three modes of operation
            //Control   description
            //  0       synchronizes card and resets internal counter to finalize I/O operations
            //  1       successively write input to a raw data byte in order starting at address 0
            //  2       successively read and return a raw data byte in order starting at address 0
            //return data from sync or write operations, and input
            //data of sync or read operations are "don't care" bits
        unsigned char Write(unsigned int Address, unsigned char* Data);
        unsigned char Write(unsigned int Address, unsigned char SectorCount, unsigned char* Data);
            //write data sectors to the card
        unsigned char Read(unsigned int Address, unsigned char* Data);
        unsigned char Read(unsigned int Address, unsigned char SectorCount,  unsigned char* Data);
            //read data sectors from the card
        unsigned char SelectCRCMode(bool Mode);
            //turn CRC mode on or off, default is off
        void SetTimeout(unsigned int Retries);
            //change the number of retries for interface functions;
            //increase if lines are unreliable; default is 1024
};

#endif