#include "mbed.h"
#include "stdint.h"
#include "DirHandle.h"
#include "SDCard.h"

#include  "PwmIn.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
DigitalOut b(p19);
void RequestSyncRead();
bool Read(float* Data);
bool Checksum(unsigned char* Data);
void FillSerialBuffer();
Timer timer;

//Serial pc(USBTX, USBRX); // tx, rx

Serial DataLines(p9,p10);
unsigned char Buffer[79];
unsigned char BufferStart = 0;
unsigned char BufferEnd = 0;
unsigned char ModeData[4];


DigitalIn button(p20); // button that  tells the loop to close the file
float Range;
SDCard Logger(p5, p6, p7, p8, "SDCard");
int main()
{
   
    Logger.SelectCRCMode(1); 
   PwmIn* PwmInput = new PwmIn(p13);// range finder
   PwmIn* servoa = new PwmIn(p14); // collective
   PwmIn* servob = new PwmIn(p15);// tail
   PwmIn* servoc = new PwmIn(p16);//aileron
   PwmIn* servod = new PwmIn(p17);// pitch 
   PwmIn* servoe = new PwmIn(p18); // esc  
// pc.baud(9600);
  //       pc.printf("IMU Test");

    
  
    ModeData[0] = 0xC2;
    ModeData[1] = 6;
    ModeData[2] = 31;
    ModeData[3] = 0;
    
    float data[6];
    bool isvalid;
  //  pc.baud(9600);
    DataLines.baud(115200);
    DataLines.format(8, Serial::None, 1);
    DataLines.attach(&FillSerialBuffer, Serial::RxIrq);

    FILE *fp = fopen("/SDCard/logfile.csv", "w");
b.write(1);

    fprintf(fp, "Line,Time,AGL,Collective,Tail,Aileron,Pitch,ESC,X Accel,Y Accel,Z Accel,X Ang,Y Ang,Z Ang,Valid\r\n");
    timer.start();
int i =0;
    while(!button)
    {
    RequestSyncRead();
    wait_us(15000);
    isvalid = Read(data);
   /*
        pc.printf(" range  %f\r\n",PwmInput->pulsewidth()/343.2 );  
        pc.printf("servo a:  %f\r\n", servoa->read());  
        pc.printf("servo b:  %f\r\n", servob->read());  
        pc.printf("servo c:  %f\r\n", servoc->read());  
        pc.printf("servo d:  %f\r\n", servod->read());  
        pc.printf("servo e:  %f\r\n", servoe->read());  
        pc.printf("IMU Accel x Reads %f\r\n",data[0]);
        pc.printf("IMU accel y Reads %f\r\n",data[1]);
        pc.printf("IMU accel z Reads %f\r\n",data[2]);
        pc.printf("IMU ang rate x Reads %f\r\n",data[3]);
        pc.printf("IMU ang rate y Reads %f\r\n",data[4]);
        pc.printf("IMU ang rate z Reads %f\r\n",data[5]); 
        pc.printf("Data Valid: %d\r\n\r\n", isvalid); 
    */
        fprintf(fp, "%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d\r\n",
        i,timer.read_ms(),PwmInput->pulsewidth()/343.2,servoa->read(),servob->read(),
        servoc->read(),servod->read(),servoe->read(),data[0],data[1],data[2],data[3],data[4],data[5],isvalid);
  
     i++;  
        }
           fclose(fp);
        while (1)
        {
            led1 = !led1;
            wait_ms(250);
            led2 = !led2;
            wait_ms(250);
            led3 = !led3;
            wait_ms(250);
            led4 = !led4;
            wait_ms(250);
        }        

}

void RequestSyncRead()
{
    if (ModeData[3])
    {
        DataLines.putc(0xFA);
        ModeData[3] = 0;
    }
    DataLines.putc(ModeData[0]);
}
    //lazy switches to synchronous mode and sends polled mode command byte

bool Read(float* Data)
{
    bool Result;
    unsigned char t = 0;
    while ((Buffer[BufferStart] != ModeData[0]) && (t < ModeData[2]))
    {
        BufferStart++;
        BufferStart %= ModeData[2];
        t++;
    }
        //find the header byte
        //pc.printf("\n\r TEST header found at %d\n\r", BufferStart);
    Result = Checksum(&Buffer[BufferStart]);
        //compute checksum
    BufferStart++;
    BufferStart %= ModeData[2];
        //move past header byte
    if (t < ModeData[2])
    {
        for (unsigned int i = 0; i < ModeData[1]; i++)
        {
            for(unsigned int j = 3; j < 4; j--)
            {
                ((unsigned char*)&Data[i])[j] = Buffer[BufferStart];
                BufferStart++;
                BufferStart %= ModeData[2];
            }
        }
            //convert big endian bytes to little endian floats
        BufferStart += 6;
            //move index past timer and checksum bytes
        BufferStart %= ModeData[2];
    }
        //if the header search did not timeout
    return Result;
}

bool Checksum(unsigned char* Data)
{
    unsigned short Sum = 0;
    for (unsigned char i = 0; i < ModeData[2] - 2; i++)
    {
        Sum += Data[i];
    }
    return (((unsigned char*)&Sum)[0] == Data[ModeData[2] - 1])
        && (((unsigned char*)&Sum)[1] == Data[ModeData[2] - 2]);
}

void FillSerialBuffer()
{
    while (DataLines.readable())
    {
        Buffer[BufferEnd] = DataLines.getc();
        BufferEnd++;
        BufferEnd %= ModeData[2];
    }
}
    //this automatically reads in new serial data as it is received to
    //make a software buffer that is big enough to handle an entire
    //packet; the processor is about 1000 times faster than the data lines