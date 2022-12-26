//mbed Microcontroller Library
//SDCard Interface
//Copyright 2010
//Thomas Hamilton

#include "SDCard.h"

SDCard::SDCard(PinName mosi, PinName miso, PinName sck, PinName cs, const char* DiskName) :
    FATFileSystem(DiskName), DataLines(mosi, miso, sck), ChipSelect(cs), CRCMode(1), Timeout(1024)
{
    DataLines.frequency(100000);
        //set universal speed
    ChipSelect.write(1);
        //chip select is active low
    GenerateCRCTable(1, 137, CommandCRCTable);
        //generate the command crc lookup table;
        //(generator polynomial x^7 + x^3 + 1 converts to decimal 137)
    GenerateCRCTable(2, 69665, DataCRCTable);
        //generate the command crc lookup table;
        //(generator polynomial x^16 + x^12 + x^5 + 1 converts to decimal 69665)
    Initialize();
        //run card setup operations
}

SDCard::~SDCard()
{
    delete[] CommandCRCTable;
    delete[] DataCRCTable;
    delete[] OCR;
    delete[] CSD;
    delete[] FSR;
    delete[] Workspace;
        //delete all card data register copies and workspaces
    delete this;
}

unsigned char SDCard::disk_initialize()
{ return 0x00; }
    //disc is initialized during construction
unsigned char SDCard::disk_status()
{ return 0x00; }
    //card is always initialized
unsigned char SDCard::disk_read(
    unsigned char* buff, unsigned long sector, unsigned char count)
{ return Read((unsigned int)sector, count, buff); }
    //read operations must efficiently complete multiple sector transactions
unsigned char SDCard::disk_write(
    const unsigned char* buff, unsigned long sector, unsigned char count)
{ return Write((unsigned int)sector, count, (unsigned char*)buff); }
    //write operations must efficiently complete multiple sector transactions
unsigned char SDCard::disk_sync()
{ return 0x00; }
    //all disc functions are synchronous
unsigned long SDCard::disk_sector_count()
{
    switch (CSD[0] & 0xC0)
    {
        case 0x00:
            return ((((CSD[6] & 0x03) << 10) | (CSD[7] << 2) | ((CSD[8] & 0xC0) >> 6)) + 1)
                * (1 << ((((CSD[9] & 0x03) << 1) | ((CSD[10] & 0x80) >> 7)) + 2));
                //calculate sector count as specified for version 1 cards
        case 0x40:
            return ((((CSD[7] & 0x3F) << 16) | (CSD[8] << 8) | CSD[9]) + 1) * 1024;
                //calculate sector count as specified for version 2 cards
        default:
            return 0;
    }
}
    //return number of sectors on card
unsigned short SDCard::disk_sector_size()
{ return 512; }
    //fix SD card sector size to 512 for all cards
unsigned long SDCard::disk_block_size()
{
    switch (CSD[0] & 0xC0)
    {
        case 0x00:
            return (CSD[10] << 1) | (CSD[11] >> 7) + 1;
                //calculate erase sector size for version 1 cards
        case 0x40:
            return 1;
                //erase sector size is given by allocation unit for version 2 cards
        default:
            return 0;
    }
}
    //return the number of sectors in an erase sector

