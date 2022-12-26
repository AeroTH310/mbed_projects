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
#include "IMU3DMGX2.h"

IMU3DMGX2::IMU3DMGX2(PinName tx, PinName rx) : BIG_ENDIAN(false), dataLines(tx, rx), continuous(false) 
{
    dataLines.baud(115200);
    dataLines.format(8, Serial::None, 1);

}

IMU3DMGX2::~IMU3DMGX2() {
    delete this;
}

/*----------------------------------------------------------------------
 * getRawAccAngRate
 * gets the Raw Accelerometer and Angular Rate Sensor Outputs
 *
 * parameters:  accel : array of accelerations which will be filled
                angrate : array of angular rates which will be filled
 *
 * returns:     valid : if data is valid
 *--------------------------------------------------------------------*/
int IMU3DMGX2::getRawAccAngRate(double accel[3], double angrate[3]) { //C1
    uint8_t i, k;               //loop variables
    uint8_t valid;
    uint8_t cmd[1];
    uint8_t rep[31];            //length of reply for CMD_RAW

    cmd[0] = CMD_RAW;

    if (!continuous) {          //if not continuous have to send the command first
        valid = transaction(cmd, sizeof(cmd), rep, sizeof(rep), 1000);
    } else {                     //no need to send the command just receive the data
        valid = receive(cmd[0], rep, sizeof(rep), 1000);
    }
    k = 1;
    for (i = 0; i<3;i++) {
        accel[i] = floatFromBytes(rep+k);
        k += 4;
    }
    k = 13;
    for (i = 0; i<3;i++) {
        angrate[i] = floatFromBytes(rep+k);
        k += 4;
    }
    
    return valid;
}

/*----------------------------------------------------------------------
 * getAccAngRate
 * gets the Acceleration and Angular Rate
 *
 * parameters:  accel : array of accelerations which will be filled
                angrate : array of angular rates which will be filled
 *
 * returns:     valid : if data is valid
 *--------------------------------------------------------------------*/
int IMU3DMGX2::getAccAngRate(double accel[3], double angrate[3]) { //C2
    uint8_t i, k;               //loop variables
    uint8_t valid;
    uint8_t cmd[1];
    uint8_t rep[31];            //length of reply for CMD_ACCEL_ANGRATE

    cmd[0] = CMD_ACCEL_ANGRATE;

    if (!continuous) {          //if not continuous have to send the command first
        valid = transaction(cmd, sizeof(cmd), rep, sizeof(rep), 1000);
    } else {                     //no need to send the command just receive the data
        valid = receive(cmd[0], rep, sizeof(rep), 1000);
    }

    k = 1;
    for (i = 0; i<3;i++) {
        accel[i] = floatFromBytes(rep+k) * G;
        k += 4;
    }
    k = 13;
    for (i = 0; i<3;i++) {
        angrate[i] = floatFromBytes(rep+k);
        k += 4;
    }
    
    return valid;
}

/*----------------------------------------------------------------------
 * getAccAngRateOrient
 * gets the Acceleration, Angular Rate & Orientation Matrix
 *
 * parameters:  accel : array of accelerations which will be filled
                angrate : array of angular rates which will be filled
                orient : orientation matrix which will be filled
 *
 * returns:     valid : if data is valid
 *--------------------------------------------------------------------*/
int IMU3DMGX2::getAccAngRateOrient(double accel[3], double angrate[3], double orient[9]) { //C8
    uint8_t i, k;
    uint8_t valid;
    uint8_t cmd[1];
    uint8_t rep[67];                        //length of reply for CMD_ACCEL_ANGRATE_ORIENT

    cmd[0] = CMD_ACCEL_ANGRATE_ORIENT;

    if (!continuous) {          //if not continuous have to send the command first
        valid = transaction(cmd, sizeof(cmd), rep, sizeof(rep), 1000);
    } else {                     //no need to send the command just receive the data
        valid = receive(cmd[0], rep, sizeof(rep), 1000);
    }

    k = 1;
    for (i = 0; i<3;i++) {
        accel[i] = floatFromBytes(rep+k) * G;
        k += 4;
    }
    k = 13;
    for (i = 0; i<3;i++) {
        angrate[i] = floatFromBytes(rep+k);
        k += 4;
    }
    k = 25;
    for (i = 0; i<9;i++) {
        orient[i] = floatFromBytes(rep+k);
        k += 4;
    }
    
    return valid;
}

