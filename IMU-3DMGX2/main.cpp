/*
CP Robotics Club Helicopter Team
code for testing the IMU class


*/

#include "mbed.h"
#include "IMU3DMGX2.h"
 
IMU3DMGX2 imu(p9, p10);
Serial PC(USBTX,USBRX);

int main() {
    PC.baud(115200);    //fast speed so that it doesn't take alot time while printing
    bool isvalid;
    int i;          //loop variables
    //arrays for data recieved from IMU
    double accel[3];
    double angrate[3];
    double mag[3];
    double orient[9];
    double roll, pitch, yaw;
    
    for(i=0;i<10;i++){  //get ten readings in polled mode and print off
        isvalid = imu.getAccAngRate(accel, angrate); //C2 command; polled data collection
        
        PC.printf("IMU Accel x Reads %f\r\n",accel[0]);
        PC.printf("IMU accel y Reads %f\r\n",accel[1]);
        PC.printf("IMU accel z Reads %f\r\n",accel[2]);
        
        PC.printf("IMU ang rate x Reads %f\r\n",angrate[0]);
        PC.printf("IMU ang rate y Reads %f\r\n",angrate[1]);
        PC.printf("IMU ang rate z Reads %f\r\n",angrate[2]); 
        
        PC.printf("Validity: %d\r\n\r\n", isvalid);
        
        //wait_ms(100);
    }
    
    isvalid = imu.setContinuous(imu.CMD_GYROSTAB_ANGRATE_MAG);    //set IMU in continuous mode for command gyrostablize accel and angrates and mag vector
    PC.printf("Validity: %d\r\n\r\n", isvalid);
    PC.printf("Continuous mode:  %d\r\n\r\n", imu.continuous);
    
    for(i=0;i<10;i++){     //get 10 readings in continuos mode and print off
        isvalid = imu.getGyroStabAccAngRateMag(accel, angrate, mag); //D2
        
        PC.printf("IMU Accel x Reads %f\r\n",accel[0]);
        PC.printf("IMU accel y Reads %f\r\n",accel[1]);
        PC.printf("IMU accel z Reads %f\r\n",accel[2]);
        
        PC.printf("IMU ang rate x Reads %f\r\n",angrate[0]);
        PC.printf("IMU ang rate y Reads %f\r\n",angrate[1]);
        PC.printf("IMU ang rate z Reads %f\r\n",angrate[2]); 
        
        PC.printf("IMU mag x Reads %f\r\n",mag[0]);
        PC.printf("IMU mag y Reads %f\r\n",mag[1]);
        PC.printf("IMU mag z Reads %f\r\n",mag[2]); 
        
        PC.printf("Validity: %d\r\n\r\n", isvalid);
        
        //wait_ms(150);
    }
    
    imu.stopContinuous();                       //stop continuous mode
    PC.printf("Continuous mode:  %d\r\n\r\n", imu.continuous);
    
    for(i=0;i<10;i++){  //get ten readings in polled mode and print off
        isvalid = imu.getEuler(&roll, &pitch, &yaw); //CE
        
        PC.printf("IMU Roll Reads %f\r\n",roll);
        PC.printf("IMU Pitch Reads %f\r\n",pitch);
        PC.printf("IMU Yaw Reads %f\r\n",yaw);
        
        PC.printf("Validity: %d\r\n\r\n", isvalid);
        
       // wait_ms(150);
    }
    
    while(1);           //just idle for testing 
    
    
}
    