unsigned char SDCard::Log(unsigned char Control, unsigned char Data)
{
    static unsigned char Mode = 0x00;
        //store previous operating mode to determine current behavior
    static unsigned short Index = 0;
        //store last written byte number of current memory block

    if (CRCMode)
    {
        SelectCRCMode(0);
    }
        //CRC's are not used in raw data mode

    switch (Control)
    {
        case 0x00:
            if (Mode)
            {
                ChipSelect.write(0);
                for (; Index < 512; Index++)
                {
                    DataLines.write(0xFF);
                }
                    //get through left over space, filling with 0xFF for write blocks
                DataLines.write(0xFF);
                DataLines.write(0xFF);
                    //get through CRC
                ChipSelect.write(1);
                if (Mode == 0x01)
                {
                    ChipSelect.write(0);
                    t = 0;
                    do
                    {
                        t++;
                    } while (((DataLines.write(0xFF) & 0x11) != 0x01) && (t < Timeout));
                        //get through data response token
                    while (!DataLines.write(0xFF));
                        //get through busy signal
                    DataLines.write(0xFD);
                    DataLines.write(0xFF);
                        //send stop transmission token
                    while (!DataLines.write(0xFF));
                        //get through busy signal
                    ChipSelect.write(1);
                    DataLines.write(0xFF);
                }
                    //finish write block
                else
                {
                    Command(12, 0, Workspace);
                        //send stop transmission command
                    ChipSelect.write(0);
                    while (!DataLines.write(0xFF));
                        //get through busy signal
                    ChipSelect.write(1);
                    DataLines.write(0xFF);
                }
                    //finish read block
                Index = 0;
                Mode = 0x00;
                    //reset index to start and mode to synced
            }
            return 0xFF;
                //control code 0 synchronizes the card

        case 0x01:
            if (Mode != 0x01)
            {
                Log(0, 0);
                Command(25, 0, Workspace);
                Mode = 0x01;
            }
                //if previous call was not a write operation, sync the card,
                //start a new write block, and set function to write mode
            if (Index == 0)
            {
                ChipSelect.write(0);
                DataLines.write(0xFC);
                DataLines.write(Data);
                ChipSelect.write(1);
                Index++;
            }
                //if the index is at the start, send the start block token before the byte
            else if (Index < 511)
            {
                ChipSelect.write(0);
                DataLines.write(Data);
                ChipSelect.write(1);
                Index++;
            }
                //if the index is between the boundaries, simply write the byte
            else
            {
                ChipSelect.write(0);
                DataLines.write(Data);
                DataLines.write(0xFF);
                DataLines.write(0xFF);
                t = 0;
                do
                {
                    t++;
                } while (((DataLines.write(0xFF) & 0x11) != 0x01) && (t < Timeout));
                while (!DataLines.write(0xFF));
                ChipSelect.write(1);
                Index = 0;
            }
                //if the index is at the last address, get through CRC,
                //Data response token, and busy signal and reset the index
            return 0xFF;
                //return stuff bits; control code 1 writes a byte

        case 0x02:
            if (Mode != 0x02)
            {
                Log(0, 0);
                Command(18, 0, Workspace);
                Mode = 0x02;
            }
                //if previous call was not a read operation, sync the card,
                //start a new read block, and set function to read mode
            if (Index == 0)
            {
                ChipSelect.write(0);
                t = 0;
                do
                {
                    t++;
                } while ((DataLines.write(0xFF) != 0xFE) && (t < Timeout));
                Workspace[0] = DataLines.write(0xFF);
                ChipSelect.write(1);
                Index++;
                return Workspace[0];
            }
                //if the index is at the start, get the start block token and read the first byte
            else if (Index < 511)
            {
                ChipSelect.write(0);
                Workspace[0] = DataLines.write(0xFF);
                ChipSelect.write(1);
                Index++;
                return Workspace[0];
            }
                //if the index is between the boundaries, simply read the byte
            else
            {
                ChipSelect.write(0);
                Workspace[0] = DataLines.write(0xFF);
                DataLines.write(0xFF);
                DataLines.write(0xFF);
                ChipSelect.write(1);
                Index = 0;
                return Workspace[0];
            }
                //if the index is at the last address, get through
                //CRC and reset the index; control code 2 reads a byte

        default:
            return 0xFF;
                //return stuff bits
    }
}

