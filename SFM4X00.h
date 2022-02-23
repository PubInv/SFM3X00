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

#ifndef SFM4X00_H
#define SFM4X00_H

#include <Arduino.h>
#include <Wire.h>

// max and min for SFM3400
#define SFM3400_MIN 0x80   // 128 decimal
#define SFM3400_MAX 0xFF80 // 65408 decimal

// max and min for SFM3200
#define SFM3200_MIN 0x0F4F // 3913 decimal
#define SFM3200_MAX 0xFB0C // 64268 decimal

// I2C commands
// see application note: https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/5_Mass_Flow_Meters/Sensirion_Mass_Flow_Meters_Application_Note_SFM3xxx.pdf
#define READ_SCALE_FACTOR               0x30DE
#define READ_TEMP_S_FACTOR              0x31AC
#define READ_FLOW_OFFSET                0x30DF
#define READ_TEMPERATURE_OFFSET         0x31AD
#define READ_FLOW_REGISTER              0x1000
#define SOFT_RESET                      0x2000
// ** see application note before using temperature measurement
#define READ_TEMPERATURE_MEASUREMENT    0x1001
#define READ_STATUS_REGISTER            0x1010

#define READ_SERIAL_NUMBER_U            0x31AE
#define READ_SERIAL_NUMBER_L            0x31AF
#define READ_ARTICLE_NUMBER_U           0x31E3
#define READ_ARTICLE_NUMBER_L           0x31E4



class SFM4x00
{
  public:
    uint8_t  sensorAddress;
    uint32_t serialNumber;
    uint32_t articleNumber;
    float    flowOffset;
    float    flowScale;
    float    temperatureOffset;
    float    temperatureScale;
    uint16_t minFlow;
    uint16_t maxFlow;

    // construct sensor with desired I2C address
    SFM4x00(byte address)
    {
      this->sensorAddress = address;
    }

    // send I2C command to sensor
    uint8_t sendCommand(uint16_t command);

    // read 2 bytes of dtaa from the sensor
    uint16_t readData();

    // reads offste values and starts flow measurment
    void begin();

    // initialize sensor with flow offset and scale values
    void setupFlowSensor();

    // read the serial number
    uint32_t requestSerialNumber();


    // read the flow scale factor
    uint16_t requestScaleFactor();

    // read the temperature scale factor
    uint16_t requestTScale();

    // read sensor flow offset value
    uint16_t requestOffset();

    // read sensor temperature offset value
    uint16_t requestTOffset();

    // read Status word from sensor
    uint16_t requestStatus();

    // Take one flow measurement
    float takeFlowMeas();

    // Take one temperature measurement
    float takeTempMeas();

    // returns 0 if measurment is within bounds
    // returns 1 if measurment is not within bounds
    bool checkRange(uint16_t rawFlow);
    bool checkRange(float computedFlow);
};


#endif
