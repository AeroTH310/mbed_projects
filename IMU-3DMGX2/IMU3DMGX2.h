/*
* Andrew Olson - CP Robotics Club Helicopter Code - MircoStrain IMU
*
* 
*
* Some code adapted from http://www.ros.org/doc/api/microstrain_3dmgx2_imu/html/3dmgx2_8cc_source.html
* and from Microstrain's example code
*
* Probably should include some kind of GPL or something
*
*/

#ifndef IMU3DMGX2Lib                //doesn't like names starting with numbers
#define IMU3DMGX2Lib

#include "stdint.h"
#include "mbed.h"

#define MAX_SKIP 1000
#define G   9.80665

class IMU3DMGX2
{
    public:
        
        bool BIG_ENDIAN;
        
        bool continuous;
        
        /*Enumeration of possible IMU commands*/
        enum cmd {
            CMD_RAW                            = 0xC1,
            CMD_ACCEL_ANGRATE                  = 0xC2,
            CMD_DELVEL_DELANG                  = 0xC3,
            CMD_CONTINUOUS                     = 0xC4,
            CMD_ORIENT                         = 0xC5,
            CMD_ATT_UPDATE                     = 0xC6,
            CMD_MAG_VEC                        = 0xC7,
            CMD_ACCEL_ANGRATE_ORIENT           = 0xC8,   
            CMD_WRITE_ACCEL_BIAS               = 0xC9,
            CMD_WRITE_GYRO_BIAS                = 0xCA,
            CMD_ACCEL_ANGRATE_MAG              = 0xCB,
            CMD_ACCEL_ANGRATE_MAG_ORIENT       = 0xCC,
            CMD_CAPTURE_GYRO_BIAS              = 0xCD,
            CMD_EULER                          = 0xCE,
            CMD_EULER_ANGRATE                  = 0xCF,
            CMD_TEMPERATURES                   = 0xD1,
            CMD_GYROSTAB_ANGRATE_MAG           = 0xD2,
            CMD_DELVEL_DELANG_MAG              = 0xD3,
            CMD_DEV_ID_STR                     = 0xEA,
            CMD_STOP_CONTINUOUS                = 0xFA
        };
        
        IMU3DMGX2(PinName tx, PinName rx);     //serial output, serial input
        
        // Destructor
        ~IMU3DMGX2();                          // release resources
        
         bool setContinuous(cmd command);
         
         void stopContinuous();
         
         int getRawAccAngRate(double accel[3], double angrate[3]); //C1
         
         int getAccAngRate(double accel[3], double angrate[3]); //C2
         
         int getAccAngRateOrient(double accel[3], double angrate[3], double orient[9]); //C8
         
         int getAccAngRateMag(double accel[3], double angrate[3], double mag[3]); //CB
         
         int getAccAngRateMagOrient(double accel[3], double angrate[3], double mag[3], double orient[9]); //CC

         int getEuler(double *roll, double *pitch, double *yaw); //CE
         
         int getEulerAngRates(double *roll, double *pitch, double *yaw, double angrate[3]); //CF
         
         int getGyroStabAccAngRateMag(double accel[3], double angrate[3], double mag[3]); //D2
        
    private:
        
         Serial dataLines;
         
         int send(void *cmd, int cmd_len);
         
         int receive(char command, void *rep, int rep_len, int timeout);
         
         int transaction(void *cmd, int cmd_len, void *rep, int rep_len, int timeout);
         
         int read(void *buff, int nbytes);
         
         int write(const void *buff, int nbytes);
         
         float floatFromBytes(const unsigned char* pBytes);
         
         short convert2short(unsigned char* buffer);
         
         unsigned short convert2ushort(unsigned char* buffer);
         
         long convert2long(unsigned char* plbyte);
         
         unsigned long convert2ulong(unsigned char* plbyte);
           
};

#endif