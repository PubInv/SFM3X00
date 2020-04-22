/*
 * =====================================================================================
 *
 *       Filename:  SFM3X000.h
 *
 *    Description:  Senserion SFM3X00 library
 *
 *        Version:  1.0
 *        Created:  04/16/2020 16:59:40
 *
 *   Organization:  Public Invention
 *
 *        License:  Senserion BSD 3-Clause License
 *
 * =====================================================================================
 */

#ifndef SFM3X00_H
#define SFM3X00_H

#include <Wire.h>
#include <Arduino.h>

// I2C commands
// see application note: https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/5_Mass_Flow_Meters/Sensirion_Mass_Flow_Meters_Application_Note_SFM3xxx.pdf
#define READ_SCALE_FACTOR               0x30DE
#define READ_FLOW_OFFSET                0x30DF
#define START_CONTINUOUS_MEASUREMENT    0x1000
#define SOFT_RESET                      0x2000    
// ** see application note before using temperature measurement
#define START_TEMPERATURE_MEASUREMENT   0x1001

#define READ_SERIAL_NUMBER_U            0x31AE
#define READ_SERIAL_NUMBER_L            0x31AF
#define READ_ARTICLE_NUMBER_U           0x31E3
#define READ_ARTICLE_NUMBER_L           0x31E4



class SFM3X00
{
  public:
    uint8_t  sensorAddress;
    uint32_t serialNumber;
    uint32_t articleNumber;
    float    flowOffset;
    float    flowScale;
  
    // construct sensor with desired I2C address
    SFM3X00(byte address)
    {
      this->sensorAddress = address;
    }
   
    // send I2C command to sensor
    void sendCommand(uint16_t command);

    // read 2 bytes of dtaa from the sensor
    uint16_t readData();

    // reads offste values and starts flow measurment
    void begin();
    
    // initialize sensor with flow offset and scale values
    void setupFlowSensor();

    // reads raw flow and calculates the real flow based on sensor values
    // returns flow in slm
    float readFlow();

    // read the serial number
    uint32_t requestSerialNumber();
    
    // read the article number
    uint32_t requestArticleNumber();
    
    // read the scale factor
    uint16_t requestScaleFactor();
    
    // read sensor flow offset avlue
    uint16_t requestOffset();

    // start measuring flow
    void startContinuousMeasurement();
    
};


#endif 