unsigned char SDCard::Write(unsigned int Address, unsigned char* Data)
{
    if (!Capacity)
    {
        Command(24, Address * 512, Workspace);
    }
    else
    {
        Command(24, Address, Workspace);
    }
        //send single block write command; addressing depends on the card version
    if (Workspace[0])
    { return 0x04; }
        //if a command error occurs, return parameter error
    DataCRC(512, Data, Workspace);
        //calculate the data CRC
    ChipSelect.write(0);
    DataLines.write(0xFE);
        //write start block token
    for (unsigned short i = 0; i < 512; i++)
    {
        DataLines.write(Data[i]);
    }
        //write the data to the addressed card sector
    DataLines.write(Workspace[0]);
    DataLines.write(Workspace[1]);
        //write the data CRC to the card
    t = 0;
    do
    {
        Workspace[0] = DataLines.write(0xFF);
        t++;
    } while (((Workspace[0] & 0x11) != 0x01) && (t < Timeout));
        //gather the data block response token
    while (!DataLines.write(0xFF));
        //get through the busy signal
    ChipSelect.write(1);
    DataLines.write(0xFF);
    if (((Workspace[0] & 0x1F) != 0x05) || (t == Timeout))
    { return 0x01; }
    else
    { return 0x00; }
        //if data response token indicates error, return R/W error
}
unsigned char SDCard::Write(unsigned int Address, unsigned char SectorCount, unsigned char* Data)
{
    static unsigned char CurrentSectorCount = 1;
        //store the last write sector count
    if (SectorCount != CurrentSectorCount)
    {
        Command(55, 0, Workspace);
        Command(23, SectorCount, Workspace);
        if (Workspace[0])
        { return 0x04; }
        CurrentSectorCount = SectorCount;
    }
        //set the expected number of write blocks if different from previous operations
    if (!Capacity)
    {
        Command(25, Address * 512, Workspace);
    }
    else
    {
        Command(25, Address, Workspace);
    }
    if (Workspace[0])
    { return 0x04; }
    Workspace[4] = 0x00;
    for (unsigned char i = 0; i < SectorCount; i++)
    {
        DataCRC(512, &Data[i * 512], Workspace);
            //calculate data crc for each passed write block
        ChipSelect.write(0);
        DataLines.write(0xFC);
            //send multiple write block start token
        for (unsigned int j = i * 512; j < (i + 1) * 512; j++)
        {
            DataLines.write(Data[j]);
        }
            //write each data block
        DataLines.write(Workspace[0]);
        DataLines.write(Workspace[1]);
        t = 0;
        do
        {
            Workspace[0] = DataLines.write(0xFF);
            t++;
        } while (((Workspace[0] & 0x11) != 0x01) && (t < Timeout));
        while (!DataLines.write(0xFF));
        ChipSelect.write(1);
        Workspace[4] |= Workspace[0];
            //record if any write errors are detected in the data response tokens
        if (t == Timeout)
        {
            ChipSelect.write(0);
            DataLines.write(0xFD);
            DataLines.write(0xFF);
            while (!DataLines.write(0xFF));
            ChipSelect.write(1);
            DataLines.write(0xFF);
            return 0x01;
        }
            //if a block write operation gets timed out, make sure the card is synced and exit
    }
    ChipSelect.write(0);
    DataLines.write(0xFD);
    DataLines.write(0xFF);
    while (!DataLines.write(0xFF));
    ChipSelect.write(1);
    DataLines.write(0xFF);
    if ((Workspace[4] & 0x1F) != 0x05)
    { return 0x01; }
    else
    { return 0x00; }
}

