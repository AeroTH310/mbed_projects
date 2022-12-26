#include "mbed.h"
#include "MB1210.h"
//#include "SDCard.h"
#include "PwmIn.h"

//////////////////////////////
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
Serial Computer(USBTX, USBRX);
//////////////////////////////

//MB1210 Ranger(p12, p15, p13, p14);
//SDCard Logger(p5, p6, p7, p8, "SDCard");
PwmIn PWMReader(p15);

int main()
{
wait(1);
Computer.printf("reading: %d ms", PWMReader.pulsewidth_us());

    while (1)
    {
        if (PWMReader.pulsewidth_us() > 1500)
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
        else
        {
            led1 = 0;
            led2 = 0;
            led3 = 0;
            led4 = 0;
        }
    }



/*    Computer.baud(9600);
    Computer.format(8, Serial::None, 1);
    Ranger.Voltage(3.3);
    Ranger.Unit(39.370);
    Ranger.Mode(0);
    Logger.SelectCRCMode(1);
    
    FILE *fp = fopen("/SDCard/LOG.txt", "w");
    fprintf(fp, "start logging\r\n");
    
    for(int i = 0; i < 1000; i++)
    {
        Ranger.RequestSyncRead();
        wait(.1);
        fprintf(fp, "%f; \r\n", Ranger.Read());
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
        }*/
}