/*----------------------------------------------------------------------
 * getAccAngRateMag
 * gets the Acceleration, Angular Rate & Magnetometer Vector
 *
 * parameters:  accel : array of accelerations which will be filled
                angrate : array of angular rates which will be filled
                mag : array of magnetometer orientations which will be filled
 *
 * returns:     valid : if data is valid
 *--------------------------------------------------------------------*/
int IMU3DMGX2::getAccAngRateMag(double accel[3], double angrate[3], double mag[3]) { //CB
    uint8_t i, k;
    uint8_t valid;
    uint8_t cmd[1];
    uint8_t rep[43];                            //length of reply for CMD_ACCEL_ANGRATE_MAG   

    cmd[0] = CMD_ACCEL_ANGRATE_MAG;

    if (!continuous) {          //if not continuous have to send the command first
        valid = transaction(cmd, sizeof(cmd), rep, sizeof(rep), 1000);
    } else {                     //no need to send the command just receive the data
        valid = receive(cmd[0], rep, sizeof(rep), 1000);
    }

    k = 1;
    for (i = 0; i<3;i++) {
        accel[i] = floatFromBytes(rep+k) * G;
        k += 4;
    }
    k = 13;
    for (i = 0; i<3;i++) {
        angrate[i] = floatFromBytes(rep+k);
        k += 4;
    }
    k = 25;
    for (i = 0; i<3;i++) {
        mag[i] = floatFromBytes(rep+k);
        k += 4;
    }
    
    return valid;
}

/*----------------------------------------------------------------------
 * getAccAngRateMagOrient
 * gets the Acceleration, Angular Rate & Magnetometer Vector & Orientation Matrix
 *
 * parameters:  accel : array of accelerations which will be filled
                angrate : array of angular rates which will be filled
                mag : array of magnetometer orientations which will be filled
                orient : orientation matrix which will be filled
 *
 * returns:     valid : if data is valid
 *--------------------------------------------------------------------*/
int IMU3DMGX2::getAccAngRateMagOrient(double accel[3], double angrate[3], double mag[3], double orient[9]) { //CC
    uint8_t i, k;
    uint8_t valid;
    uint8_t cmd[1];
    uint8_t rep[79];                        //length of reply for CMD_ACCEL_ANGRATE_MAG_ORIENT

    cmd[0] = CMD_ACCEL_ANGRATE_MAG_ORIENT;

    if (!continuous) {          //if not continuous have to send the command first
        valid = transaction(cmd, sizeof(cmd), rep, sizeof(rep), 1000);
    } else {                     //no need to send the command just receive the data
        valid = receive(cmd[0], rep, sizeof(rep), 1000);
    }

    k = 1;
    for (i = 0; i<3;i++) {
        accel[i] = floatFromBytes(rep+k) * G;
        k += 4;
    }
    k = 13;
    for (i = 0; i<3;i++) {
        angrate[i] = floatFromBytes(rep+k);
        k += 4;
    }
    k = 25;
    for (i = 0; i<3;i++) {
        mag[i] = floatFromBytes(rep+k);
        k += 4;
    }
    k = 37;
    for (i = 0; i<9;i++) {
        orient[i] = floatFromBytes(rep+k);
        k += 4;
    }
    
    return valid;
}

/*----------------------------------------------------------------------
 * getEuler
 * gets Euler Angles
 *
 * parameters:  roll :    Pointer to roll value which will be filled
                pitch :    Pointer to pitch value which will be filled
                yaw :    Pointer to yaw value which will be filled
 *
 * returns:     valid : if data is valid
 *--------------------------------------------------------------------*/