unsigned char SDCard::Read(unsigned int Address, unsigned char* Data)
{
    if (!Capacity)
    {
        Command(17, Address * 512, Workspace);
    }
    else
    {
        Command(17, Address, Workspace);
    }
        //send single block read command; addressing depends on the card version
    if (Workspace[0])
    { return 0x04; }
        //if a command error occurs, return parameter error
    ChipSelect.write(0);
    t = 0;
    do
    {
        t++;
    } while ((DataLines.write(0xFF) != 0xFE) && (t < Timeout));
    if (t == Timeout) { ChipSelect.write(1); DataLines.write(0xFF); return 0x01; }
        //get to start block token
    for (unsigned short i = 0; i < 512; i++)
    {
        Data[i] = DataLines.write(0xFF);
    }
        //read the data from the addressed card sector
    Workspace[2] = DataLines.write(0xFF);
    Workspace[3] = DataLines.write(0xFF);
        //read the data CRC to the card
    ChipSelect.write(1);
    DataLines.write(0xFF);
    DataCRC(512, Data, Workspace);
        //calculate the data CRC
    if (CRCMode && ((Workspace[0] != Workspace[2]) || (Workspace[1] != Workspace[3])))
    { return 0x01; }
    else
    { return 0x00; }
        //if CRC is invalid, return R/W error
}
unsigned char SDCard::Read(unsigned int Address, unsigned char SectorCount, unsigned char* Data)
{
    if (!Capacity)
    {
        Command(18, Address * 512, Workspace);
    }
    else
    {
        Command(18, Address, Workspace);
    }
    if (Workspace[0])
    { return 0; }
    Workspace[4] = 0x00;
    for (unsigned char i = 0; i < SectorCount; i++)
    {
        ChipSelect.write(0);
        t = 0;
        do
        {
            t++;
        } while ((DataLines.write(0xFF) != 0xFE) && (t < Timeout));
            //get to each data block start token
        if (t == Timeout)
        {
            ChipSelect.write(1);
            Command(12, 0, Workspace);
            ChipSelect.write(0);
            while (!DataLines.write(0xFF));
            ChipSelect.write(1);
            DataLines.write(0xFF);
            return 0x01;
        }
            //if a block read operation gets timed out, make sure the card is synced and exit
        for (unsigned int j = i * 512; j < (i + 1) * 512; j++)
        {
            Data[j] = DataLines.write(0xFF);
        }
            //read each data block
        Workspace[2] = DataLines.write(0xFF);
        Workspace[3] = DataLines.write(0xFF);
        ChipSelect.write(1);
        DataCRC(512, &Data[i * 512], Workspace);
            //calculate data crc for each read data block
        Workspace[4] |= ((Workspace[0] != Workspace[2]) || (Workspace[1] != Workspace[3]));
            //record if any invalid CRCs are detected during the transaction
    }
    Command(12, 0, Workspace);
    ChipSelect.write(0);
    while (!DataLines.write(0xFF));
    ChipSelect.write(1);
    if (Workspace[4])
    { return 0x01; }
    else
    { return 0x00; }
}

unsigned char SDCard::SelectCRCMode(bool Mode)
{
    t = 0;
    do
    {
        Command(59, Mode, Workspace);
            //command 59 sets card CRC mode
        t++;
    } while (Workspace[0] && (t < Timeout));
    CRCMode = Mode;
    if (t == Timeout)
    { return 0x01; }
    else
    { return 0x00; }
        //if command times out, return error
}

void SDCard::SetTimeout(unsigned int Retries)
{
    Timeout = Retries;
}
    //set c=number of retries for card operations

