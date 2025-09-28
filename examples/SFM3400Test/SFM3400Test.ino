/*
 * =====================================================================================
 *
 *       Filename:  SFM3400Test.ino
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

#include <SFM3X00.h>

// delay between readings
#define SAMPLE_DELAY   5000

// address of sensor
// usually 64 or 0x40 by default
#define FLOW_SENSOR_ADDRESS 0x40

// create insance of sensor with address 
SFM3X00 flowSensor(FLOW_SENSOR_ADDRESS);
#define CLOCK_SPEED_HZ 10000
void setup()
{
  // establish serial communication
  Wire.begin();

  Serial.begin(9600);
  // 50K works well with the Adrafruit I2C activator, but why push it?
 Wire.setClock(5000);
  
  // initialize sensor values and start measuring flow
  flowSensor.begin();
  delay(1000);

  // print various sensor values
  Serial.println();
  Serial.print("sensor serial number: ");
  Serial.println(flowSensor.serialNumber, HEX);
  Serial.print("sensor article number: ");
  Serial.println(flowSensor.articleNumber, HEX);
  Serial.println();
  Serial.print("read scale factor: ");
  Serial.println(flowSensor.flowScale);
  Serial.print("read flow offset: ");
  Serial.println(flowSensor.flowOffset);
  
  // display the sample rate in hertz
  Serial.print("sample rate: ");
  Serial.print(1000.0 / (float)SAMPLE_DELAY);
  Serial.println(" Hz");
  Serial.println();

  flowSensor.begin();
  delay(5);
}


void loop()
{

  // read flow from sensor and print
//  flowSensor.begin();

  
  float flow = flowSensor.readFlow();

  if(flowSensor.checkRange(flow))
  {
    Serial.print("flow exceeded sensor limits:  ");
    Serial.print(flow);
    Serial.println(" slm"); 
  }
  else
  {
    Serial.print("flow : ");
    Serial.println(flow);
     
  }


//  delay(SAMPLE_DELAY);
  Serial.println("reading in two!");
//     delay(2000); 
}