int IMU3DMGX2::getEuler(double *roll, double *pitch, double *yaw) { //CE
    uint8_t valid;
    uint8_t cmd[1];
    uint8_t rep[19];                    //length of reply for CMD_EULER

    cmd[0] = CMD_EULER;

    if (!continuous) {          //if not continuous have to send the command first
        valid = transaction(cmd, sizeof(cmd), rep, sizeof(rep), 1000);
    } else {                     //no need to send the command just receive the data
        valid = receive(cmd[0], rep, sizeof(rep), 1000);
    }

    *roll = floatFromBytes(rep + 1);
    *pitch = floatFromBytes(rep + 5);
    *yaw = floatFromBytes(rep + 9);
    
    return valid;
}

/*----------------------------------------------------------------------
 * getEuler
 * gets Euler Angles and Angular Rates
 *
 * parameters:  roll :    Pointer to roll value which will be filled
                pitch :    Pointer to pitch value which will be filled
                yaw :    Pointer to yaw value which will be filled
                angrate : array of angular rates which will be filled
 *
 * returns:     valid : if data is valid
 *--------------------------------------------------------------------*/
int IMU3DMGX2::getEulerAngRates(double *roll, double *pitch, double *yaw, double angrate[3]) { //CF
    uint8_t i, k;
    uint8_t valid;
    uint8_t cmd[1];
    uint8_t rep[31];                    //length of reply for CMD_EULER_ANGRATE

    cmd[0] = CMD_EULER_ANGRATE;

    if (!continuous) {          //if not continuous have to send the command first
        valid = transaction(cmd, sizeof(cmd), rep, sizeof(rep), 1000);
    } else {                     //no need to send the command just receive the data
        valid = receive(cmd[0], rep, sizeof(rep), 1000);
    }

    *roll = floatFromBytes(rep + 1);
    *pitch = floatFromBytes(rep + 5);
    *yaw = floatFromBytes(rep + 9);
    k = 13;
    for (i = 0; i < 3; i++) {
        angrate[i] = floatFromBytes(rep+k);
        k += 4;
    }
    
    return valid;
}

/*----------------------------------------------------------------------
 * getGyroStabAccAngRateMag
 * gets the Gyro Stabilized Acceleration, Angular Rate & Magnetometer Vector
 *
 * parameters:  accel : array of accelerations which will be filled
                angrate : array of angular rates which will be filled
                mag : array of magnetometer orientations which will be filled
 *
 * returns:     valid : if data is valid
 *--------------------------------------------------------------------*/
int IMU3DMGX2::getGyroStabAccAngRateMag(double accel[3], double angrate[3], double mag[3]) { //D2
    uint8_t i, k;
    uint8_t valid;
    uint8_t cmd[1];
    uint8_t rep[43];                        //length of reply for CMD_GYROSTAB_ANGRATE_MAG

    cmd[0] = CMD_GYROSTAB_ANGRATE_MAG;

    if (!continuous) {          //if not continuous have to send the command first
        valid = transaction(cmd, sizeof(cmd), rep, sizeof(rep), 1000);
    } else {                     //no need to send the command just receive the data
        valid = receive(cmd[0], rep, sizeof(rep), 1000);
    }

    k = 1;
    for (i = 0; i<3;i++) {
        accel[i] = floatFromBytes(rep+k) * G;
        k += 4;
    }
    k = 13;
    for (i = 0; i<3;i++) {
        angrate[i] = floatFromBytes(rep+k);
        k += 4;
    }
    k = 25;
    for (i = 0; i<3;i++) {
        mag[i] = floatFromBytes(rep+k);
        k += 4;
    }
    
    return valid;
}

/*----------------------------------------------------------------------
 * setContinuous
 * Sets the IMU in continuous Mode
 *
 * parameters:  command : Mode that will be output
 *
 * returns:     If sucessful changing mode
 *--------------------------------------------------------------------*/