unsigned char SDCard::Initialize()
{
    for (unsigned char i = 0; i < 16; i++)
    {
        DataLines.write(0xFF);
            //clock card at least 74 times to power up
    }

    t = 0;
    do
    {
        Command(0, 0, Workspace);
            //send command 0 to put the card into SPI mode
        t++;
    } while ((Workspace[0] != 0x01) && (t < Timeout));
    if (t == Timeout) { return 0x01; }

    t = 0;
    do
    {
        Command(59, 1, Workspace);
            //turn on CRCs
        t++;
    } while ((Workspace[0] != 0x01) && (Workspace[0] != 0x05) && (t < Timeout));
        //command 59 is not valid for all cards in idle state
    if (t == Timeout) { return 0x01; }

    t = 0;
    do
    {
        Command(8, 426, Workspace);
            //voltage bits are 0x01 for 2.7V - 3.6V,
            //check pattern 0xAA, [00,00,01,AA] = 426
        t++;
    } while (((Workspace[0] != 0x01) || ((Workspace[3] & 0x0F) != 0x01) ||
        (Workspace[4] != 0xAA)) && (Workspace[0] != 0x05) && (t < Timeout));
        //check version, voltage acceptance, and check pattern
    if (t == Timeout) { return 0x01; }
    Version = Workspace[0] != 0x05;
        //store card version

    t = 0;
    do
    {
        Command(58, 0, Workspace);
            //check the OCR
        t++;
    } while (((Workspace[0] != 0x01) ||
        !((Workspace[2] & 0x20) || (Workspace[2] & 0x10))) && (t < Timeout));
        //check for correct operating voltage 3.3V
    if (t == Timeout) { return 0x01; }

    t = 0;
    do
    {
        Command(55, 0, Workspace);
        Command(41, 1073741824, Workspace);
            //specify host supports high capacity cards, [40,00,00,00] = 1073741824
        t++;
    } while (Workspace[0] && (t < Timeout));
        //check if card is ready
    if (t == Timeout) { return 0x01; }

    if (SelectCRCMode(1))
    { return 0x01; }
        //turn on CRCs for all cards

    t = 0;
    do
    {
        Command(58, 0, Workspace);
            //check the OCR again
        t++;
    } while ((Workspace[0] || !(Workspace[1] & 0x80)) && (t < Timeout));
        //check power up status
    if (t == Timeout) { return 0x01; }
    for (unsigned char i = 0; i < 4; i++)
    {
        OCR[i] = Workspace[i + 1];
            //record OCR
    }
    Capacity = (OCR[0] & 0x40) == 0x40;
        //record capacity

    t = 0;
    do
    {
        do
        {
            Command(9, 0, Workspace);
                //read the card-specific-data register
            t++;
        } while (Workspace[0] && (t < Timeout));
        if (t == Timeout) { return 0x01; }
        ChipSelect.write(0);
        do
        {
            t++;
        } while ((DataLines.write(0xFF) != 0xFE) && (t < Timeout));
            //get to the start-data-block token
        if (t == Timeout) { ChipSelect.write(1); DataLines.write(0xFF); return 0x01; }
        for (unsigned char i = 0; i < 16; i++)
        {
            CSD[i] = DataLines.write(0xFF);
                //gather CSD
        }
        Workspace[2] = DataLines.write(0xFF);
        Workspace[3] = DataLines.write(0xFF);
            //save CSD CRC
        ChipSelect.write(1);
        DataLines.write(0xFF);
        DataCRC(16, CSD, Workspace);
            //calculate the CSD data CRC
        Workspace[4] = 0;
        for (unsigned char i = 0; i < 15; i++)
        {
            Workspace[4] = CommandCRCTable[Workspace[4]] ^ CSD[i];
        }
        Workspace[4] = CommandCRCTable[Workspace[4]] | 0x01;
            //calculate the CSD table CRC
        t++;
    } while (((Workspace[0] != Workspace[2]) || (Workspace[1] != Workspace[3]) ||
        (Workspace[4] != CSD[15])) && (t < Timeout));
        //check all CSD CRCs
    if (t == Timeout) { return 0x01; }

    if (((CSD[3] & 0x07) > 0x02) ||
        (((CSD[3] & 0x78) > 0x30) && ((CSD[3] & 0x07) > 0x01)))
    {
        DataLines.frequency(25000000);
            //maximum speed is 25MHz
    }
    else
    {
       Workspace[0] = 1;
        for (unsigned char i = 0; i < (CSD[3] & 0x07); i++)
        {
            Workspace[0] *= 10;
                //the first three bits are a power of ten multiplier for speed
        }
        switch (CSD[3] & 0x78)
        {
            case 0x08: DataLines.frequency(Workspace[0] * 100000); break;
            case 0x10: DataLines.frequency(Workspace[0] * 120000); break;
            case 0x18: DataLines.frequency(Workspace[0] * 140000); break;
            case 0x20: DataLines.frequency(Workspace[0] * 150000); break;
            case 0x28: DataLines.frequency(Workspace[0] * 200000); break;
            case 0x30: DataLines.frequency(Workspace[0] * 250000); break;
            case 0x38: DataLines.frequency(Workspace[0] * 300000); break;
            case 0x40: DataLines.frequency(Workspace[0] * 350000); break;
            case 0x48: DataLines.frequency(Workspace[0] * 400000); break;
            case 0x50: DataLines.frequency(Workspace[0] * 450000); break;
            case 0x58: DataLines.frequency(Workspace[0] * 500000); break;
            case 0x60: DataLines.frequency(Workspace[0] * 550000); break;
            case 0x68: DataLines.frequency(Workspace[0] * 600000); break;
            case 0x70: DataLines.frequency(Workspace[0] * 700000); break;
            case 0x78: DataLines.frequency(Workspace[0] * 800000); break;
            default: break;
                //read the CSD card speed bits and speed up card operations
        }
    }

    if (CSD[4] & 0x40)
        //check for switch command class support
    {
        t = 0;
        do
        {
            Command(6, 2147483649, Workspace);
                //switch to high-speed mode (SDR25, 50MHz)
            t++;
        } while (Workspace[0] && (Workspace[0] != 0x04) && (t < Timeout));
            //some cards that support switch class commands respond with illegal command
        if (t == Timeout) { return 0x01; }
        if (!Workspace[0])
        {
            do
            {
                ChipSelect.write(0);
                do
                {
                    t++;
                } while ((DataLines.write(0xFF) != 0xFE) && (t < Timeout));
                    //get to the start-data-block token
                if (t == Timeout) { ChipSelect.write(1); DataLines.write(0xFF); return 0x01; }
                for (unsigned char i = 0; i < 64; i++)
                {
                    FSR[i] = DataLines.write(0xFF);
                        //gather function-status register
                }
                Workspace[2] = DataLines.write(0xFF);
                Workspace[3] = DataLines.write(0xFF);
                    //record data CRC
                ChipSelect.write(1);
                DataLines.write(0xFF);
                DataCRC(64, FSR, Workspace);
                    //calculate CRC
                t++;
            } while (((Workspace[0] != Workspace[2]) || (Workspace[1] != Workspace[3])) &&
                (t < Timeout));
                //complete CRC
            if (t == Timeout) { return 0x01; }
            if ((FSR[13] & 0x02) && ((FSR[16] & 0x0F) == 0x01))
            {
                DataLines.frequency(50000000);
                    //increase speed if function switch was successful
            }
        }
    }

    if (!Version)
    {
        t = 0;
        do
        {
            Command(16, 512, Workspace);
                //set data-block length to 512 bytes
            t++;
        } while (Workspace[0] && (t < Timeout));
        if (t == Timeout) { return 0x01; }
    }

    if (SelectCRCMode(0))
    { return 0x01; }
        //turn off CRCs

    return 0x00;
}

