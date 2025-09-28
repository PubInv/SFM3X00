/*
 * =====================================================================================
 *
 *       Filename:  SFM4x000.cpp
 *
 *    Description:  Senserion SFM4x00 library
 *
 *        Version:  1.0
 *        Created:  04/16/2020 16:59:40
 *
 *   Organization:  Public Invention
 *
 *        License:  Sensirion BSD 3-Clause License
 *
 * =====================================================================================
 */

#include "SFM4X00.h"

#define SFM_DEBUG

uint8_t SFM4x00::sendCommand(uint16_t command)
{
  //Serial.println();
  //Serial.println(command, HEX);
  uint8_t b1 = (command & 0xFF00) >> 8;
  //Serial.println(b1, HEX);
  uint8_t b0 = command & 0x00FF;
  //Serial.println(b0, HEX);

  Wire.beginTransmission(byte(this->sensorAddress));
  Wire.write(byte(b1));
  Wire.write(byte(b0));
  return(Wire.endTransmission());
}


uint16_t SFM4x00::readData()
{
  uint8_t b[3];
  int sens_addy = this->sensorAddress; // breaking out the address like this prevents the compiler from throwing an error (sensorAddress is a u8_t)
  
  Wire.requestFrom(sens_addy, 3);

  b[1] = Wire.read();
  b[0] = Wire.read();
  b[2] = Wire.read();  //checksum
  uint16_t c {0};
  c = (b[1] << 8) | b[0];

  //Serial.println(c, HEX);
  //Serial.println(c, DEC);

  return c;
}


uint32_t SFM4x00::requestSerialNumber()
{
  sendCommand(READ_SERIAL_NUMBER_U);

  uint16_t upperBytes = readData();

  sendCommand(READ_SERIAL_NUMBER_L);

  uint16_t lowerBytes = readData();

  uint32_t serialNumber {0};

  serialNumber = ((uint32_t)upperBytes << 16) | lowerBytes;

 return serialNumber;
}


uint16_t SFM4x00::requestScaleFactor()
{
  sendCommand(READ_SCALE_FACTOR);

  int16_t scaleFactor = readData();

  return scaleFactor;
}

uint16_t SFM4x00::requestTScale()
{
  sendCommand(READ_TEMP_S_FACTOR);

  int16_t scaleFactor = readData();

  return scaleFactor;
}


uint16_t SFM4x00::requestOffset()
{
  sendCommand(READ_FLOW_OFFSET);

  uint16_t offset = readData();

  return offset;
  
}
uint16_t SFM4x00::requestTOffset()
{
  sendCommand(READ_TEMPERATURE_OFFSET);

  uint16_t offset = readData();

  return offset;
}

uint16_t SFM4x00::requestStatus()
{
 sendCommand(READ_STATUS_REGISTER);

  uint16_t offset = readData();

  return offset;    
}

void SFM4x00::setupFlowSensor()
{
  this->serialNumber = requestSerialNumber();
  this->flowOffset   = requestOffset();
  this->flowScale    = requestScaleFactor();
  this->temperatureOffset = requestTOffset();
  this->temperatureScale = requestTScale ();

  Serial.print("SN:");
  Serial.print(this->serialNumber);
  Serial.print("  FlowOffset:");
  Serial.print(this->flowOffset);
  Serial.print(" FlowScale:");
  Serial.print(this->flowScale);
  Serial.print("  TempOffset:");
  Serial.print(this->temperatureOffset);
  Serial.print("  TempScale:");
  Serial.print(this->temperatureScale);
  Serial.print(" SFM Status:");
  Serial.print(requestStatus());


  if(this-> flowScale == 800.0)
  {
    this->minFlow = SFM3400_MIN;
    this->maxFlow = SFM3400_MAX;
  }
  else if(this-> flowScale == 120.0)
  {
    this->minFlow = SFM3200_MIN;
    this->maxFlow = SFM3200_MAX;
  }
}


float SFM4x00::takeFlowMeas()
{
    uint8_t result =0;
    uint16_t status_result = 0xFFFF; 
    uint16_t flowdata = 0xFFFF;  // better error code needed here, even though the SFM4300 is a 14bit device.
    status_result = requestStatus();
    
    result = sendCommand(READ_FLOW_REGISTER);
  
  if(status_result > 0){
      #ifdef SFM_DEBUG
      Serial.println("FlowSens measurement problem");
      #endif
      return 0xFFFF; //Houston, we have a problem.
  }
  if(result == 0)
  {
      flowdata = readData();
      
      return ((float)flowdata - this->flowOffset) / this->flowScale;
      
  }else{
      #ifdef SFM_DEBUG
      Serial.println("FlowSens not ready");
      #endif
      return -2.0;
  }
  return -1.0;
}


void SFM4x00::begin()
{
  this->setupFlowSensor();

}


float SFM4x00::takeTempMeas()
{
    uint8_t result =0;
    uint16_t status_result = 0xFFFF; 
    uint16_t tempdata = 0xFFFF;  // better error code needed here, even though the SFM4300 is a 14bit device.
    
    status_result = requestStatus();
    
    result = sendCommand(READ_TEMPERATURE_MEASUREMENT);
  if(status_result == 0){
      #ifdef SFM_DEBUG
      Serial.println("TempSens measurement problem");
      #endif
      return 0xFFFF; //Houston, we have a problem.
  }
  if(result == 0)
  {
      tempdata = readData();
      
      return ((float)tempdata - this->temperatureOffset) / this->temperatureScale;
      
  }else{
      #ifdef SFM_DEBUG
      Serial.println("TempSens not ready");
      #endif
      return -2.0;
  }
  return -1.0;
}


bool SFM4x00::checkRange(uint16_t rawFlow)
{
  return ((rawFlow <= this->minFlow) || (rawFlow >= this-> maxFlow));
}


bool SFM4x00::checkRange(float computedFlow)
{
  float min_f = ((float)this->minFlow - this->flowOffset) / this->flowScale;
  float max_f = ((float)this->maxFlow - this->flowOffset) / this->flowScale;

  return ((computedFlow <= min_f) || (computedFlow >= max_f));
}