bool IMU3DMGX2::setContinuous(cmd command) {
    uint8_t cmd[4];
    uint8_t rep[8];

    cmd[0] = CMD_CONTINUOUS;
    cmd[1] = 0xC1; //Confirms user intent
    cmd[2] = 0x29; //Confirms user intent
    cmd[3] = command;

    transaction(cmd, sizeof(cmd), rep, sizeof(rep), 1000);
    // Verify that continuous mode is set on correct command:
    if (rep[1] != command) {
        return false;
    }

    continuous = true;
    return true;
}

/*----------------------------------------------------------------------
 * stopContinuous
 * Takes the IMU out of continuous mode
 *
 * parameters:  none
 *
 * returns:     none
 *--------------------------------------------------------------------*/
void IMU3DMGX2::stopContinuous() {
    uint8_t cmd[1];

    cmd[0] = CMD_STOP_CONTINUOUS;

    send(cmd, sizeof(cmd));

    wait_ms(100);

    continuous = false;
}

/*----------------------------------------------------------------------
 * send
 * sends a packet to the IMU
 *
 * parameters:  cmd : pointer to the command
                cmd_len : length of command
 *
 * returns:     number of bytes written
 *--------------------------------------------------------------------*/
int IMU3DMGX2::send(void *cmd, int cmd_len) {
    int bytes;

    bytes = write(cmd, cmd_len);

    if (bytes != cmd_len) {
        bytes = -1;             //error in writting to IMU
    }

    return bytes;
}

/*----------------------------------------------------------------------
 * recieve
 * recieves a message from the IMU
 *
 * parameters:  command : IMU command
                rep     : pointer to reply buffer
                rep_len : length of reply
                timeout : length of timeout
 *
 * returns:     is data valid
 *--------------------------------------------------------------------*/
int IMU3DMGX2::receive(char command, void *rep, int rep_len, int timeout) {
    int nbytes, bytes, skippedBytes;
    uint8_t i,valid;
    uint16_t checksum = 0;

    skippedBytes = 0;

    *(uint8_t*)(rep) = 0;   //skip till find header
    while (*(uint8_t*)(rep) != command && skippedBytes < MAX_SKIP) {
        read(rep, 1);
        skippedBytes++;
    }
    bytes = 1;
    while (bytes < rep_len) {
        nbytes = read((uint8_t*)rep + bytes, rep_len-bytes);

        bytes += nbytes;
    }

    for (i = 0;i<rep_len-2;i++) {
        checksum += ((uint8_t*)rep)[i];
    }

    if (checksum != convert2ushort((unsigned char*)((uint8_t*)rep+rep_len-2)))
        valid = 0;
    else
        valid = 1;

    return valid;
}

/*----------------------------------------------------------------------
 * transaction
 * sends a packet to the IMU and waits for a reply
 *
 * parameters:  cmd : pointer to the command
                cmd_len : length of command
                rep     : pointer to reply buffer
                rep_len : length of reply
                timeout : length of timeout
 *
 * returns:     is data valid
 *--------------------------------------------------------------------*/
int IMU3DMGX2::transaction(void *cmd, int cmd_len, void *rep, int rep_len, int timeout) {
    send(cmd, cmd_len);

    return receive(*(uint8_t*)cmd, rep, rep_len, timeout);
}

/*
    The following to functions are the only things that will need to be changed
    when we switch to the gumstix.  These are the functions that deal with the
    serial port on whatever system that we are using.  On the mbed we will use
    the built in Serial Function.  On the gumstix, I'm assuming that we will have
    some standaed C libraries and be able to use some low level function calls
    from <unistd.h>.  Otherwise the gumstix's Serial methods will have to be
    implemented into these functions.
*/

/*----------------------------------------------------------------------
 * read
 * reads nbytes bytes into buff from serial port
 *
 * parameters:  buff : pointer to buffer where bytes will be stored
                nbytes : number of bytes to be read
 *
 * returns:     number of bytes read
 *--------------------------------------------------------------------*/
int IMU3DMGX2::read(void *buff, int nbytes) {
    int count = 0;

    while (count != nbytes) {
        if (dataLines.readable()) {
            ((char*)buff)[count] = dataLines.getc();  //might have to cast buff as a char
            count++;
        }
    }

    return count;
}