void SDCard::Command(unsigned char Index, unsigned int Argument, unsigned char* Response)
{
    CommandCRC(&Index, &Argument, Workspace);
        //calculate command CRC
    ChipSelect.write(0);
        //assert chip select low to synchronize command
    DataLines.write(0x40 | Index);
        //the index is assumed valid, commands start with "01b"
    DataLines.write(((char*)&Argument)[3]);
    DataLines.write(((char*)&Argument)[2]);
    DataLines.write(((char*)&Argument)[1]);
    DataLines.write(((char*)&Argument)[0]);
        //send the argument bytes in order from MSB to LSB (mbed is little endian)
    DataLines.write(Workspace[0]);
        //send the command CRC
    t = 0;
    do
    {
        Response[0] = DataLines.write(0xFF);
            //clock the card high to let it run operations, the first byte will
            //be busy (all high), the response will be sent some time later
        t++;
    } while ((Response[0] & 0x80) && (t < Timeout));
        //check for a response by testing if the first bit is low
    if ((Index == 8) || (Index == 13) || (Index == 58))
    {
        for (unsigned char i = 1; i < 5; i++)
        {
            Response[i] = DataLines.write(0xFF);
        }
            //get the rest of the response
    }
    ChipSelect.write(1);
        //assert chip select high to synchronize command
    DataLines.write(0xFF);
        //clock the deselected card high to complete processing for some cards
}

