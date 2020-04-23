/***************************************************************************

Licensed under MIT license.

 ***************************************************************************/

/***************************************************************************
  Copyright Robert L. Read, 2020
 ***************************************************************************/


#include <Wire.h>
#include <SPI.h>
#include <SFM3X00.h>

// address of sensor
// usually 64 or 0x40 by default
#define FLOW_SENSOR_ADDRESS 0x40

// create insance of sensor with address 
SFM3X00 flowSensor(FLOW_SENSOR_ADDRESS);

//#define PIRDS_SENSIRION_SFM3200 0
//#define PIRDS_SENSIRION_SFM3400 1

// At present we have to install the SENSIRION_SFM3400 backwards
// because of the physical mounting!!!
bool SENSOR_INSTALLED_BACKWARD = true;

void setup() {

  Serial.begin(115200);
  Wire.begin();

  flowSensor.begin();

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

  while (!Serial);
  
  delay(300);
  
  zero_integration(millis());
}

// For calibrating, we will simply compute positive and negative volumes via integration,
// Setting to zero on putton press of the OLED
float G_volume = 0.0; 
float last_ms = 0.0;
float last_flow = 0.0;

void zero_integration(unsigned long ms) {
  G_volume = 0;
  last_ms = ms;
  last_flow = 0;
}

// Note: v units are milliliters
float add_to_running_integration(float v,unsigned long ms,float flow_millilters_per_minute) {
  float f = flow_millilters_per_minute;
    // Use a basic quadrilateral integration
  // we'll treat a very small flow as zero...
  if (abs(f) < 100.0) {
    f = 0.0;
  }
  float ml_per_ms = f / (60.0 * 1000.0);
  v += (ms - last_ms) * (ml_per_ms + last_flow)/2;
  last_ms = ms;
  last_flow = ml_per_ms;
  return v;
}

void loop() {
  short int incomingByte;
  while (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.print(incomingByte);
    if (incomingByte == '\n') { 
        Serial.println("Zeroing! 0000000000000000000000000000");
          zero_integration(millis());
    }
  }
  
 // we don't know how long the Serial operation will take,
 // so we read the millisecond clock again... 
  unsigned long ms = millis();

  float raw_flow_slm = flowSensor.readFlow();  // standard liters per minute
  float flow = (SENSOR_INSTALLED_BACKWARD) ? -raw_flow_slm : raw_flow_slm;

  float flow_milliliters_per_minute =  (flow * 1000.0);
  G_volume = add_to_running_integration(G_volume, ms,flow_milliliters_per_minute);
  
  Serial.print("(press return to zero) Volume: ");
  Serial.println(G_volume);
  delay(10);
}