/*----------------------------------------------------------------------
 * write
 * writes nbytes bytes from buff to serial port
 *
 * parameters:  buff : pointer to buffer where bytes will be written from
                nbytes : number of bytes to be written
 *
 * returns:     number of bytes written
 *--------------------------------------------------------------------*/
int IMU3DMGX2::write(const void *buff, int nbytes) {
    int count = 0;

    while (count != nbytes) {
        if (dataLines.writeable()) {
            dataLines.putc(((char*)buff)[count]);  //might have to cast buff as a char
            count++;
        }
    }

    return count;
}

/*----------------------------------------------------------------------
 * FloatFromBytes
 * Converts bytes to Float.
 *
 * parameters:  pBytes : received buffer containing pointer to 4 bytes
 *
 * returns:     a float value.
 *--------------------------------------------------------------------*/
float IMU3DMGX2::floatFromBytes(const unsigned char* pBytes) {
    float f = 0;
    if (!BIG_ENDIAN) {
        ((char*)(&f))[0] = pBytes[3];
        ((char*)(&f))[1] = pBytes[2];
        ((char*)(&f))[2] = pBytes[1];
        ((char*)(&f))[3] = pBytes[0];
    } else {
        ((char*)(&f))[0] = pBytes[0];
        ((char*)(&f))[1] = pBytes[1];
        ((char*)(&f))[2] = pBytes[2];
        ((char*)(&f))[3] = pBytes[3];
    }

    return f;
}
/*----------------------------------------------------------------------
 * convert2short
 * Convert two adjacent bytes to an integer.
 *
 * parameters:  buffer : pointer to first of two buffer bytes.
 * returns:     the converted value aa a signed short -32 to +32k.
 *--------------------------------------------------------------------*/
short IMU3DMGX2::convert2short(unsigned char* buffer) {
    short x;
    if (!BIG_ENDIAN) {
        x = (buffer[0] <<8) + (buffer[1] & 0xFF);
    } else {
        x = (short)buffer;
    }
    return x;
}
/*----------------------------------------------------------------------
 * convert2ushort
 * Convert two adjacent bytes to a short.
 *
 * parameters:  buffer : pointer to first of two buffer bytes.
 * returns:     the converted value as a unsigned short 0-64k.
 *--------------------------------------------------------------------*/
unsigned short IMU3DMGX2::convert2ushort(unsigned char* buffer) {
    unsigned short x;
    if (!BIG_ENDIAN) {
        x = (buffer[0] <<8) + (buffer[1] & 0xFF);
    } else {
        x = (unsigned short)buffer;
    }
    return x;
}
/*----------------------------------------------------------------------
 * convert2long
 * Convert four adjacent bytes to a signed long.
 *
 * parameters:  buffer : pointer to a 4 byte buffer.
 * returns:     the converted value as a signed long.
 *--------------------------------------------------------------------*/
long IMU3DMGX2::convert2long(unsigned char* plbyte) {
    long l = 0;
    if (!BIG_ENDIAN) {
        l = (plbyte[0] <<24) + (plbyte[1] <<16) + (plbyte[2] <<8) + (plbyte[3] & 0xFF);
    } else {
        l = (long)plbyte;
    }
    return l;
}
/*----------------------------------------------------------------------
 * convert2ulong
 * Convert four adjacent bytes to a unsigned long.
 *
 * parameters:  buffer : pointer to a 4 byte buffer.
 * returns:     the converted value as a unsigned long.
 *--------------------------------------------------------------------*/
unsigned long IMU3DMGX2::convert2ulong(unsigned char* plbyte) {
    unsigned long ul = 0;
    if (!BIG_ENDIAN) {
        ul = (plbyte[0] <<24) + (plbyte[1] <<16) + (plbyte[2] <<8) + (plbyte[3] & 0xFF);
    } else {
        ul = (unsigned long)plbyte;
    }
    return ul;
}