void SDCard::CommandCRC(unsigned char* IndexPtr, unsigned int* ArgumentPtr, unsigned char* Result)
{
    if (CRCMode)
    {
        Result[0] =
            CommandCRCTable[
                CommandCRCTable[
                    CommandCRCTable[
                        CommandCRCTable[
                            CommandCRCTable[
                                *IndexPtr | 0x40
                            ] ^ ((char*)ArgumentPtr)[3]
                        ] ^ ((char*)ArgumentPtr)[2]
                    ] ^ ((char*)ArgumentPtr)[1]
                ] ^ ((char*)ArgumentPtr)[0]
            ] | 0x01;
    }
    else
    {
        Result[0] = 0xFF;
    }
        //using a CRC table, the CRC result of a byte is equal to the byte
        //in the table at the address equal to the input byte, a message CRC
        //is obtained by successively XORing these with the message bytes
}

void SDCard::DataCRC(unsigned short Length, unsigned char* Data, unsigned char* Result)
{
    if (CRCMode)
    {
        unsigned char Reference;
            //store the current CRC lookup value
        Result[0] = 0x00;
        Result[1] = 0x00;
            //initialize result carrier
        for (unsigned short i = 0; i < Length; i++)
            //step through each byte of the data to be checked
        {
            Reference = Result[0];
                //record current crc lookup for both bytes
            Result[0] = DataCRCTable[2 * Reference] ^ Result[1];
                //new fist byte result is XORed with old second byte result
            Result[1] = DataCRCTable[(2 * Reference) + 1] ^ Data[i];
                //new second byte result is XORed with new data byte
        }
        for (unsigned char i = 0; i < 2; i++)
            //the final result must be XORed with two 0x00 bytes.
        {
            Reference = Result[0];
            Result[0] = DataCRCTable[2 * Reference] ^ Result[1];
            Result[1] = DataCRCTable[(2 * Reference) + 1];
        }
    }
    else
    {
        Result[0] = 0xFF;
        Result[1] = 0xFF;
    }
}

void SDCard::GenerateCRCTable(unsigned char Size, unsigned long long Generator, unsigned char* Table)
{
    unsigned char Index[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        //this will hold information from the generator; the position indicates
        //the order of the encountered 1, the value indicates its position in
        //the generator, the 9th entry indicates the number of 1's encountered
    for (unsigned char i = 0; i < 64; i++)
    {
        if (((char*)&Generator)[7] & 0x80)
        { break; }
        Generator = Generator << 1;
            //shift generator so that the first bit is high
    }
    for (unsigned char i = 0; i < Size; i++)
    {
        Table[i] = 0x00;
            //initialize table
    }
    for (unsigned char i = 0; i < 8; i++)
        //increment through each generator bit
    {
        if ((0x80 >> i) & ((unsigned char*)&Generator)[7])
            //if a 1 is encountered in the generator
        {
            Index[Index[8]] = i;
            Index[8]++;
                //record its order and location and increment the counter
        }
        for (unsigned char j = 0; j < (0x01 << i); j++)
            //each bit increases the number of xor operations by a power of 2
        {
            for (unsigned char k = 0; k < Size; k++)
                //we need to perform operations for each byte in the CRC result
            {
                Table[(Size * ((0x01 << i) + j)) + k] = Table[(Size * j) + k];
                    //each new power is equal to all previous entries with an added
                    //xor on the leftmost bit and each succeeding 1 on the generator
                for (unsigned char l = 0; l < Index[8]; l++)
                    //increment through the encountered generator 1s
                {
                    Table[(Size * ((0x01 << i) + j)) + k] ^=
                        (((unsigned char*)&Generator)[7-k] << (i + 1 - Index[l]));
                    Table[(Size * ((0x01 << i) + j)) + k] ^=
                        (((unsigned char*)&Generator)[6-k] >> (7 - i + Index[l]));
                        //xor the new bit and the new generator 1s
                }
            }
        }
    }
}